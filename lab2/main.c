
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

#define MBYTES 4096
#define BYTES_COUNT (1024L * 1024 * MBYTES)
#define REPEATS_COUNT 5

static inline uint64_t __rdtscp(uint32_t* aux) {
  uint64_t rax, rdx;
  asm volatile ("rdtsc\n" : "=a" (rax), "=d" (rdx) : :);
  return (rdx << 32) + rax;
}

int main(int argc, char* argv[]) {
  unsigned int arg;
  uint64_t start, end;
  double time, speed, all_time = 0, all_speed = 0;
  double best_speed = 0, best_time;

  if (argc != 2) {
    fprintf(stderr, "Usage: <clock-speed>\n");
    return -1;
  }

  uint64_t clock = atol(argv[1]);

  size_t size = BYTES_COUNT - (BYTES_COUNT % sizeof(current_type_t));
  size_t elems_count = BYTES_COUNT / sizeof(current_type_t);
  printf("Allocating %lu %lu-byte(s) elements on %lu bytes...\n",
      elems_count, sizeof(current_type_t), size);
  current_type_t* src = (current_type_t*) malloc(size);
  current_type_t* dst = (current_type_t*) malloc(size);

  fprintf(stderr, "[skip/%d] Warming up memory...\n", REPEATS_COUNT);
  for (size_t i = 0; i < elems_count; i++)
      dst[i] = src[i];
  fprintf(stderr, "[skip/%d] Warming finished.\n", REPEATS_COUNT);

  for (int repeat = 0; repeat < REPEATS_COUNT; repeat++) {
    fprintf(stderr, "[%d/%d] Start copying...\n", repeat, REPEATS_COUNT);
    start = __rdtscp(&arg);

    for (size_t i = 0; i < elems_count; i++)
      dst[i] = src[i];

    end = __rdtscp(&arg);
    time = ((double) end - start) / clock;
    speed = size / (time * 1024 * 1024);
    all_time += time;
    all_speed += speed;

    if (best_speed < speed) {
      best_time = time;
      best_speed = speed;
    }

    printf("[%d/%d] Copy finished in: %f sec, Speed: %f MB/sec\n", repeat,
        REPEATS_COUNT, time, speed);
  }

  printf("Average time: %f sec, speed: %f MB/sec\n",
      all_time / REPEATS_COUNT, all_speed / REPEATS_COUNT);
  printf("Best time: %f sec, speed: %f MB/sec\n", best_time, best_speed);

  fprintf(stderr, "Start memcpy test...\n");
  start = __rdtscp(&arg);

  memcpy(dst, src, size);

  end = __rdtscp(&arg);
  time = ((double) end - start) / clock;
  speed = size / (time * 1024 * 1024);
  printf("Memcpy finished in: %f sec, Speed: %f MB/sec\n", time, speed);

  fprintf(stderr, "Array depend: %lu, %lu\n",
      (size_t) (src + size), (size_t) (dst + size));

  free(src);
  free(dst);
  return 0;
}

