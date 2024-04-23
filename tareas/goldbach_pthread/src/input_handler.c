#include "input_handler.h"

/// lee de la entrada estandar los numeros ingresados y valida que sean ints
input_t* read_input() {
  input_t* input = (input_t*) calloc(1, sizeof(input_t));
  int64_t* input_arr = (int64_t*) calloc(1, sizeof(int64_t));
  if (!(input && input_arr)) {  /// validar memoria
    fprintf(stderr, "Error: Could not allocate input/input_arr"
      " in memory. errno: %i\n", errno);
    return NULL;
  }

  int64_t count = 0;

  FILE* f_input = stdin;
  int64_t value = 0;
  while (1) {
    if ((fscanf(f_input, "%" SCNd64, &value)) == 1) {
      input_arr[count] = value;
      count++;
      input_arr = (int64_t*) realloc(input_arr, sizeof(int64_t) * (count + 1));
    } else {  /// si no logro leer mas
      if (fgetc(f_input) != EOF) {  /// preguntamos si es por el eof
        /// sino es por el eof, es porque quiso leer algun tipo de dato como
        /// int64t cuando no lo era, por lo que liberamos memoria, setteamos
        /// codigo de error e imprimimos el error
        free(input_arr);
        free(input);
        errno = 21;
        fprintf(stderr, "Error: invalid input. errno: %i\n", errno);
        return NULL;
      }
      /// si si fue por el EOF, no hay problema entonces solo salga del while
      /// infinito
      break;
    }
  }

  /// setteamos los valores que obtenimos
  input->input_arr = input_arr;
  input->size = count;

  return input;
}

/// obtener argumentos de la linea de comandos
int analyze_arguments(int argc, char* argv[]) {
  int thread_count = sysconf(_SC_NPROCESSORS_ONLN);

  /// si son 2 argumentos settea el segundo
  if (argc == 2) {
    int temp_thread_count = atoi(argv[1]);

    /// si la entrada fue valida la asignamos
    if (temp_thread_count > 0) {
      thread_count = temp_thread_count;
    }
  }

  return thread_count;
}