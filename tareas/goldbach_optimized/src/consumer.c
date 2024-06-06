/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "libraries_and_structs.h"
#include "consumer.h"
#include "queue.h"
#include "goldbach_handler.h"

void* consume(void* data) {
  input_t* input = (input_t*)data;

  while (true) {
    // Cerrar candado para acceder a shared mem
    pthread_mutex_lock(&input->can_access_consumed_count);
    if (input->consumed_count < input->size) {
      ++input->consumed_count;
      // Abrir candado para continuar
      pthread_mutex_unlock(&input->can_access_consumed_count);
    } else {
      // Abrir candado para salirse
      pthread_mutex_unlock(&input->can_access_consumed_count);
      break;
    }

    // Esperar a que alguien meta a la queue de consumo
    sem_wait(&input->can_consume);

    int64_t value = 0;
    queue_dequeue(&input->queue, &value);

    // Calcular el Goldbach Num
    goldbach_number_t* my_gold_number = calc_goldbach_number(value);

    // Cerrar candado para acceder  shares mem
    pthread_mutex_lock(&input->can_add_goldbach_nunmber);

    // Increase allocation size by one more element
    goldbach_number_t** temp = realloc(input->goldbach_numbers
      , (input->goldbach_numbers_index + 1) * sizeof(goldbach_number_t*));
    if (temp == NULL) {
      // Abrir candado para salir
      pthread_mutex_unlock(&input->can_add_goldbach_nunmber);
      exit(EXIT_FAILURE);
    }

    // Asignar goldbach numbers
    input->goldbach_numbers = temp;
    input->goldbach_numbers[input->goldbach_numbers_index] = my_gold_number;
    input->goldbach_numbers_index++;

    pthread_mutex_unlock(&input->can_add_goldbach_nunmber);
  }

  return NULL;
}
