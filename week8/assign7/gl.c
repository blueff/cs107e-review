#include <gl.h>
#include <font.h>
#include <malloc.h>
#include <math.h>

#define Minimum(a, b) ((a) < (b) ? (a) : (b))
#define Minimum3(a, b, c) ((a) < (b) ? Minimum(a, c) : Minimum(b, c))
#define Maximum(a, b) ((a) > (b) ? (a) : (b))
#define Maximum3(a, b, c) ((a) > (b) ? Maximum(a, c) : Maximum(b, c))
#define Abs(a) ((a) < 0 ? -(a) : (a))

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
  int max_x = Minimum(start_x + w, width);
  int max_y = Minimum(start_y + h, height);

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

  int max_x = Minimum(start_x + font_width, buffer_width);
  int max_y = Minimum(start_y + font_height, buffer_height);

  if(max_x <= 0 || max_y <= 0)
    return;

  for(int y = Maximum(start_y, 0); y < max_y; y++) {
    for(int x = Maximum(start_x, 0); x < max_x; x++) {
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

float
rect_sdf(float px, float py, float ax, float ay, float bx, float by, float r)
{
  float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
  float lengthSq = bax * bax + bay * bay;
  float dot = pax * bax + pay * bay;

  if(dot < 0 || dot > lengthSq) {
    return 0.5;
  }

  return ((pax * pax + pay * pay) - (dot * dot) / lengthSq) - r * r;
}

void
alphablend(int x, int y, float alpha, color_t c)
{
  int pitch = fb_get_pitch();
  unsigned char *p = (unsigned char *)fb_get_draw_buffer() + pitch * y + x * 4;

  unsigned char r = (c >> 16) & 0xff;
  unsigned char g = (c >> 8) & 0xff;
  unsigned char b = (c >> 0) & 0xff;

  // B
  p[0] = (unsigned char)((float)p[0] * (1 - alpha) + (float)b * alpha);
  // G
  p[1] = (unsigned char)((float)p[1] * (1 - alpha) + (float)g * alpha);
  // R
  p[2] = (unsigned char)((float)p[2] * (1 - alpha) + (float)r * alpha);
}

// Outside pixels are clipped
void
gl_draw_line_with_width(int x1,
  int y1,
  int x2,
  int y2,
  float line_width,
  color_t c)
{
  int width = gl_get_width();
  int height = gl_get_height();

  int start_x = Maximum(Minimum(x1, x2) - line_width / 2, 0);
  int end_x = Minimum(Maximum(x1, x2) + line_width / 2, width - 1);
  int start_y = Maximum(Minimum(y1, y2) - line_width / 2, 0);
  int end_y = Minimum(Maximum(y1, y2) + line_width / 2, height - 1);
  for(int y = start_y; y <= end_y; y++)
    for(int x = start_x; x <= end_x; x++) {
      float sdf = rect_sdf(x, y, x1, y1, x2, y2, line_width / 2);
      alphablend(x, y, Maximum(Minimum(0.5f - sdf, 1.0f), 0.0f), c);
    }
}

void
gl_draw_line(int x1, int y1, int x2, int y2, color_t c)
{
  gl_draw_line_with_width(x1, y1, x2, y2, 1.0f, c);
}

float
triangle_area(int x1, int y1, int x2, int y2, int x3, int y3)
{
  float result = (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f;

  return Abs(result);
}

int
inside_triangle(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3)
{
  float a = triangle_area(x1, y1, x2, y2, x3, y3);
  float a1 = triangle_area(x, y, x2, y2, x3, y3);
  float a2 = triangle_area(x1, y1, x, y, x3, y3);
  float a3 = triangle_area(x1, y1, x2, y2, x, y);

  return Abs(a1 + a2 + a3 - a) < 0.0001;
}

void
gl_draw_triangle_with_width(int x1,
  int y1,
  int x2,
  int y2,
  int x3,
  int y3,
  float line_width,
  color_t c)
{
  int width = gl_get_width();
  int height = gl_get_height();
  gl_draw_line_with_width(x1, y1, x2, y2, line_width, c);
  gl_draw_line_with_width(x1, y1, x3, y3, line_width, c);
  gl_draw_line_with_width(x2, y2, x3, y3, line_width, c);

  int start_x = Maximum(Minimum3(x1, x2, x3), 0);
  int end_x = Minimum(Maximum3(x1, x2, x3), width - 1);
  int start_y = Maximum(Minimum3(y1, y2, y3), 0);
  int end_y = Minimum(Maximum3(y1, y2, y3), height - 1);
  for(int y = start_y; y <= end_y; y++)
    for(int x = start_x; x <= end_x; x++) {
      if(inside_triangle(x, y, x1, y1, x2, y2, x3, y3)) {
        gl_draw_pixel(x, y, c);
      }
    }
}

void
gl_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t c)
{
  gl_draw_triangle_with_width(x1, y1, x2, y2, x3, y3, 1.0f, c);
}
