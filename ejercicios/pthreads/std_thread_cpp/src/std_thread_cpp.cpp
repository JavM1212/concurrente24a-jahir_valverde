/// Copyright 2024 Jahir Valverde <jahir.valverde@ucr.ac.cr>

/// 1. Su archivo solución debe tener extensión .cpp

/// 10. Elimine los encabezados dependientes del sistema operativo.

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

struct shared_data_t {
  int64_t thread_count;
  std::mutex cout_mutex;
};

struct private_data_t {
  int64_t thread_number;
  std::shared_ptr<shared_data_t> shared_data;
};

void create_threads(std::shared_ptr<shared_data_t> shared_data);
void greet(private_data_t* private_data);

int main(int argc, char* argv[]) {
  int64_t error = EXIT_SUCCESS;

  /// 2. Obtenga la cantidad de CPUs disponibles en el sistema
  int64_t thread_count = std::thread::hardware_concurrency();
  if  (argc == 2) {
    /// 8. Use std::istringstream en lugar de sscanf().
    std::istringstream thread_count_input(argv[1]);
    if (!(thread_count_input >> thread_count)) {
      /// 7. Cambie la entrada y salida de C por la genérica de C/C++.
      std::cerr << "Error: invalid thread count" << "\n";
      error = 11;
    }
  }

  if (error == EXIT_SUCCESS) {
    /// 5. Es recomendable usar un puntero inteligente (std::shared_ptr<>)
    auto shared_data = std::make_shared<shared_data_t>();
    /// 9. Atrape y maneje las excepciones en el procedimiento main().
    try {
      shared_data->thread_count = thread_count;
      create_threads(shared_data);
    } catch(const std::exception& e) {
      /// 7. Cambie la entrada y salida de C por la genérica de C/C++.
      std::cerr << "Error: " << e.what() << "\n";
      error = 12;
    }
  }

  return error;
}

void create_threads(std::shared_ptr<shared_data_t> shared_data) {
  std::vector<std::thread> threads;
  /// 6. Aproveche que conoce el tamaño para crearlo de la capacidad justa
  std::vector<private_data_t> private_data(shared_data->thread_count);

  for (int64_t thread_number = 0; thread_number < shared_data->thread_count
    ; thread_number++) {
    private_data[thread_number].thread_number = thread_number;
    private_data[thread_number].shared_data = shared_data;
    /// 3. agréguelos al vector con el método emplace_back()
    threads.emplace_back(greet, &private_data[thread_number]);
  }

  /// 7. Cambie la entrada y salida de C por la genérica de C/C++.
  std::cout << "Hello from main thread \n";

  for (int64_t thread_number = 0; thread_number < shared_data->thread_count
    ; thread_number++) {
    threads[thread_number].join();
  }
}

void greet(private_data_t* private_data) {
  auto shared_data = private_data->shared_data;

  /**
   * @brief Usado para que no se den condiciones de carrera en la salida
   *        estanda. Se podria ver como, que solo un hilo imprima a la vez.
   *        Esto no anula la concurrencia y se puede ver al ejecutar el
   *        programa
   * @ref   https://www.geeksforgeeks.org/std-mutex-in-cpp/
   * 
   */
  std::lock_guard<std::mutex> lock(shared_data->cout_mutex);

    /// 7. Cambie la entrada y salida de C por la genérica de C/C++.
  std::cout << "Hello from secondary thread " << private_data->thread_number
    << " of " << shared_data->thread_count << "\n";
}
