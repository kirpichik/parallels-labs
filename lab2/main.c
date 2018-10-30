
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

#define BYTES_COUNT (1024 * 1024 * 100)

int main(int argc, char* argv[]) {
  size_t size = BYTES_COUNT - (BYTES_COUNT % sizeof(current_type_t));
  size_t elems_count = BYTES_COUNT / sizeof(current_type_t);
  printf("Allocating %lu elements on %lu bytes...\n", elems_count, size);
  current_type_t* src = (current_type_t*) malloc(size);
  current_type_t* dst = (current_type_t*) malloc(size);

  printf("Start copying...\n");
  // TODO - begin timer

  for (size_t i = 0; i < elems_count; i++)
    dst[i] = src[i];

  // TODO - end timer
  printf("Copy finished in:\n");

  printf("Start memcpy test...\n");
  // TODO - begin timer

  memcpy(dst, src, size);

  // TODO - end timer
  printf("Memcpy finished in:\n");

  fprintf(stderr, "Array depend: %lu, %lu\n", (size_t) (src + size), (size_t) (dst + size));

  free(src);
  free(dst);
  return 0;
}

