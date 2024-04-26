/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"
 
/// modificamos el mensaje de usage
const char* const usage =
  "Usage: "
  "prodcons packages producers consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  producers   number of producer threads\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for ( ProducerTest* producer : this->producers ) {
    delete producer;
  }
  delete this->dispatcher;
  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  // Create objects for the simulation

  /// ahora se crean varios prodcers en vez de solo uno, no es necesario
  /// crear una cola por producto porque todos comparten la del dispatcher
  this->producers.resize(this->producerCount);
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    /// para calcular cuantos paquetes se le van a dar a cada prodcutor, 
    /// se va a aplicar la formula de mapeo estatico por bloque que 
    /// se proporciona en https://jeisson.ecci.ucr.ac.cr/concurrente/2022b/material/#mapping
    /// para obtener los rangos, luego se va a restar finish - start para obtener
    /// la cantidad (que en este caso es lo que se ocupa)
    /// en este caso
    /// i = index, D = this->packageCount, w = this->producerCount

    size_t start = get_static_block_start(index, this->packageCount, this->producerCount);
    size_t finish = get_static_block_start(index + 1, this->packageCount, this->producerCount);
    size_t producerPackageCount = finish - start;

    this->producers[index] = new ProducerTest(producerPackageCount, this->productorDelay
    , this->producerCount);
    assert(this->producers[index]);
  }
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  this->dispatcher->producerStopMessages = this->producerCount;
  this->dispatcher->createOwnQueue();
  // Create each producer
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue();
  }

  // Communicate simulation objects
  // Producer push network messages to the dispatcher queue
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    /// ahora se asigna la queue del dispatcher para cada producer
    this->producers[index]->setProducingQueue(this->dispatcher->getConsumingQueue());
  }
  // this->producer->setProducingQueue(this->dispatcher->getConsumingQueue());
  // Dispatcher delivers to each consumer, and they should be registered
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->dispatcher->registerRedirect(index + 1
      , this->consumers[index]->getConsumingQueue());
  }

  // Start the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    /// ahora se inicializa el hilo de cada productor en vez de solo uno
    this->producers[index]->startThread();
  }
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->startThread();
  }

  // Wait for objets to finish the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    /// se hace join de cada productor en vez de solo 1
    this->producers[index]->waitToFinish();
  }
  this->dispatcher->waitToFinish();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 5 + 1 arguments are mandatory
  if ( argc != 7 ) {  /// ahora son 7 no 6
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  /// agregamos la lectura del producerCount
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}

size_t ProducerConsumerTest::get_static_block_start(size_t i, size_t D, size_t w) {
  size_t min = get_static_block_min(i, (D % w));
  return i * (D / w) + min;
}

size_t ProducerConsumerTest::get_static_block_min(size_t i, size_t mod) {
  if (i < mod) {
    return i;
  }
  if (i > mod) {
    return mod;
  }
  // i == mod 
  return i;
}
