#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;
typedef struct block_header {
  size_t size;
  int is_free;
  struct block_header *next;
} block_header_t;

static block_header_t *heap_head = NULL;
static char *heap_brk = NULL;

static inline size_t align8(size_t x) {
  return (x + 7) & ~((size_t)7);
}

static block_header_t *request_block(size_t size) {
  if (heap_brk == NULL) {
    heap_brk = (char *)heap.start;
  }

  size_t total = sizeof(block_header_t) + size;
  char *new_brk = heap_brk + total;
  if (new_brk > (char *)heap.end) {
    return NULL;
  }

  block_header_t *blk = (block_header_t *)heap_brk;
  blk->size = size;
  blk->is_free = 0;
  blk->next = NULL;
  heap_brk = new_brk;
  return blk;
}

static void split_block(block_header_t *blk, size_t size) {
  if (blk->size < size + sizeof(block_header_t) + 8) {
    return;
  }

  block_header_t *remain = (block_header_t *)((char *)(blk + 1) + size);
  remain->size = blk->size - size - sizeof(block_header_t);
  remain->is_free = 1;
  remain->next = blk->next;

  blk->size = size;
  blk->next = remain;
}

static void coalesce_free_blocks(void) {
  for (block_header_t *cur = heap_head; cur != NULL && cur->next != NULL; ) {
    if (cur->is_free && cur->next->is_free) {
      cur->size += sizeof(block_header_t) + cur->next->size;
      cur->next = cur->next->next;
    } else {
      cur = cur->next;
    }
  }
}

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

void *malloc(size_t size) {  
#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
  if (size == 0) {
    return NULL;
  }

  size = align8(size);

  if (heap_head == NULL) {
    block_header_t *blk = request_block(size);
    if (blk == NULL) {
      return NULL;
    }
    heap_head = blk;
    return (void *)(blk + 1);
  }

  block_header_t *cur = heap_head;
  block_header_t *last = NULL;
  while (cur != NULL) {
    if (cur->is_free && cur->size >= size) {
      split_block(cur, size);
      cur->is_free = 0;
      return (void *)(cur + 1);
    }
    last = cur;
    cur = cur->next;
  }

  block_header_t *blk = request_block(size);
  if (blk == NULL) {
    return NULL;
  }
  last->next = blk;
  return (void *)(blk + 1);
#endif
  return NULL;
}

void free(void *ptr) {
#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
  if (ptr == NULL) {
    return;
  }

  block_header_t *blk = ((block_header_t *)ptr) - 1;
  assert((char *)blk >= (char *)heap.start && (char *)blk < (char *)heap.end);
  blk->is_free = 1;
  coalesce_free_blocks();
#else
  (void)ptr;
#endif
}

#endif
