#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>
#include <time.h>

#define ROB_CLEAR_COUNT 400
#define L3_SIZE 6
#define ARRAY_SIZE (L3_SIZE * 2 * 1024 * 1024 / sizeof(int))

int arr[ARRAY_SIZE];

void arr_random_init() {
    int prev = 0;
    int pos;

    for (size_t i = 0; i < ARRAY_SIZE; i++)
        arr[i] = ARRAY_SIZE;

    for (size_t i = 0; i < ARRAY_SIZE - 1; i++) {
        while (arr[pos = rand() % ARRAY_SIZE] != ARRAY_SIZE || pos == prev);
        arr[prev] = pos;
        prev = pos;
    }

    arr[prev] = 0;
}

int main(int argc, const char *argv[]) {
    unsigned int iter_num = atoi(argv[1]);
    uint64_t start;
    uint64_t end;
    int pos = 0;

    srand(time(NULL));
    fprintf(stderr, "arr = %lu\n", sizeof(arr));
    fprintf(stderr, "Init array...\n");
    arr_random_init();
    fprintf(stderr, "Computing...\n");

    // Очищаем кэш
    size_t* cleanup = (size_t*)malloc(sizeof(size_t) * ARRAY_SIZE);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
        cleanup[i] = i;

    // Очищаем буфер переупорядочивания
    for (int i = 0; i < ROB_CLEAR_COUNT; i++)
        __asm__("nop");

    start = __rdtsc();
    for (size_t i = 0; i < iter_num; i++) {
        pos = arr[pos];
