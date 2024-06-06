// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// TODO(all): Implement a thread-safe queue

#ifndef QUEUE_H
#define QUEUE_H

#include "libraries_and_structs.h"
#include "pthread.h"
#include "stdbool.h"

/**
 * @brief nodo de la cola
 * 
 */
typedef struct queue_node {
  int64_t data;
  struct queue_node* next;
} queue_node_t;

/**
 * @brief cola con su cabeza y tail
 * 
 */
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @todo: document all interfaces
 * @remaks init and destroy are NOT thread-safe
 */
int queue_init(queue_t* queue);

/**
 * @brief destructor de cola
 * 
 * @param queue 
 * @return int 
 */
int queue_destroy(queue_t* queue);

/**
 * @brief si es vacio devuelve true, si no devuelve false
 * 
 * @param queue 
 * @return true 
 * @return false 
 */
bool queue_is_empty(queue_t* queue);

/**
 * @brief misma que la anterior pero no es thread sage
 * 
 * @param queue 
 * @return true 
 * @return false 
 */
bool queue_is_empty_unsafe(queue_t* queue);

/**
 * @brief ingresar a la cola
 * 
 * @param queue 
 * @param data 
 * @return int 
 */
int queue_enqueue(queue_t* queue, const int64_t data);

/**
 * @brief sacar de la cola
 * 
 * @param queue 
 * @param data 
 * @return int 
 */
int queue_dequeue(queue_t* queue, int64_t* data);

/**
 * @brief limpiar la cola
 * 
 * @param queue 
 */
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
