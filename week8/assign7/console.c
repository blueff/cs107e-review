#include "gl.h"
#include "fb.h"
#include <console.h>
#include <font.h>
#include <malloc.h>
#include <printf.h>
#include <assert.h>
#include <string.h>

typedef union {
  struct {
    int x;
    int y;
  };
  struct {
    int width;
    int height;
  };
} v2;

static v2
V2(int x, int y)
{
  v2 result = { .x = x, .y = y };
  return result;
}

static struct {
  // Console area in characters
  v2 dim;

  // current cursor poistion
  v2 cursor;

  int font_width;
  int font_height;

  // char content[dim.height][dim.width],
  void *content;

  // because of scroll down
  // we need to use this index to determine which one is the first row
  int content_index;

  // Used to set the framebuffer y offset
  // NOTE: this is in character, not in pixel
  int y_offset;
} _console;

static color_t BG_COLOR = GL_BLACK;
static color_t CURSOR_COLOR = 0xff444444;
static color_t TEXT_COLOR = GL_GREEN;

static void
set_char(v2 pos, char c)
{
  int row = (pos.y + _console.content_index) % _console.dim.height;
  char(*content)[_console.dim.width] = _console.content;
  content[row][pos.x] = c;
}

// return 0 if not present
static char
get_char(v2 pos)
{
  int row = (pos.y + _console.content_index) % _console.dim.height;
  char(*content)[_console.dim.width] = _console.content;
  return content[row][pos.x];
}

static void
draw_background(void)
{
  gl_draw_rect(0,
    0,
    _console.font_width * _console.dim.width,
    _console.font_height * _console.dim.y,
    BG_COLOR);
}

static void
draw_cursor(v2 cursor)
{
  int font_width = _console.font_width;
  int font_height = _console.font_height;

  gl_draw_rect(cursor.x * font_width,
    cursor.y * font_height,
    font_width,
    font_height,
    CURSOR_COLOR);
}

// Font size block with background color
static void
draw_block(v2 pos)
{
  int font_width = _console.font_width;
  int font_height = _console.font_height;

  gl_draw_rect(pos.x * font_width,
    pos.y * font_height,
    font_width,
    font_height,
    BG_COLOR);
}

static void
draw_char(v2 pos, char c, int with_bg)
{
  int font_width = _console.font_width;
  int font_height = _console.font_height;

  int x = pos.x * font_width;
  int y = pos.y * font_height;

  if(with_bg) {
    gl_draw_rect(x, y, font_width, font_height, BG_COLOR);
  }

  gl_draw_char(x, y, c, TEXT_COLOR);
}

static void
scroll_down(void)
{
  // `content_index` points to the last line of the console
  // Clear it to zero
  memset((char *)_console.content + _console.content_index * _console.dim.width,
    0,
    _console.dim.width);

  _console.content_index = (_console.content_index + 1) % _console.dim.height;

  _console.y_offset += 1;
  fb_set_y_offset(_console.y_offset * _console.font_height);

  // Set last line of the framebuffer to be background color
  int pitch = fb_get_pitch();
  color_t *buffer = fb_get_draw_buffer();

  int start_index
    = (pitch / 4) * (_console.dim.height - 1) * _console.font_height;
  int end_index = start_index + (pitch / 4) * _console.font_height;

  for(int i = start_index; i < end_index; i++) {
    buffer[i] = BG_COLOR;
  }

  if(_console.y_offset == _console.dim.height) {
    _console.y_offset = 0;
    fb_set_y_offset(0);
    unsigned char *buffer = (unsigned char *)fb_get_draw_buffer();
    int length = pitch * fb_get_height();
    memcpy(buffer, buffer + length, length);
  }
}

static void
on_formfeed(void)
{
  draw_background();
  memset(_console.content, 0, _console.dim.x * _console.dim.y);
  _console.cursor.x = 0;
  _console.cursor.y = 0;
  draw_cursor(_console.cursor);
}

