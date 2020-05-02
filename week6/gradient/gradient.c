#include "gpio.h"
#include "mailbox.h"
#include "printf.h"
#include "uart.h"

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int virtual_width;
  unsigned int virtual_height;
  unsigned int pitch;
  unsigned int bit_depth;
  unsigned int x_offset;
  unsigned int y_offset;
  void *framebuffer;
  unsigned int total_bytes;
} fb_config_t;

static volatile fb_config_t fb __attribute__((aligned(16)));

// 1280/1024 640/512
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const int DEPTH = 32;

int
fb_init(void) {
  // write these values to the GPU
  fb.width = WIDTH;
  fb.height = HEIGHT;
  fb.virtual_width = WIDTH;
  fb.virtual_height = HEIGHT;
  fb.bit_depth = DEPTH;
  fb.x_offset = 0;
  fb.y_offset = 0;

  // read these values from the GPU - must be 0 initially
  fb.framebuffer = 0;
  fb.total_bytes = 0;
  fb.pitch = 0;

  // send the request to the GPU using "mail"
  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);

  // read the response from the GPU
  int err = mailbox_read(MAILBOX_FRAMEBUFFER);
  return err;
}

void
draw_gradient() {
  int pitch = fb.pitch / 4;
  unsigned int *buf = (unsigned int *)fb.framebuffer;

  for(int y = 0; y < fb.virtual_height; y++) {
    for(int x = 0; x < fb.virtual_width; x++) {
      unsigned int color = (y << 24) | (x << 16) | 0x00ff;
      buf[y * pitch + x] = color;
    }
  }
}

void
main(void) {
  gpio_init();
  uart_init();

  int err = fb_init();

  printf("requested physical size = (%d, %d)\n", fb.width, fb.height);
  printf("requested virtual size = (%d, %d)\n",
         fb.virtual_width,
         fb.virtual_height);
  printf("requested depth (bits) = %d\n", fb.bit_depth);

  printf("result/error (should be 0) = %x\n", err);
  printf("physical size = (%d, %d)\n", fb.width, fb.height);
  printf("virtual size = (%d, %d)\n", fb.virtual_width, fb.virtual_height);
  printf("depth (bits) = %d\n", fb.bit_depth);
  printf("framebuffer address = %p\n", fb.framebuffer);
  printf("framebuffer total bytes = %d (height*width*depth=%d)\n",
         fb.total_bytes,
         fb.virtual_height * fb.virtual_width * 4);
  printf("pitch (number of bytes in a scanline)= %d (width*depth=%d)\n",
         fb.pitch,
         fb.virtual_width * 4);

  if(err == 0) {
    draw_gradient();
  }

  uart_putchar(EOT);
}
