
#include <stdio.h>
#include <stdint.h>
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

int main(int argc, char* argv[]) {
  printf("type size: %lu\n", sizeof(current_type_t));
  return 0;
}

