#include "malloc.h"
#include "assert.h"
#include "printf.h"
#include <strings.h>

#define roundup(x, n) (((x) + ((n)-1)) & (~((n)-1)))
#define MALLOC_HEADER_SIZE 8

extern int __bss_end__;
char *heap_start = (char *)&__bss_end__;
char *heap_end = (char *)&__bss_end__;

typedef struct malloc_header {
  size_t size;
  int status;  // 0 if free, 1 if in use
} malloc_header;

void
_reset_heap(void) {
  heap_end = heap_start;
}

static inline malloc_header *
next_block(malloc_header *block) {
  return (malloc_header *)((char *)block + MALLOC_HEADER_SIZE + block->size);
}

static inline size_t
size_between(malloc_header *end, malloc_header *start) {
  return (char *)end - (char *)start - MALLOC_HEADER_SIZE;
}

void
heap_dump(char *title) {
  malloc_header *header = (malloc_header *)heap_start;
  int i = 0;
  printf("==> %s\n", title);
  printf("heap start: %p\n", heap_start);
  printf("heap end:   %p\n", heap_end);
  while((char *)header < heap_end) {
    printf(
      "=> # %3d\n"
      "  addr:         %p\n"
      "  total size:   0x%x(%d)\n"
      "  payload size: 0x%x(%d)\n"
      "  status:       %d\n",
      i,
      header,
      header->size + MALLOC_HEADER_SIZE,
      header->size + MALLOC_HEADER_SIZE,
      header->size,
      header->size,
      header->status);
    header =
      (malloc_header *)((char *)header + MALLOC_HEADER_SIZE + header->size);
    i++;
  }
  printf("\n");
}

void *
sbrk(int nbytes) {
  assert(nbytes % 8 == 0);

  char *cur_sp = NULL;
  __asm__("mov %0, sp" : "=r"(cur_sp));

  if(heap_end + nbytes > cur_sp) return NULL;

  void *prev_end = heap_end;
  heap_end += nbytes;
  return prev_end;
}

void *
malloc(size_t nbytes) {
  if(nbytes == 0) return NULL;

  assert(sizeof(malloc_header) == MALLOC_HEADER_SIZE);

  size_t payload_size = roundup(nbytes, 8);

  malloc_header *search = (malloc_header *)heap_start;
  while((char *)search < heap_end) {
    // found a block
    if(search->size >= payload_size && search->status == 0) {
      // split
      size_t remaining = search->size - payload_size;
      if(remaining > 0) {
        malloc_header *remaining_block =
          (malloc_header *)((char *)search + MALLOC_HEADER_SIZE + payload_size);
        remaining_block->status = 0;
        remaining_block->size = remaining - MALLOC_HEADER_SIZE;
      }

      search->size = payload_size;
      search->status = 1;
      return (void *)(search + 1);
    }

    search = next_block(search);
  }

  void *result = sbrk(MALLOC_HEADER_SIZE + payload_size);

  if(result == NULL) return NULL;

  malloc_header *header = (malloc_header *)result;
  header->size = payload_size;
  header->status = 1;

  return (void *)(header + 1);
}

void
free(void *ptr) {
  if(ptr == NULL) return;

  malloc_header *header = (malloc_header *)ptr - 1;
  assert(header->status == 1);
  header->status = 0;

  malloc_header *end = header;
  while(end->status == 0 && (char *)end < heap_end) {
    end = next_block(end);
  }

  header->size = (char *)end - (char *)header - MALLOC_HEADER_SIZE;
}

void *
realloc(void *ptr, size_t new_size) {
  if(ptr == NULL) {
    return malloc(new_size);
  }

  if(new_size == 0) {
    free(ptr);
    return NULL;
  }

  malloc_header *header = (malloc_header *)ptr - 1;
  assert(header->status == 1);

  size_t payload_size = roundup(new_size, 8);

  if(header->size >= payload_size) {
    malloc_header *remaining_header =
      (malloc_header *)((char *)header + MALLOC_HEADER_SIZE + payload_size);

    remaining_header->status = 1;
    remaining_header->size = header->size - payload_size - MALLOC_HEADER_SIZE;
    free(remaining_header + 1);

    header->size = payload_size;
    return ptr;
  }

  malloc_header *search = next_block(header);
  while(search->status == 0 && (char *)search < heap_end) {
    search = next_block(search);

    if(size_between(search, header) >= payload_size) break;
  }

  if(size_between(search, header) >= payload_size) {
    header->size = payload_size;

    size_t remaining_size = size_between(search, header) - payload_size;
    if(remaining_size > 0) {
      malloc_header *remaining_block =
        (malloc_header *)((char *)header + MALLOC_HEADER_SIZE + payload_size);
      remaining_block->status = 0;
      remaining_block->size = remaining_size - MALLOC_HEADER_SIZE;
    }

    return ptr;
  } else {
    void *newptr = malloc(payload_size);

    if(newptr) {
      memcpy(newptr, ptr, header->size);
    }

    free(ptr);

    return newptr;
  }
}
