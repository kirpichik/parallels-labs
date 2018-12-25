#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>

#define REPEATS 100000000

int main(int argc, char* argv[]) {
  uint64_t start = 0;
  uint64_t end = 0;
  volatile int a = 0;

  FILE* file = fopen("result_step_1.dat", "w");
  for (register size_t LSpy = 1; LSpy <= 100; LSpy++) {
    start = __rdtsc();
    for (register size_t i = 0; i < REPEATS; i++) {
      if ((i % LSpy) == 0)
        a = 1;
    }
    end = __rdtsc();
    printf("LSpy: %3d, Result: %.4lf\n", LSpy, (double)(end - start) / REPEATS);
    fprintf(file, "%d %.4lf\n", LSpy, (double)(end - start) / REPEATS);
  }
  fclose(file);

  return 0;
}
