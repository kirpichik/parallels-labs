
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <x86intrin.h>
#include <stdint.h>

#define DENSE_PACK 1
#define CACHE_LINE_PACK 64
#define PAGES_PACK 4096

#define CPU_FREQ 2900000000ULL
#define LIFE_SPAWN_PERCENT 50
#define WORLD_M 64
#define WORLD_N 32
#define THREADS_COUNT 4
#define STEPS 1000000
#define BARRIER_PACK_TYPE DENSE_PACK

typedef struct thread_data {
  int* src;
  int* dest;
  size_t thread_number;
  double all_time;
  double min;
} thread_data_t;

static volatile int sync[THREADS_COUNT][BARRIER_PACK_TYPE];

static inline int get_cell(int* field, size_t x, size_t y) {
  return field[x * (WORLD_N + 2) + y];
}

static inline void set_cell(int* field, size_t x, size_t y, int value) {
  field[x * (WORLD_N + 2) + y] = value;
}

static inline size_t count_neighbours(int* field, size_t x, size_t y) {
  size_t life = get_cell(field, x - 1, y) + get_cell(field, x + 1, y);
  for (size_t i = x - 1; i <= x + 1; i++)
    life += get_cell(field, i, y - 1) + get_cell(field, i, y + 1);
  return life;
}

static inline void update_cell(int* src, int* dest, size_t x, size_t y) {
  size_t life = count_neighbours(src, x, y);
  if (get_cell(src, x, y))
    set_cell(dest, x, y, life == 2 || life == 3);
  else
    set_cell(dest, x, y, life == 3);
}

static inline void barrier_wait(size_t thread_number, size_t step) {
  size_t left = (thread_number ? thread_number : THREADS_COUNT) - 1;
  size_t right = (thread_number == THREADS_COUNT - 1 ? 0 : thread_number + 1);
  sync[thread_number][0]++;
  while (sync[left][0] < step || sync[right][0] < step);
}

static inline void swap_worlds(thread_data_t* data) {
  int* temp = data->src;
  data->src = data->dest;
  data->dest = temp;
}

static void generate_world(int* field) {
  srand(time(NULL));

  for (size_t i = 1; i <= WORLD_M; i++)
    for (size_t j = 1; j <= WORLD_N; j++)
      set_cell(field, i, j, (rand() % 100) > LIFE_SPAWN_PERCENT);
}

static void* life_thread(void* arg) {
  thread_data_t* data = (thread_data_t*)arg;
  size_t begin_x = (WORLD_M / THREADS_COUNT) * data->thread_number;
  size_t end_x = (WORLD_M / THREADS_COUNT) * (data->thread_number + 1);
  size_t begin_y = 1;
  size_t end_y = WORLD_N;
  uint64_t start, end;
  double time;

  for (size_t step = 0; step < STEPS; step++) {
    for (size_t i = begin_x + 1; i <= end_x; i++)
      for (size_t j = begin_y + 1; j <= end_y; j++)
        update_cell(data->src, data->dest, i, j);

    start = __rdtsc();
    barrier_wait(data->thread_number, step + 1);
    end = __rdtsc();

    time = ((double) end - start) / CPU_FREQ;
    data->all_time += time;
    if (data->min > time || data->min == 0)
      data->min = time;

    swap_worlds(data);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  pthread_t threads[THREADS_COUNT];
  thread_data_t data[THREADS_COUNT];
  double min = 0, average = 0;
  int error;

  int* world_src = (int*) malloc((WORLD_M + 2) * (WORLD_N + 2) * sizeof(int));
  if (!world_src) {
    perror("Cannot allocate src world");
    return -1;
  }

  int* world_dest = (int*) malloc((WORLD_M + 2) * (WORLD_N + 2) * sizeof(int));
  if (!world_dest) {
    perror("Cannot allocate dest world");
    free(world_src);
    return -1;
  }

  generate_world(world_src);

  for (size_t i = 0; i < THREADS_COUNT; i++) {
    data[i].src = world_src;
    data[i].dest = world_dest;
    data[i].thread_number = i;
    data[i].all_time = data[i].min = 0;
    error = pthread_create(threads + i, NULL, &life_thread, data + i);
    if (error) {
      fprintf(stderr, "Cannot create thread: %s\n", strerror(error));
      return error;
    }
  }

  for (size_t i = 0; i < THREADS_COUNT; i++) {
    error = pthread_join(threads[i], NULL);
    if (error)
      fprintf(stderr, "Cannot join thread: %s\n", strerror(error));
    else {
      average += data[i].all_time;
      if (min > data[i].min || min == 0)
        min = data[i].min;
    }
  }

  printf("Min: %0.8f, Average: %0.8f\n", min, average / (STEPS * THREADS_COUNT));

  free(world_dest);
  free(world_src);
  return 0;
}
