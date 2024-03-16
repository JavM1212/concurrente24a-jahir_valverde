// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include "grandma_lottery.h"

// procedure main
int main(void) {
    // create_thread(greet)
    pthread_t thread;   // no es necesario inicializarlo porque de eso se 
                        // encarga phtrad_create
    // si phtread_create devuelve un int distinto  0 es que dio error 
    int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
    if (error == EXIT_SUCCESS) {
        // print "Hello from main thread"
        // sleep(1);  // Pausa el hilo actual 1 segundo
        fprintf(stdout, "Hello from main thread\n");  // stdout
        pthread_join(thread, /*value_ptr*/ NULL);  // Equivalente al destroy
    } else {
        // print "Hello from main thread"
        fprintf(stderr, "Could not create secondary thread\n");  // stderr
    }

    return error;  // devolver el error al sistema operativo
}  // end procedure

// procedure greet
void* greet(void* data) {
    (void) data;
    // print "Hello from secondary thread"
    fprintf(stdout, "Hello from secondary thread\n");
    return NULL;
}  // end procedure
