#include "fb.h"
#include <mailbox.h>

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
static fb_mode_t fb_mode;

void
fb_init(unsigned int width,
  unsigned int height,
  unsigned int depth_in_bytes,
  fb_mode_t mode)
{
  fb.width = width;
  fb.height = height;
  fb_mode = mode;

  switch(mode) {
    case FB_SINGLEBUFFER: {
      fb.virtual_height = height;
      fb.virtual_width = width;
    } break;

    case FB_DOUBLEBUFFER: {
      fb.virtual_height = height * 2;
      fb.virtual_width = width;
    } break;

    case FB_FORCONSOLE: {
      fb.virtual_height = height * 2;
      fb.virtual_width = width;
    } break;
  }

  fb.bit_depth = depth_in_bytes * 8;

  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);
  mailbox_read(MAILBOX_FRAMEBUFFER);

  // TODO: what should we do if err returned by mailbox_read != 0?
}

unsigned int
fb_get_width(void)
{
  return fb.width;
}

unsigned int
fb_get_height(void)
{
  return fb.height;
}

unsigned int
fb_get_depth(void)
{
  return fb.bit_depth / 8;
}

unsigned int
fb_get_pitch(void)
{
  return fb.pitch;
}

void *
fb_get_draw_buffer(void)
{
  switch(fb_mode) {
    case FB_SINGLEBUFFER: {
      return fb.framebuffer;
    } break;

    case FB_DOUBLEBUFFER: {
      unsigned int length = fb.height * fb.pitch;
      return fb.y_offset == 0 ? (char *)fb.framebuffer + length
                              : fb.framebuffer;
    } break;

    case FB_FORCONSOLE: {
      return (char *)fb.framebuffer + fb.y_offset * fb.pitch;
    } break;
  }

  return 0;
}

void
fb_swap_buffer(void)
{
  fb.y_offset = fb.y_offset == 0 ? fb.height : 0;
  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);
  mailbox_read(MAILBOX_FRAMEBUFFER);
}

void
fb_set_y_offset(int y_offset)
{
  fb.y_offset = y_offset;
  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb);
  mailbox_read(MAILBOX_FRAMEBUFFER);
}
