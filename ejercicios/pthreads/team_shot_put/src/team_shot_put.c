/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>

/**
 * @brief Structs
 * 
 */
typedef struct shared_data_t {
  int64_t count;
} shared_data_t;

typedef struct best_shot_t {
  float best_shot;

  int64_t team;
  int64_t player_number;
  int64_t id;  /// usado solamente para generar numeros aleatorios 

  shared_data_t* shared_data;
} best_shot_t;

/**
 * @brief Declaraciones
 * 
 */
int64_t create_pthread_teams(shared_data_t* shared_data
  , pthread_t* team_threads, best_shot_t* best_shot);
void* execute_teams_shots(void* data);
float gen_random_number(int64_t seed);
void arbitrar(best_shot_t* best_shot);

/**
 * @brief Arbitro
 * 
 */
int main(int argc, char* argv[]) {
  int64_t error = EXIT_SUCCESS;
  int64_t athlete_count = 3;  /// cantidad de jugadores por defecto

  /// validacion de entradas
  if (argc == 2) {
    if (!((sscanf(argv[1], "%" SCNu64, &athlete_count)) == 1)) {
        fprintf(stderr, "Error: invalid athlete count");
        return 11;
    }
    if (athlete_count < 0 || athlete_count % 2 == 0) {
        fprintf(stderr, "Error: only odd numbers higher than 0 are allowed \n");
        return 12;
    }
  }

  /// callocs
  pthread_t* phthreads = (pthread_t*) calloc(2, sizeof(pthread_t));
  shared_data_t* shared_data = (shared_data_t*) calloc(1, sizeof(shared_data_t));
  /**
   * @brief best_shots es la pizarra donde los jugadores anotaron los 
   *        resultados. Luego el arbitro va a imprimir los resultados
   *        y determinar el ganador
   * 
   */
  best_shot_t* best_shots = (best_shot_t*) calloc(athlete_count * 2
    , sizeof(best_shot_t));

  if (phthreads && shared_data && best_shots) {
    /// settear valor de memoria compartida
    shared_data->count = athlete_count;

    for (int64_t i = 0; i < athlete_count * 2; i++) {
      best_shots[i].shared_data = shared_data;
      best_shots[i].id = i * 100;
    }
    
    /// crear hilos
    error = create_pthread_teams(shared_data, phthreads, best_shots);
  } else {
    fprintf(stderr, "Error: could not allocate dynamic data");
    error = 13;
  }

  /**
   * @brief 
   *        .resultados Luego el arbitro imprime los result
   * 
   */
  arbitrar(best_shots);

  /// frees
  free(best_shots);
  free(shared_data);
  free(phthreads);
  return error;
}

/**
 * @brief Helper para crear hilos
 * 
 */
int64_t create_pthread_teams(shared_data_t* shared_data, pthread_t* phthreads
  , best_shot_t* best_shots) {
  int64_t error = EXIT_SUCCESS;

  for (int64_t team = 0; team < 2; team++) {  /// 2 equipos (hilos) siempre
    /// primera mitad para el primer equipo (hilo) y segunda para el segundo
    (best_shots + team * shared_data->count)->team = team;

    error = pthread_create(&phthreads[team], /*attr*/ NULL, execute_teams_shots
      , best_shots + team * shared_data->count);

    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "Error: could not create secondary threads\n");
      error = 22;
      break;
    }
  }

  for (int64_t team_thread = 0; team_thread < 2; team_thread++) {
    pthread_join(phthreads[team_thread], /*value_ptr*/ NULL);
  }

  return error;
}

/**
 * @brief Funcion para realizar los lanzamientos
 * 
 */
void* execute_teams_shots(void* data) {
  best_shot_t* best_shots = (best_shot_t*) data;
  shared_data_t* shared_data = best_shots[0].shared_data;
  int64_t team = best_shots[0].team;

  float aux = 0.0;
  for (int64_t athlete = 0; athlete < shared_data->count; athlete++) {
    best_shots[athlete].player_number = athlete;
    best_shots[athlete].team = team;
    aux = 0.0;
    for (int64_t intento = 0; intento < 3; intento++) {
      aux = gen_random_number(best_shots[athlete].id + intento);
      if (aux > best_shots[athlete].best_shot) {
        best_shots[athlete].best_shot = aux;
      }
    }
  }

  return NULL;
}

/**
 * @brief create random number
 * 
 */
float gen_random_number(int64_t seed) {
  unsigned int u_seed = time(NULL) + seed;
  /**
   * @brief Se le suma un numero generado con info del thread para intentar
   *        generar una seed diferente
   * 
   *        Todas las operaciones son en float para que castee el retorno como
   *        eso
   * 
   *        rand_r devuelve un numero aleatorio, y la constante RAND_MAX 
   *        representa el maximo valor que puede generar la funcion
   * 
   *        Por eso al dividir entre RAND_MAX obtengo un intervalo de valores
   *        posibles que van como [0.0,1.0], y al multiplicarlo por 25 el nuevo
   *        intervalo va de [0.0,25.0]
   *        
   * 
   */
  return ((float) rand_r(&u_seed) / (float) RAND_MAX) * 25.0;;
}

/**
 * @brief 
 * 
 */
void arbitrar(best_shot_t* best_shots) {
  /// Puedo extraer este dato de cualquier team porque es memoria compartida
  shared_data_t* shared_data = best_shots[0].shared_data;
  int64_t* pts = (int64_t*) calloc(2, sizeof(int64_t));
  int64_t winner = 0;

  for (int64_t best_shot = 0; best_shot < shared_data->count * 2; best_shot++) {
    fprintf(stdout, "%" PRIu64 ".%" PRIu64 " best shot put %.2fm\n"
    , (best_shots[best_shot].team + 1), (best_shots[best_shot]
    .player_number + 1), best_shots[best_shot].best_shot);
  }

  for (int64_t best_shot = 0; best_shot < shared_data->count; best_shot++) {
    if (best_shots[best_shot].best_shot > best_shots[best_shot + shared_data->count].best_shot) {
      pts[0]++;
    } 
    if (best_shots[best_shot].best_shot < best_shots[best_shot + shared_data->count].best_shot) {
      pts[1]++;
    }
  }

  if (pts[0] > pts[1]) {
    winner = 1;
  }
  if (pts[0] < pts[1]) {
    winner = 2;
  }
  
  if (winner != 0) {
    fprintf(stdout, "result %" PRIu64 ":%" PRIu64 " team %" PRIu64 " wins\n", pts[0], pts[1], winner);
  } else {
    fprintf(stdout, "result %" PRIu64 ":%" PRIu64 " DRAW\n", pts[0], pts[1]);
  }

  free(pts);  
}