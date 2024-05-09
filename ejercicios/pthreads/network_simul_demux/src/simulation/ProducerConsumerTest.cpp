/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "MergerTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: prodcons packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  // delete this->dispatcher;
  for ( auto producer : this->producers )
    delete producer;
  delete this->merger;
  delete this->consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  this->producers.resize(this->producerCount);
  // Create objects for the simulation
  for (size_t index = 0; index < this->producerCount; index++) {
    this->producers[index] = new ProducerTest(this->packageCount
      , this->productorDelay, 1);
  }

  this->merger = new MergerTest(this->producerCount);

  this->consumer = new ConsumerTest(this->consumerDelay);
  this->consumer->createOwnQueue();

  // Communicate simulation objects
  for (size_t index = 0; index < this->producerCount; index++) {
    this->producers[index]->setProducingQueue(
      this->merger->getConsumingQueue(static_cast<int>(index)));
  }

  this->merger->setProducingQueue(this->consumer->getConsumingQueue());

  for (size_t index = 0; index < this->producerCount; index++) {
    this->producers[index]->startThread();
  }
  this->merger->startThread();
  this->consumer->startThread();

  this->consumer->waitToFinish();
  this->merger->waitToFinish();
  for (size_t index = 0; index < this->producerCount; index++) {
    this->producers[index]->waitToFinish();
  }

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 5 + 1 arguments are mandatory
  if ( argc != 3 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->producerCount = std::strtoull(argv[index++], nullptr, 10); 

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
