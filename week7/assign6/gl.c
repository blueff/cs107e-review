#include <gl.h>
#include <font.h>
#include <malloc.h>

#define MINIMUM(a, b) (a < b ? a : b)
#define MAXIMUM(a, b) (a > b ? a : b)

static void *font_buffer;
static gl_mode_t gl_mode;

void
gl_init(unsigned int width, unsigned int height, gl_mode_t mode)
{
  gl_mode = mode;
  fb_init(width, height, 4, mode);
}

unsigned int
gl_get_width(void)
{
  return fb_get_width();
}

unsigned int
gl_get_height(void)
{
  return fb_get_height();
}

color_t
gl_color(unsigned char r, unsigned char g, unsigned char b)
{
  return (0xff << 24) | (r << 16) | (g << 8) | b;
}

void
gl_swap_buffer(void)
{
  if(gl_mode == GL_DOUBLEBUFFER) {
    fb_swap_buffer();
  }
}

void
gl_draw_pixel(int x, int y, color_t c)
{
  int width = gl_get_width();
  int height = gl_get_height();
  if(x < 0 || x >= width || y < 0 || y >= height)
    return;

  int pitch = fb_get_pitch() / 4;
  unsigned int(*arr)[pitch] = fb_get_draw_buffer();
  arr[y][x] = c;
}

color_t
gl_read_pixel(int x, int y)
{
  int width = gl_get_width();
  int height = gl_get_height();
  if(x < 0 || x >= width || y < 0 || y >= height)
    return 0;

  int pitch = fb_get_pitch() / 4;

  unsigned int(*arr)[pitch] = fb_get_draw_buffer();
  return arr[y][x];
}

void
gl_draw_rect(int start_x, int start_y, int w, int h, color_t c)
{
  int height = gl_get_height();
  int width = gl_get_width();
  int max_x = MINIMUM(start_x + w, width);
  int max_y = MINIMUM(start_y + h, height);

  if(start_x < 0) {
    start_x = 0;
  }
  if(start_y < 0) {
    start_y = 0;
  }

  int pitch = fb_get_pitch() / 4;
  unsigned int(*arr)[pitch] = fb_get_draw_buffer();

  for(int y = start_y; y < max_y; y++) {
    for(int x = start_x; x < max_x; x++) {
      arr[y][x] = c;
    }
  }
}

void
gl_clear(color_t c)
{
  gl_draw_rect(0, 0, gl_get_width(), gl_get_height(), c);
}

void
gl_draw_char(int start_x, int start_y, int ch, color_t c)
{
  int font_width = font_get_width();
  int font_height = font_get_height();

  int buffer_width = gl_get_width();
  int buffer_height = gl_get_height();

  if(font_buffer == NULL) {
    font_buffer = malloc(font_width * font_height);
  }

  if(!font_get_char(ch, font_buffer, font_width * font_height))
    return;

  // Cast to a 2D array
  unsigned char(*font_buffer_arr)[font_width] = font_buffer;

  if(start_x >= buffer_width || start_y >= buffer_height)
    return;

  int max_x = MINIMUM(start_x + font_width, buffer_width);
  int max_y = MINIMUM(start_y + font_height, buffer_height);

  if(max_x <= 0 || max_y <= 0)
    return;

  for(int y = MAXIMUM(start_y, 0); y < max_y; y++) {
    for(int x = MAXIMUM(start_x, 0); x < max_x; x++) {
      int pixel = font_buffer_arr[y - start_y][x - start_x];
      if(pixel == 0xff) {
        gl_draw_pixel(x, y, c);
      }
    }
  }
}

void
gl_draw_string(int start_x, int start_y, const char *str, color_t c)
{
  int font_width = font_get_width();
  int x = start_x;
  char *p = (char *)str;
  while(*p) {
    gl_draw_char(x, start_y, *p, c);
    p++;
    x += font_width;
  }
}
