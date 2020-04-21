#include "malloc.h"
#include "assert.h"
#include "printf.h"
#include "backtrace.h"
#include <strings.h>

#define roundup(x, n) (((x) + ((n)-1)) & (~((n)-1)))
#define REDZONE 0x01020304
#define FRAME_COUNT 3

extern int __bss_end__;
char *heap_start = (char *)&__bss_end__;
char *heap_end = (char *)&__bss_end__;

static unsigned int malloc_count = 0;
static unsigned int free_count = 0;
static unsigned int allocated_bytes = 0;

typedef struct malloc_header {
  size_t size;
  int status;  // 0 if free, 1 if in use
  size_t request;
  size_t _padding;
  frame_t frames[FRAME_COUNT];
  uint32_t redzone;
} malloc_header;

#define MALLOC_HEADER_SIZE sizeof(malloc_header)

static void
print_farmes(frame_t frames[]) {
  for(int i = 0; i < FRAME_COUNT; i++) {
    frame_t *frame = frames + i;
    printf("#%d 0x%x at %s + %d\n",
           i,
           frame->resume_addr,
           frame->name,
           frame->resume_offset);
  }
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
memory_report(void) {
  printf("=============================================\n");
  printf("         Mini-Valgrind Memory Report         \n");
  printf("=============================================\n");
  printf("\n");

  printf("malloc/free: %d allocs, %d frees, %d bytes allocated\n",
         malloc_count,
         free_count,
         allocated_bytes);

  unsigned int lost_blocks = 0;
  unsigned int lost_bytes = 0;

  malloc_header *header = (malloc_header *)heap_start;
  while((char *)header < heap_end) {
    // Found a lost block!
    if(header->status == 1) {
      lost_blocks++;
      lost_bytes += header->request;
      printf("\n");
      printf("%d bytes are lost, allocated by\n", header->request);
      print_frames(header->frames, FRAME_COUNT);
    }

    header = next_block(header);
  }

  if(lost_blocks > 0) {
    printf("\n");
    printf("Lost %d total bytes in %d blocks.\n", lost_bytes, lost_blocks);
  }
}

void
_reset_heap(void) {
  heap_end = heap_start;
  malloc_count = 0;
  free_count = 0;
  allocated_bytes = 0;
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
      "  request size: 0x%x(%d)\n"
      "  status:       %d\n",
      i,
      header,
      header->size + MALLOC_HEADER_SIZE,
      header->size + MALLOC_HEADER_SIZE,
      header->size,
      header->size,
      header->request,
      header->request,
      header->status);

    header = next_block(header);

    i++;
  }
  printf("\n");
}

typedef struct redzone_value {
  uint32_t leading;
  uint32_t trailing;
} redzone_value;

static redzone_value
get_redzone_value(malloc_header *header) {
  redzone_value result = {};
  result.leading = *((uint32_t *)header + MALLOC_HEADER_SIZE / 4 - 1);
  result.trailing =
    *((uint32_t *)header + MALLOC_HEADER_SIZE / 4 + header->size / 4 - 1);

  return result;
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
  assert(MALLOC_HEADER_SIZE % 8 == 0);

  if(nbytes == 0) return NULL;

  // last four bytes is the red zone
  size_t payload_size = roundup(nbytes + 4, 8);

  int has_found = 0;
  malloc_header *target = (malloc_header *)heap_start;
  while((char *)target < heap_end) {
    // found a block
    if(target->size >= payload_size && target->status == 0) {
      // split
      size_t remaining = target->size - payload_size;
      if(remaining >= MALLOC_HEADER_SIZE) {
        malloc_header *remaining_block =
          (malloc_header *)((char *)target + MALLOC_HEADER_SIZE + payload_size);
        remaining_block->status = 0;
        remaining_block->size = remaining - MALLOC_HEADER_SIZE;

        target->size = payload_size;
      }

      has_found = 1;
      break;
    }

    target = next_block(target);
  }

  if(!has_found) {
    void *result = sbrk(MALLOC_HEADER_SIZE + payload_size);
    if(result == NULL) return NULL;
    target = (malloc_header *)result;
    target->size = payload_size;
  }

  target->request = nbytes;
  target->status = 1;
  target->redzone = REDZONE;
  backtrace(target->frames, FRAME_COUNT);

  uint32_t *trailing_redzone = (uint32_t *)(target + 1) + target->size / 4 - 1;
  *trailing_redzone = REDZONE;

  malloc_count++;
  allocated_bytes += nbytes;
  return (void *)(target + 1);
}

void
free(void *ptr) {
  if(ptr == NULL) return;

  malloc_header *header = (malloc_header *)ptr - 1;

  redzone_value red = get_redzone_value(header);
  if(red.leading != REDZONE || red.trailing != REDZONE) {
    printf("=============================================\n");
    printf("**********  Mini-Valgrind Alert  **********\n");
    printf("=============================================\n");
    printf(
      "Attempt to free address %p that has damaged red zone(s): "
      "[%8x] [%8x]\n",
      ptr,
      red.leading,
      red.trailing);
    printf("Block of size %d bytes, allocated by\n", header->request);
    print_farmes(header->frames);
    return;
  }

  assert(header->status == 1);
  free_count++;

  malloc_header *end = next_block(header);
  while(end->status == 0 && (char *)end < heap_end) {
    end = next_block(end);
  }

  header->size = (char *)end - (char *)header - MALLOC_HEADER_SIZE;
  header->status = 0;
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

  // 4 bytes is the red zone
  size_t payload_size = roundup(new_size + 4, 8);

  if(header->size >= payload_size) {
    if(header->size - payload_size >= MALLOC_HEADER_SIZE) {
      malloc_header *remaining_header =
        (malloc_header *)((char *)header + MALLOC_HEADER_SIZE + payload_size);

      remaining_header->status = 0;
      remaining_header->size = header->size - payload_size - MALLOC_HEADER_SIZE;

      header->size = payload_size;
      uint32_t *redzone = (uint32_t *)(header + 1) + header->size / 4 - 1;
      *redzone = REDZONE;
    }

    header->request = new_size;

    return ptr;
  }

  malloc_header *search = next_block(header);
  while(search->status == 0 && (char *)search < heap_end) {
    search = next_block(search);

    if(size_between(search, header) >= payload_size) break;
  }

  if(size_between(search, header) >= payload_size) {
    size_t remaining_size = size_between(search, header) - payload_size;
    // Don't have to include the red zone
    if(remaining_size >= MALLOC_HEADER_SIZE) {
      malloc_header *remaining_block =
        (malloc_header *)((char *)header + MALLOC_HEADER_SIZE + payload_size);
      remaining_block->status = 0;
      remaining_block->size = remaining_size - MALLOC_HEADER_SIZE;

      header->size = payload_size;
    } else {
      header->size = size_between(search, header);
    }

    uint32_t *redzone = (uint32_t *)(header + 1) + header->size / 4 - 1;
    *redzone = REDZONE;

    return ptr;
  } else {
    void *newptr = malloc(new_size);

    if(newptr) {
      memcpy(newptr, ptr, header->request);
    }

    free(ptr);

    return newptr;
  }
}
