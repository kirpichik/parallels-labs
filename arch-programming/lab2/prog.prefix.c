#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>
#include <time.h>

#define ARRAY_SIZE (12 * 1024 * 1024 / sizeof(int))

int arr[ARRAY_SIZE];

void arr_random_init() {
    /*int x = 0, ind;
    for (int i = 0; i < ARRAY_SIZE; ++i)
        arr[i] = ARRAY_SIZE;

    for (int k = 0; k < ARRAY_SIZE - 1; ++k) {
        while (arr[ind = rand() % ARRAY_SIZE] != ARRAY_SIZE || ind == x);
        arr[x] = ind;
        x = ind;
    }

    arr[x] = 0;*/

    size_t pos = 0, prev = 0;
    memset(arr, -1, sizeof(arr));

    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        pos = rand() % ARRAY_SIZE;
        while (arr[pos] != -1) {
            if (pos++ >= ARRAY_SIZE)
                pos = 0;
        }

        arr[pos] = prev;
        prev = pos;
    }

    arr[0] = prev;
}

int main(int argc, const char *argv[]) {
    unsigned int iter_num = atoi(argv[1]);
    uint64_t start;
    uint64_t end;
    int k = 0;

    srand(time(NULL));
    fprintf(stderr, "Init array...\n");
    arr_random_init();
    fprintf(stderr, "Computing...\n");

    int* trash = (int *) calloc(sizeof(int), ARRAY_SIZE);
    for (size_t i = 0; i < ARRAY_SIZE; ++i)
        trash[i] = i;

    for (int i = 0; i < 400; i++)
        __asm__("nop");

    start = __rdtsc();
    for (size_t i = 0; i < iter_num; i++) {
        k = arr[k];
