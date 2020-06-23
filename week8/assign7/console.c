#include <console.h>
#include <gl.h>
#include <font.h>
#include <malloc.h>
#include <printf.h>
#include <assert.h>

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

static struct {
  // Left padding and top padding
  v2 padding;

  // Console area in characters
  v2 dim;

  // Console area in pixels
  v2 size;

  v2 cursor; // current cursor poistion

  v2 end;

  // char content[dim.height][dim.width + 1],
  // extra byte for '\0'
  void *content;

  // For scrolling, which row to read
  int first_row_index;
} _console;

static color_t BG_COLOR = GL_BLACK;
static color_t CURSOR_COLOR = 0xff444444;
static color_t TEXT_COLOR = GL_GREEN;
static color_t PADDING_COLOR = 0xff2c3e4f;

void
console_set_color(color_t bg_color,
  color_t cursor_color,
  color_t text_color,
  color_t padding_color)
{
  BG_COLOR = bg_color;
  CURSOR_COLOR = cursor_color;
  TEXT_COLOR = text_color;
  PADDING_COLOR = padding_color;
}

static int
circular_add(int v, int total)
{
  return (v + 1) % total;
}

static char *
get_line(int y)
{
  char(*content)[_console.dim.width + 1] = _console.content;
  return content[y];
}

static void
draw_padding(void)
{
  v2 padding = _console.padding;
  v2 size = _console.size;
  int width = gl_get_width();
  int height = gl_get_height();

  gl_draw_rect(0, 0, padding.x, height, PADDING_COLOR);

  gl_draw_rect(padding.x, 0, size.width, padding.y, PADDING_COLOR);

  gl_draw_rect(padding.x,
    padding.y + size.height,
    size.width,
    height - padding.y - size.height,
    PADDING_COLOR);

  gl_draw_rect(padding.x + size.width,
    0,
    width - padding.x - size.width,
    height,
    PADDING_COLOR);
}

static void
draw_clear(void)
{
  v2 padding = _console.padding;
  v2 size = _console.size;
  gl_draw_rect(padding.x, padding.y, size.width, size.height, BG_COLOR);
}

void
draw_console(void)
{
  int font_width = font_get_width();
  int font_height = font_get_height();
  v2 cursor = _console.cursor;
  v2 padding = _console.padding;

  draw_clear();

  // Draw console
  for(int dy = 0; dy < _console.dim.height; dy++) {
    int y = (dy + _console.first_row_index) % _console.dim.height;

    // Draw cursor
    if(cursor.y == y) {
      gl_draw_rect(padding.x + cursor.x * font_width,
        padding.y + dy * font_height,
        font_width,
        font_height,
        CURSOR_COLOR);
    }

    char *line = get_line(y);
    if(line[0] != 0) {
      gl_draw_string(padding.x, padding.y + dy * font_height, line, TEXT_COLOR);
    }
  }

  gl_swap_buffer();
}

static void
scroll_down()
{
  int original_read_index = _console.first_row_index;
  _console.first_row_index
    = circular_add(original_read_index, _console.dim.height);
  char *line = get_line(original_read_index);
  line[0] = 0;
}

static void
move_cursor_left()
{
  v2 cursor = _console.cursor;
  cursor.x -= 1;

  if(cursor.x < 0) {
    // We dont' support 'scroll up'
    assert(cursor.y != _console.first_row_index);

    cursor.y -= 1;
    if(cursor.y < 0) {
      cursor.y += _console.dim.height;
    }
  }

  _console.cursor = cursor;
}

static void
move_cursor_right()
{
  v2 cursor = _console.cursor;
  cursor.x += 1;

  if(cursor.x == _console.dim.width) {
    cursor.y = circular_add(cursor.y, _console.dim.height);
    cursor.x = 0;

    if(cursor.y == _console.first_row_index) {
      scroll_down();
    }
  }

  _console.cursor = cursor;
}

static void
move_end_right()
{
  v2 end = _console.end;
  end.x += 1;
  char *line = get_line(end.y);
  line[end.x] = 0;

  if(end.x == _console.dim.width) {
    // Don't have to modify `first_row_index`
    // `move_cursor_right` will handle that
    end.y = (end.y + 1) % _console.dim.height;
    end.x = 0;
  }

  _console.end = end;
}

static void
put_newline(void)
{
  v2 cursor = _console.cursor;
  cursor.y = (cursor.y + 1) % _console.dim.height;
  cursor.x = 0;

  if(cursor.y == _console.first_row_index) {
    scroll_down();
  }

  _console.cursor = cursor;
  _console.end = cursor;
}

static void
put_char(char c)
{
  v2 cursor = _console.cursor;
  v2 end = _console.end;

  char *line = get_line(cursor.y);
  line[cursor.x] = c;

  if(cursor.x == end.x && cursor.y == end.y) {
    move_end_right();
  }

  move_cursor_right();
}

// Screen resolution is 960x540
// Draw console in the center of the screen
void
console_init(unsigned int nrows, unsigned int ncols)
{
  int screen_width = 1920 / 2;
  int screen_height = 1080 / 2;
  gl_init(screen_width, screen_height, GL_DOUBLEBUFFER);

  int font_width = font_get_width();
  int font_height = font_get_height();

  _console.dim = (v2){ .width = ncols, .height = nrows };
  _console.size = (v2){
    .width = font_width * _console.dim.width,
    .height = font_height * _console.dim.height,
  };

  _console.padding = (v2){ .x = (screen_width - _console.size.width) / 2,
    .y = (screen_height - _console.size.height) / 2 };

  _console.content = malloc(nrows * (ncols + 1));

  // Clear to zero
  for(int y = 0; y < nrows; y++) {
    char *line = get_line(y);
    line[0] = 0;
  }

  draw_padding();
  draw_clear();
  gl_swap_buffer();

  draw_padding();
  draw_clear();
  gl_swap_buffer();
}

void
console_clear(void)
{
  _console.cursor = (v2){ .x = 0, .y = 0 };
  _console.end = (v2){ .x = 0, .y = 0 };
  _console.first_row_index = 0;
  char *line = get_line(0);
  line[0] = 0;
}

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
      put_newline();
      break;

    case '\b':
      move_cursor_left();
      break;

    case '\r':
      _console.cursor.x = 0;
      break;

    case '\f':
      console_clear();
      break;

    // Bell
    case '\a':
      break;

    // Move the cursor right
    case 0x1b: {
      if(*(c + 1) == '[' && *(c + 2) == 'C') {
        c += 2;
        move_cursor_right();
      }
    } break;

    default:
      result++;
      put_char(*c);
      break;
    }

    c++;
  }

  draw_console();

  return result;
}
