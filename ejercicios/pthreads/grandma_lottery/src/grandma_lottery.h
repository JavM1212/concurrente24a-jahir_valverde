// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef CONCURRENTE24A_JAHIR_VALVERDE_EJERCICIOS_PTHREADS_GRANDMA_LOTTERY_SRC_GRANDMA_LOTTERY_H_
#define CONCURRENTE24A_JAHIR_VALVERDE_EJERCICIOS_PTHREADS_GRANDMA_LOTTERY_SRC_GRANDMA_LOTTERY_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief   Este procedimiento imprime un mensaje en la salida estandar. Es
 *          ejecutado por un hilo principal.
 */
//  procedure main
int main(void);

/**
 * @brief   Este procedimiento imprime un mensaje en la salida estandar. Es
 *          ejecutado por un hilo secundario.
 */
void* greet(void* data);

#endif  // CONCURRENTE24A_JAHIR_VALVERDE_EJERCICIOS_PTHREADS_GRANDMA_LOTTERY_SRC_GRANDMA_LOTTERY_H_
