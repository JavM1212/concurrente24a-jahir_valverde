// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief   Este procedimiento imprime un mensaje en la salida estandar. Es
 *          ejecutado por un hilo secundario.
 */
void* greet(void* data);

/**
 * @brief   Este procedimiento imprime un mensaje en la salida estandar. Es
 *          ejecutado por un hilo principal.
 */
// procedure main
int main(void);