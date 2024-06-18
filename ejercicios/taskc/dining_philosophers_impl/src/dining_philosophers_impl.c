#define _DEFAULT_SOURCE

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int philosopher_number;
    int min_think_time;
    int max_think_time;
    int min_eat_time;
    int max_eat_time;
    sem_t* chopsticks;
    int chopstick_count;
} philosopher_data_t;

void* philosopher_dine(void* args) {
  philosopher_data_t* data = (philosopher_data_t*) args;
  int left_chopstick = data->philosopher_number % data->chopstick_count;
  int right_chopstick = (data->philosopher_number + 1) % data->chopstick_count;
  
  while (1) {
      // Think
    int think_time = data->min_think_time + rand() % (data->max_think_time - data->min_think_time + 1);
    printf("Philosopher %d is thinking for %d ms\n", data->philosopher_number, think_time);
    usleep(think_time * 1000);
    
    // Pick up chopsticks
    if (data->philosopher_number % 2 == 0) {
      sem_wait(&data->chopsticks[left_chopstick]);
      sem_wait(&data->chopsticks[right_chopstick]);
    } else {
      sem_wait(&data->chopsticks[right_chopstick]);
      sem_wait(&data->chopsticks[left_chopstick]);
    }

    // Eat
    int eat_time = data->min_eat_time + rand() % (data->max_eat_time - data->min_eat_time + 1);
    printf("Philosopher %d is eating for %d ms\n", data->philosopher_number, eat_time);
    usleep(eat_time * 1000);

    // Put down chopsticks
    sem_post(&data->chopsticks[left_chopstick]);
    sem_post(&data->chopsticks[right_chopstick]);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    fprintf(stderr, "usage: %s philosopher_count min_think_time max_think_time min_eat_time max_eat_time\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  int philosopher_count = atoi(argv[1]);
  int min_think_time = atoi(argv[2]);
  int max_think_time = atoi(argv[3]);
  int min_eat_time = atoi(argv[4]);
  int max_eat_time = atoi(argv[5]);
  int chopstick_count = philosopher_count >= 2 ? philosopher_count : 2;

  // Validate input times
  if (min_think_time > max_think_time || min_eat_time > max_eat_time || philosopher_count < 1) {
    fprintf(stderr, "Invalid input values.\n");
    return EXIT_FAILURE;
  }

  pthread_t philosophers[philosopher_count];
  sem_t chopsticks[chopstick_count];
  philosopher_data_t philosopher_data[philosopher_count];

  srand(time(NULL));

  for (int i = 0; i < chopstick_count; i++) {
    sem_init(&chopsticks[i], 0, 1);
  }

  for (int i = 0; i < philosopher_count; i++) {
    philosopher_data[i].philosopher_number = i;
    philosopher_data[i].min_think_time = min_think_time;
    philosopher_data[i].max_think_time = max_think_time;
    philosopher_data[i].min_eat_time = min_eat_time;
    philosopher_data[i].max_eat_time = max_eat_time;
    philosopher_data[i].chopsticks = chopsticks;
    philosopher_data[i].chopstick_count = chopstick_count;
    pthread_create(&philosophers[i], NULL, philosopher_dine, &philosopher_data[i]);
  }

  for (int i = 0; i < philosopher_count; i++) {
    pthread_join(philosophers[i], NULL);
  }

  for (int i = 0; i < chopstick_count; i++) {
    sem_destroy(&chopsticks[i]);
  }

  return EXIT_SUCCESS;
}
