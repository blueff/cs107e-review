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

#define PADDING_COLOR 0xff2c3e4f

static struct {
  // Left padding and top padding
  v2 padding;

  // Console area in characters
  v2 dim;

  // Console area in pixels
  v2 size;

  v2 cursor; // current cursor poistion

  v2 end;

  // nrows * (ncols + 1) size
  // Reserve extra byte for '\0'
  // char content[][]
  void *content;

  // For scrolling, which row to read
  int read_index;
} console_buffer;

static int
circular_add_index(int idx, int all)
{
  return (idx + 1) % all;
}

static char *
get_line(int y)
{
  char(*content)[console_buffer.dim.width + 1] = console_buffer.content;
  return content[y];
}

static void
draw_padding(void)
{
  v2 padding = console_buffer.padding;
  v2 size = console_buffer.size;
  int width = gl_get_width();
  int height = gl_get_height();

  color_t c = PADDING_COLOR;

  gl_draw_rect(0, 0, padding.x, height, c);

  gl_draw_rect(padding.x, 0, size.width, padding.y, c);

  gl_draw_rect(padding.x,
    padding.y + size.height,
    size.width,
    height - padding.y - size.height,
    c);

  gl_draw_rect(padding.x + size.width,
    0,
    width - padding.x - size.width,
    height,
    c);
}

static void
draw_clear(void)
{
  v2 padding = console_buffer.padding;
  v2 size = console_buffer.size;
  gl_draw_rect(padding.x, padding.y, size.width, size.height, GL_BLACK);
}

void
draw_console(void)
{
  int font_width = font_get_width();
  int font_height = font_get_height();
  v2 cursor = console_buffer.cursor;
  v2 padding = console_buffer.padding;

  draw_clear();

  // Draw console
  for(int dy = 0; dy < console_buffer.dim.height; dy++) {
    int y = (dy + console_buffer.read_index) % console_buffer.dim.height;

    // Draw cursor
    if(cursor.y == y) {
      gl_draw_rect(padding.x + cursor.x * font_width,
        padding.y + dy * font_height,
        font_width,
        font_height,
        0xff444444);
    }

    char *line = get_line(y);
    if(line[0] != 0) {
      gl_draw_string(padding.x, padding.y + dy * font_height, line, GL_GREEN);
    }
  }

  gl_swap_buffer();
}

static void
scroll_down()
{
  int original_read_index = console_buffer.read_index;
  console_buffer.read_index
    = circular_add_index(original_read_index, console_buffer.dim.height);
  char *line = get_line(original_read_index);
  line[0] = 0;
}

static void
move_cursor_left()
{
  v2 cursor = console_buffer.cursor;
  cursor.x -= 1;

  if(cursor.x < 0) {
    // We dont' support 'scroll up'
    assert(cursor.y != console_buffer.read_index);

    cursor.y -= 1;
    if(cursor.y < 0) {
      cursor.y += console_buffer.dim.height;
    }
  }

  console_buffer.cursor = cursor;
}

static void
move_cursor_right()
{
  v2 cursor = console_buffer.cursor;
  cursor.x += 1;

  if(cursor.x == console_buffer.dim.width) {
    cursor.y = circular_add_index(cursor.y, console_buffer.dim.height);
    cursor.x = 0;

    if(cursor.y == console_buffer.read_index) {
      scroll_down();
    }
  }

  console_buffer.cursor = cursor;
}

static void
move_end_right()
{
  v2 end = console_buffer.end;
  end.x += 1;
  char *line = get_line(end.y);
  line[end.x] = 0;

  if(end.x == console_buffer.dim.width) {
    // Don't have to modify `read_index`
    // `move_cursor_right` will handle that
    end.y = (end.y + 1) % console_buffer.dim.height;
    end.x = 0;
  }

  console_buffer.end = end;
}

static void
put_newline(void)
{
  v2 cursor = console_buffer.cursor;
  cursor.y = (cursor.y + 1) % console_buffer.dim.height;
  cursor.x = 0;

  if(cursor.y == console_buffer.read_index) {
    scroll_down();
  }

  console_buffer.cursor = cursor;
  console_buffer.end = cursor;
}

static void
put_char(char c)
{
  v2 cursor = console_buffer.cursor;
  v2 end = console_buffer.end;

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

  console_buffer.dim = (v2){ .width = ncols, .height = nrows };
  console_buffer.size = (v2){
    .width = font_width * console_buffer.dim.width,
    .height = font_height * console_buffer.dim.height,
  };

  console_buffer.padding
    = (v2){ .x = (screen_width - console_buffer.size.width) / 2,
        .y = (screen_height - console_buffer.size.height) / 2 };

  console_buffer.content = malloc(nrows * (ncols + 1));

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
clear_console(void)
{
  console_buffer.cursor = (v2){ .x = 0, .y = 0 };
  console_buffer.end = (v2){ .x = 0, .y = 0 };
  console_buffer.read_index = 0;
  char *line = get_line(0);
  line[0] = 0;
}

int
console_printf(const char *format, ...)
{
  int result = 0;

  // NOTE: Assume we never get overflow
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
      console_buffer.cursor.x = 0;
      break;

    case '\f':
      clear_console();
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
