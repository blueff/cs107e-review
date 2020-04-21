#include "../backtrace.h"
#include "../assert.h"
#include "../printf.h"
#include "../malloc.h"
#include <uart.h>
#include <strings.h>

void main(void);
void _cstart(void);
int backtrace_1(int);
int backtrace_2(int);
void test_backtrace_complex(void);

void
print_title(char *title) {
  printf("====== %s ======\n\n", title);
}

int
backtrace_3(int a) {
  int b = a * 7 - 5;

  if(b > 3) {
    b -= 2;
  }

  frame_t frames[10];
  int result = backtrace(frames, 10);

  assert(result == 6);

  assert(strcmp(frames[0].name, "backtrace_3") == 0);
  assert(frames[0].resume_addr ==
         frames[0].resume_offset + (uintptr_t)backtrace_3);

  assert(strcmp(frames[1].name, "backtrace_2") == 0);
  assert(frames[1].resume_addr ==
         frames[1].resume_offset + (uintptr_t)backtrace_2);

  assert(strcmp(frames[2].name, "backtrace_1") == 0);
  assert(frames[2].resume_addr ==
         frames[2].resume_offset + (uintptr_t)backtrace_1);

  assert(strcmp(frames[3].name, "test_backtrace_complex") == 0);
  assert(frames[3].resume_addr ==
         frames[3].resume_offset + (uintptr_t)test_backtrace_complex);

  assert(strcmp(frames[4].name, "main") == 0);
  assert(frames[4].resume_addr == frames[4].resume_offset + (uintptr_t)main);

  assert(strcmp(frames[5].name, "_cstart") == 0);
  assert(frames[5].resume_addr == frames[5].resume_offset + (uintptr_t)_cstart);

  print_title("Test Backtrace Complex");
  print_frames(frames, result);
  printf("\n");

  return b;
}

int
backtrace_2(int a) {
  int b = a * 100;
  if(b > 20) {
    b -= 20;
  }

  return backtrace_3(b);
}

int
backtrace_1(int a) {
  int b = a * 3 + 1;
  if(b > a + 100) {
    b -= 100;
  }

  return backtrace_2(b);
}

void
test_backtrace_complex(void) {
  backtrace_1(100);
}

void
test_name_of(void) {
  assert(strcmp(name_of((uintptr_t)test_name_of), "test_name_of") == 0);
  assert(strcmp(name_of((uintptr_t)main), "main") == 0);
  assert(strcmp(name_of((uintptr_t)0x8000), "???") == 0);
}

void
test_backtrace_simple(void) {
  frame_t frames[4];

  int result = backtrace(frames, 4);

  assert(result == 3);

  assert(strcmp(frames[0].name, "test_backtrace_simple") == 0);
  assert(frames[0].resume_addr ==
         frames[0].resume_offset + (uintptr_t)test_backtrace_simple);

  assert(strcmp(frames[1].name, "main") == 0);
  assert(frames[1].resume_addr == frames[1].resume_offset + (uintptr_t)main);

  assert(strcmp(frames[2].name, "_cstart") == 0);
  assert(frames[2].resume_addr == frames[2].resume_offset + (uintptr_t)_cstart);

  print_title("Test Backtrace Simple");
  print_frames(frames, result);
  printf("\n");
}

void
test_heap_dump(void) {
  print_title("Test Heap Dump");

  heap_dump("Empty Heap");

  void *p1 = malloc(1);
  heap_dump("After p1=malloc(1)");

  void *p2 = malloc(10);
  heap_dump("After p2=malloc(10)");

  free(p1);
  heap_dump("After free(p1)");

  free(p2);
  heap_dump("After free(p2)");
}

void
test_malloc_recycle(void) {
  print_title("Test Malloc Recycle");

  void *p1 = malloc(20);
  heap_dump("After p1=malloc(20)");

  free(p1);
  heap_dump("After free(p1)");

  void *p2 = malloc(10);
  heap_dump("After p2=malloc(10)");

  free(p2);
}

void
test_free_adjacent(void) {
  print_title("Test Free Adjacent");

  void *p1 = malloc(10);
  heap_dump("After p1=malloc(10)");

  void *p2 = malloc(10);
  heap_dump("After p2=malloc(10)");

  free(p2);
  heap_dump("After free(p2)");

  free(p1);
  heap_dump("After free(p1)");
}

void
test_realloc_1(void) {
  print_title("Test Realloc 1");
  printf("realloc with a smaller size but not enough for a new block\n\n");

  void *p1 = malloc(20);
  heap_dump("After p1=malloc(20)");

  realloc(p1, 10);
  heap_dump("After realloc(p1, 10)");
}

void
test_realloc_2(void) {
  print_title("Test Realloc 2");
  printf("realloc with a smaller size enough for a new block\n\n");

  void *p1 = malloc(100);
  heap_dump("After p1=malloc(100)");

  realloc(p1, 10);
  heap_dump("After realloc(p1, 10)");
}

void
test_realloc_3(void) {
  print_title("Test Realloc 3");
  printf("realloc with a bigger size using adjacent free block\n\n");

  void *p1 = malloc(20);
  void *p2 = malloc(20);
  heap_dump("After p1=malloc(20), p2=malloc(20)");

  free(p2);
  heap_dump("After free(p2)");

  realloc(p1, 30);
  heap_dump("After relocation(p1, 30)");
}

void
test_realloc_4(void) {
  print_title("Test Realloc 4");
  printf("realloc with a bigger size which requires new malloc\n\n");

  void *p1 = malloc(20);
  heap_dump("After p1=malloc(20)");

  realloc(p1, 40);
  heap_dump("After realloc(p1, 40)");
}

void
test_mini_valgrind(void) {
  void *p1 = malloc(10);

  // intentionally over write
  uint8_t *p = (uint8_t *)p1;
  for(int i = 0; i < 20; i++) {
    *p++ = 1;
  }

  free(p1);
}

void
test_memory_report_1(void) {
  void *p1 = malloc(10);
  void *p2 = malloc(20);
  void *p3 = malloc(30);
  free(p3);

  memory_report();
}
void
test_memory_report_2(void) {
  void *p1 = malloc(40);
  void *p2 = malloc(50);
  void *p3 = malloc(60);
  free(p3);
  free(p2);

  memory_report();
}
void
test_memory_report_3(void) {
  void *p1 = malloc(70);
  void *p2 = malloc(80);
  void *p3 = malloc(90);
  free(p3);
  free(p2);
  free(p1);

  memory_report();
}

void
main(void) {
  // test_name_of();

  // test_backtrace_simple();

  // test_backtrace_complex();

  // test_heap_dump();
  // _reset_heap();

  // test_malloc_recycle();
  // _reset_heap();

  // test_free_adjacent();
  // _reset_heap();

  // test_realloc_1();
  // _reset_heap();

  // test_realloc_2();
  // _reset_heap();

  // test_realloc_3();
  // _reset_heap();

  // test_realloc_4();
  // _reset_heap();

  // test_mini_valgrind();
  // _reset_heap();

  test_memory_report_1();
  _reset_heap();

  test_memory_report_2();
  _reset_heap();

  test_memory_report_3();
  _reset_heap();

#if !GDB_DEBUG
  uart_putchar(EOT);
#endif
}
