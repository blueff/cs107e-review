#include <printf.h>
#include "backtrace.h"

const char *
name_of(uintptr_t fn_start_addr)
{
  uint32_t word = *(uint32_t *)(fn_start_addr - 4);
  if((word >> 24) == 0xff) {
    uint32_t length = word & ~0xff000000;
    return ((char *)(fn_start_addr)) - 4 - length;
  } else {
    return "???";
  }
}

int
backtrace(frame_t f[], int max_frames)
{
  uintptr_t *cur_fp = NULL;
  __asm__("mov %0, fp" : "=r"(cur_fp));

  int result = 0;

  for(int i = 0; i < max_frames; i++) {
    uintptr_t lr = *(cur_fp - 1);
    uintptr_t *up_fp = (uintptr_t *)(*(cur_fp - 3));
    if(up_fp == NULL)
      break;

    result++;

    uintptr_t up_pc = *up_fp;

    f[i].resume_addr = lr;
    f[i].resume_offset = lr - up_pc + 12;
    f[i].name = name_of(up_pc - 12);

    cur_fp = up_fp;
  }

  return result;
}

void
print_frames(frame_t frames[], int n)
{
  for(int i = 0; i < n; i++) {
    frame_t *f = frames + i;
    printf("#%d 0x%x at %s+%d\n",
      i,
      f->resume_addr,
      f->name,
      f->resume_offset - 4);
  }
}

void
print_backtrace(void)
{
  frame_t frames[10];
  int result = backtrace(frames, 10);
  print_frames(frames, result);
}
