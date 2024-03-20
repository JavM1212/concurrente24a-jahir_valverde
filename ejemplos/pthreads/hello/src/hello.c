/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "hello.h"

/**
 * @brief Las variables globales no se llevan con la concurrencia, pero para 
 *        terminos de este ejemplo va a ser utilizada. Ojo que esto es solo
 *        con las variables. Las constantes si pueden ser globales sin
 *        problemas
 * 
 *        Es conveniente analiar si las librerias a utilizar son thread safety
 *        o no, ya que esto asegura que no usa variables globales
 *        Esto se puede revisar con el comando: man <nombre_libreria>
 * 
 *        La memoria tiene unas secciones donde se distribuyen distintas
 *        utilidades
 *        1- Code Segment: Aqui se guarda el fuente traducido a lenguaje
 *        de maquina
 *        2- Data Segment: Se guardan valores de datos como 
 *        inicializaciones
 *        3- Stack Segment: Aqui se handlean los hilos, los cuales son "workers"
 *        que permiten ejecutar conjuntos de instrucciones de manera simultanea
 *        4- Heap Segment: 
 * 
 *        Explicacion del hilo principal: @ref sugerencia: ver a la par del 
 *                                             archivo hello.png 
 *        Un hilo es practicamente una direccion de memoria a una mesa de
 *        trabajo (stack frame)
 *        
 */
int error = 0; 

int main(void) {
  /**
   * @brief no es necesario inicializarlo porque de eso se encarga phtrad_create
   * 
   */
  pthread_t thread;   

  /**
   * @brief Cuando se invoca pthread_create, se le dice al sistema operativo que 
   *        se quiere crear un nuevo secundario. El proceso es exactamente igual
   *        a crear el hilo principal.
   * 
   *        El scheduler es el que se encarga de crear hilos y sirve con
   *        una maquina de estados que se puede ver en @ref scheduler.png
   * 
   *        Estado new:
   *        El PC (program counter), direccion de memoria al segmento de codigo
   *        donde se encuentra la proxima instruccion que se debe ejecutar
   *        El RSP (regiser stack counter), direccion de memoria al segmento
   *        de pila donde se encuentra el inicio de pila apartado para el hilo
   *        actual
   * 
   *        Luego de settear esta data, esto pasa la info a una cola que se 
   *        llama ready, donde espera a que se le asignen recursos de CPU
   *        Cuando se llega al tope de la cola, es transferido a la cola 
   *        running, la cual carga un hilo de ejecucion en una CPU. Esto
   *        transfiriendo los valores de la RAM (PC, RSP, etc) a los registros
   *        de la CPU. Ademas le indica a la CPU que continue el ciclo 
   *        de instrucciones.
   *        
   *        Cuando dos o mas hilos quieren compartir el mismo recurso (por 
   *        ejemplo el stdout), se genera una condicion de carrera
   * 
   */
  error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);

  if (error == EXIT_SUCCESS) {
    /**
     * @brief Pausa el hilo actual 2 segundo. El hecho de que al dormir 2
     *        segundos, se imprima primero el hilo secundario, demuestra que el 
     *        programa es concurrente y no serial
     * 
     */
    // sleep(2);

    /**
     * @brief Pausa el hilo actual 1 micro segundo. Este programa 
     *        presenta INDETERMINISMO. El indeterminismo es una condicion
     *        en la que no es posible predecir que va a pasar. Un ejemplo  
     *        de esto es este usleep(1). El cual le da prioridad al hilo
     *        secundario, pero igual es posible que se cumpla primero el hilo
     *        principal.
     *  
     */
    usleep(1);  

    fprintf(stdout, "Hello from main thread\n");  /// stdout
    /// "Equivalente" al destroy para threads, permite almacenar valores
    /// de retorno y evita condiciones de carrera
    pthread_join(thread, /*value_ptr*/ NULL);   
  } else {
      fprintf(stderr, "Could not create secondary thread\n");  /// stderr
  }

  return error;  /// devolver el error al sistema operativo
} 

/// funcion que es llamada por el hilo secundario
void* greet(void* data) {
    (void) data;
    fprintf(stdout, "Hello from secondary thread\n");
    return NULL;
}
