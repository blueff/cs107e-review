#include "fb.h"

typedef unsigned int color_t;

#define WHITE 0xffffffff
#define BLACK 0x0
#define RED 0xffff0000
#define YELLOW 0xffffff00

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const int DEPTH = 4;

void
draw_pixel(int x, int y, color_t c) {
  unsigned int per_row =
    fb_get_pitch() /
    DEPTH;  // length of each row in pixels (include any padding)
  unsigned int(*im)[per_row] = fb_get_draw_buffer();
  im[y][x] = c;
}

void
draw_hline(int y, int x0, int x1, color_t c) {
  for(int x = x0; x < x1; x++) draw_pixel(x, y, c);
}

void
draw_vline(int x, int y0, int y1, color_t c) {
  for(int y = y0; y < y1; y++) draw_pixel(x, y, c);
}

// screen resoluton is 1920x1080
void
draw_grid_with_color(int width, int height, color_t hcolor, color_t vcolor) {
  if(width > SCREEN_WIDTH || height > SCREEN_HEIGHT) {
    return;
  }

  int x_padding = (SCREEN_WIDTH - width) / 2;
  int y_padding = (SCREEN_HEIGHT - height) / 2;

  for(int y = 0; y < height; y += 16)
    draw_hline(y + y_padding, x_padding, SCREEN_WIDTH - x_padding, hcolor);

  for(int x = 0; x < width; x += 16)
    draw_vline(x + x_padding, y_padding, SCREEN_HEIGHT - y_padding, vcolor);
}

void
draw_grid(int width, int height) {
  draw_grid_with_color(width, height, WHITE, WHITE);
}

void
draw_chessboard(int size, color_t white_color, color_t black_color) {
  int width = size * 8;
  int height = width;

  if(width > SCREEN_WIDTH || height > SCREEN_HEIGHT) {
    return;
  }

  int x_padding = (SCREEN_WIDTH - width) / 2;
  int y_padding = (SCREEN_HEIGHT - height) / 2;

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      int r_x = (x / size) % 2;
      int r_y = (y / size) % 2;
      color_t c = r_x == 0 ? (r_y == 0 ? white_color : black_color)
                           : (r_y == 0 ? black_color : white_color);
      draw_pixel(x_padding + x, y_padding + y, c);
    }
  }
}

void
main(void) {
  fb_init(SCREEN_WIDTH, SCREEN_HEIGHT, DEPTH, FB_SINGLEBUFFER);

  // need +1 to see the last grid line
  // draw_grid(640 + 1, 512 + 1);

  // draw_grid(1024 + 1, 768 + 1);

  // draw_grid_with_color(1024 + 1, 768 + 1, RED, YELLOW);

  draw_chessboard(60, WHITE, BLACK);
}