static void
on_carriage_return(void)
{
  char old_char = get_char(_console.cursor);
  if(old_char) {
    draw_char(_console.cursor, old_char, 1);
  }

  _console.cursor.x = 0;
  char new_char = get_char(_console.cursor);
  draw_cursor(_console.cursor);
  if(new_char) {
    draw_char(_console.cursor, new_char, 0);
  }
}

static void
on_cursor_right(void)
{
  v2 old_pos = _console.cursor;
  v2 new_pos = { .x = old_pos.x + 1, .y = old_pos.y };

  if(new_pos.x >= _console.dim.width) {
    new_pos.x = _console.dim.width - 1;
  }

  _console.cursor = new_pos;

  draw_block(old_pos);
  char old_char = get_char(old_pos);
  if(old_char) {
    draw_char(old_pos, old_char, 1);
  }

  draw_cursor(new_pos);
  char new_char = get_char(new_pos);
  if(new_char) {
    draw_char(new_pos, new_char, 0);
  }
}

static void
on_cursor_left(void)
{
  v2 old_pos = _console.cursor;
  v2 new_pos = { .x = old_pos.x - 1, .y = old_pos.y };

  if(new_pos.x < 0) {
    new_pos.x = 0;
    new_pos.y -= 1;

    if(new_pos.y < 0) {
      assert(!"don't support scroll up");
    }
  }
  _console.cursor = new_pos;

  char old_char = get_char(old_pos);
  char new_char = get_char(new_pos);

  draw_block(old_pos);
  if(old_char) {
    draw_char(old_pos, old_char, 1);
  }

  draw_cursor(new_pos);
  if(new_char) {
    draw_char(new_pos, new_char, 0);
  }
}

static void
on_newline(void)
{
  draw_block(_console.cursor);

  _console.cursor.y += 1;
  _console.cursor.x = 0;

  if(_console.cursor.y >= _console.dim.height) {
    scroll_down();
    _console.cursor.y -= 1;
  }
}

static void
on_char(char c)
{
  draw_char(_console.cursor, c, 1);
  set_char(_console.cursor, c);

  _console.cursor.x += 1;
  if(_console.cursor.x >= _console.dim.x) {
    _console.cursor.y += 1;
    _console.cursor.x = 0;

    if(_console.cursor.y >= _console.dim.y) {
      scroll_down();
      _console.cursor.y -= 1;
    }
  }

  draw_cursor(_console.cursor);

  char next_char = get_char(_console.cursor);
  if(next_char) {
    draw_char(_console.cursor, next_char, 0);
  }
}

// Screen resolution is 960x540
// Draw console in the center of the screen
void
console_init(unsigned int nrows, unsigned int ncols)
{
  int font_width = font_get_width();
  int font_height = font_get_height();

  gl_init(ncols * font_width, font_height * nrows, GL_FORCONSOLE);

  _console.dim = (v2){ .width = ncols, .height = nrows };

  _console.content = malloc(nrows * ncols);
  memset(_console.content, 0, nrows * ncols);

  _console.font_width = font_get_width();
  _console.font_height = font_get_height();

  draw_background();
  draw_cursor(V2(0, 0));
}

// Control sequences supported:
// `\n`: new line
// `\b`: backspace, move cursor left
// `\r`: carriage return, move cursor to the beginning
// `\f`: formfeed, clear all content
// `\a`: ring the bell
// `\e[C`: move cursor right
int
console_printf(const char *format, ...)
{
  int result = 0;

  // NOTE: assume this is enough
  char buf[1024];

  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);

  char *c = buf;
  while(*c) {
    switch(*c) {
      case '\n':
        on_newline();
        break;

      case '\b':
        on_cursor_left();
        break;

      case '\r':
        on_carriage_return();
        break;

      case '\f':
        on_formfeed();
        break;

      // Bell, do nothing
      case '\a':
        break;

      // Move the cursor right
      case '\e': {
        if(*(c + 1) == '[' && *(c + 2) == 'C') {
          c += 2;
          on_cursor_right();
        }
      } break;

      default:
        result++;
        on_char(*c);
        break;
    }

    c++;
  }

  return result;
}
