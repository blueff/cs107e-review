#include "gprof.h"
#include <malloc.h>
#include <armtimer.h>
#include <interrupts.h>
#include <stdbool.h>
#include <assert.h>
#include <printf.h>
#include <backtrace.h>

#define Minimum(a, b) ((a) < (b) ? (a) : (b))

extern int __text_end__;

static unsigned int *counters = 0;
static int counter_count = 0;
static bool is_active = false;

unsigned int timer_interval = 1000;

typedef struct {
  unsigned int addr;
  unsigned int count;
} record_t;

// `addr` is the address of some instruction
// of a function
// return addr's offset relative to the first instruction
static int
get_offset(uintptr_t addr)
{
  uint32_t *word = (uint32_t *)addr;
  while((*word >> 24) != 0xff) {
    word--;
  }
  return addr - (uintptr_t)word - 4;
}

static void
qsort(record_t a[], int lo, int hi)
{
  int h, l;
  record_t p, t;

  if(lo < hi) {
    l = lo;
    h = hi;
    p = a[hi];

    do {
      while((l < h) && (a[l].count <= p.count))
        l = l + 1;
      while((h > l) && (a[h].count >= p.count))
        h = h - 1;
      if(l < h) {
        t = a[l];
        a[l] = a[h];
        a[h] = t;
      }
    } while(l < h);

    a[hi] = a[l];
    a[l] = p;

    qsort(a, lo, l - 1);
    qsort(a, l + 1, hi);
  }
}

// `pc` is the address of the instruction which is about to execute
static bool
gprof_handler(unsigned int pc)
{
  if(armtimer_check_and_clear_interrupt()) {
    assert(pc >= 0x8000 && pc < (unsigned int)&__text_end__);
    int offset = (pc - 4 - 0x8000) / 4;
    counters[offset]++;
    return true;
  }

  return false;
}

/* Initialize profiler */
void
gprof_init(void)
{
  counter_count = ((unsigned int)(&__text_end__) - 0x8000) / 4;
  counters = malloc(counter_count * sizeof(unsigned int));
  armtimer_init(timer_interval);
  armtimer_enable_interrupts();
  interrupts_attach_handler(gprof_handler, INTERRUPTS_BASIC_ARM_TIMER_IRQ);
}

/* Turn on profiling */
void
gprof_on(void)
{
  if(is_active)
    return;

  for(int i = 0; i < counter_count; i++) {
    counters[i] = 0;
  }

  armtimer_enable();

  is_active = true;
}

/* Turn off profiling */
void
gprof_off(void)
{
  if(!is_active)
    return;

  armtimer_disable();
  is_active = false;
}

/* Report whether profiling on */
bool
gprof_is_active(void)
{
  return is_active;
}

/* Print the profiler results */
void
gprof_dump(formatted_fn_t print)
{
  record_t *records = malloc(counter_count * sizeof(record_t));

  int record_count = 0;

  for(int i = 0; i < counter_count; i++) {
    if(counters[i] > 0) {
      records[record_count].addr = 0x8000 + i * 4;
      records[record_count].count = counters[i];
      record_count++;
    }
  }

  qsort(records, 0, record_count - 1);

  for(int i = 0; i < Minimum(10, record_count); i++) {
    int j = record_count - 1 - i;
    int offset = get_offset(records[j].addr);
    const char *name = name_of(records[j].addr - offset);
    print("%p %s+%d: %d\n",
      (void *)records[j].addr,
      name,
      offset,
      records[j].count);
  }

  free(records);
}
