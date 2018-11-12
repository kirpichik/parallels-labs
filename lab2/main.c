
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

#ifndef _TYPE_SIZE
#error _TYPE_SIZE macros required
#endif

#if _TYPE_SIZE == 1
typedef uint8_t current_type_t;
#elif _TYPE_SIZE == 2
typedef uint16_t current_type_t;
#elif _TYPE_SIZE == 4
typedef uint32_t current_type_t;
#elif _TYPE_SIZE == 8
typedef uint64_t current_type_t;
#elif _TYPE_SIZE == 16
typedef __m128 current_type_t;
#elif _TYPE_SIZE == 32
typedef __m256 current_type_t;
#else
#error Unknown _TYPE_SIZE macros value
#endif

#define MBYTES 2048
#define BYTES_COUNT (1024L * 1024 * MBYTES)

static inline uint64_t __rdtscp(uint32_t* aux) {
  uint64_t rax, rdx;
  asm volatile ("rdtsc\n" : "=a" (rax), "=d" (rdx) : :);
  return (rdx << 32) + rax;
}

int main(int argc, char* argv[]) {
  unsigned int arg;

  if (argc != 2) {
    fprintf(stderr, "Usage: <clock-speed>\n");
    return -1;
  }

  uint64_t speed = atol(argv[1]);

  size_t size = BYTES_COUNT - (BYTES_COUNT % sizeof(current_type_t));
  size_t elems_count = BYTES_COUNT / sizeof(current_type_t);
  printf("Allocating %lu elements on %lu bytes...\n", elems_count, size);
  current_type_t* src = (current_type_t*) malloc(size);
  current_type_t* dst = (current_type_t*) malloc(size);

  printf("Start copying...\n");
  uint64_t start = __rdtscp(&arg);

  for (size_t i = 0; i < elems_count; i++)
    dst[i] = src[i];

  uint64_t end = __rdtscp(&arg);
  printf("Copy finished in: %f\n", ((double) end - start) / speed);

  printf("Start memcpy test...\n");
  start = __rdtscp(&arg);

  memcpy(dst, src, size);

  end = __rdtscp(&arg);
  printf("Memcpy finished in: %f\n", ((double) end - start) / speed);

  fprintf(stderr, "Array depend: %lu, %lu\n", (size_t) (src + size), (size_t) (dst + size));

  free(src);
  free(dst);
  return 0;
}

