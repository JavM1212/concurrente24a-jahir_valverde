// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "hello.h"

int main(void) {
    pthread_t thread;   // no es necesario inicializarlo porque de eso se 
                        // encarga phtrad_create
    // si phtread_create devuelve un int distinto  0 es que dio error 
    int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
    if (error == EXIT_SUCCESS) {
        // sleep(1);  // Pausa el hilo actual 1 segundo
        fprintf(stdout, "Hello from main thread\n");  // stdout
        // "Equivalente" al destroy para threads, permite almacenar valores
        // de retorno y evita condiciones de carrera
        pthread_join(thread, /*value_ptr*/ NULL);   
    } else {
        fprintf(stderr, "Could not create secondary thread\n");  // stderr
    }

    return error;  // devolver el error al sistema operativo
} 

// funcion que es llamada por el hilo secundario
void* greet(void* data) {
    (void) data;
    fprintf(stdout, "Hello from secondary thread\n");
    return NULL;
}
