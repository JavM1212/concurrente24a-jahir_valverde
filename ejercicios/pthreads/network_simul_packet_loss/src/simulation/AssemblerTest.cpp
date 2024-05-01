/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "AssemblerTest.hpp"
#include "Util.hpp"

AssemblerTest::AssemblerTest(float probability)
  : probability(probability) {
}

int AssemblerTest::run() {
  // Dispatch all the network messages we receive to their respective queues
  this->consumeForever();

  // If we exited from the forever loop, the finish message was received
  // For this simulation is OK to propagate it to all the queues
  this->producingQueue->enqueue(this->stopCondition);

  return EXIT_SUCCESS;
}

