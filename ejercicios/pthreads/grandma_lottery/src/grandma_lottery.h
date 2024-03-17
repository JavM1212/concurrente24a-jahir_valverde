/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#ifndef GRANDMA_LOTTERY_H_
#define GRANDMA_LOTTERY_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Este procedimiento es ejcutada por el hilo abuela. Se encarga de 
 *         encomendar a los hilos nietos, la tarea de ejecutar la funcion
 *         gen_num y recibir los valores de retorno
 */
int main(void);

/**
 * @brief Este procedimiento es ejecutado por alguno de los hilos nietos.
 *        Genera un numero aleatorio con la funcion rand_r
 * 
 *        v1 representa la variante 1 del ejercicio
 *        1. Retornan la dirección de memoria de la variable que tiene el 
 *        número comprado.
 * 
 *        La variante 1 es la correcta, me doy cuenta porque al ejecutar las 
 *        diferentes herramientas (asan, msan, tsan, ubsan), no recibo mensajes
 *        de error. Ademas no da segmentation fault al liberar memoria
 * 
 *        La variante 1 es la que esta por defecto en la entrega
 */
void* gen_num_v1(void* data);

/**
 * @brief Este procedimiento es ejecutado por alguno de los hilos nietos.
 *        Genera un numero aleatorio con la funcion rand_r
 * 
 *        v2 representa la variante 2 del ejercicio
 *        2. Retornan el número comprado como una falsa dirección de memoria.
 * 
 *        La variante 2 es la incorrecta, me doy cuenta porque al usar
 *        make asan y correr el ejecutable con esta variante. Los valores
 *        se imprimen correctamente, sin embargo se da una fuga de memoria
 *        Tambien make msan me indica que hay errores en memoria con esta
 *        variante. Ademas make tsan, al ser corrido me indica que hay
 *        problemas con threads. Y por ultimo, make ubsan tambien me 
 *        indica que hay errores de comportamiento indefinido en el codigo
 * 
 *        La variante 2 se puede visualizar descomentando lo etiquetado con
 *        @var2 y comentando lo que esta etiquetado con @var1
 */
void* gen_num_v2(void* data);

#endif  /// GRANDMA_LOTTERY_H_
