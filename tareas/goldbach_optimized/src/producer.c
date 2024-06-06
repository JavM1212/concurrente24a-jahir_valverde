// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdio.h>

#include "libraries_and_structs.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  input_t* input = (input_t*)data;

  int64_t my_unit = 0;
  while (true) {
    my_unit = 0;
    pthread_mutex_lock(&input->can_access_next_unit);
      if (input->next_unit < input->size) {
        my_unit = input->input_arr[input->next_unit];
        ++input->next_unit;
      } else {
        pthread_mutex_unlock(&input->can_access_next_unit);
        break;
      }
    pthread_mutex_unlock(&input->can_access_next_unit);

    queue_enqueue(&input->queue, my_unit);

    // hacer el signal para avisar al consumidor
    sem_post(&input->can_consume);
  }

  return NULL;
}
