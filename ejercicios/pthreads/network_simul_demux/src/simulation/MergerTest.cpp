#include "Log.hpp"
#include "MergerTest.hpp"
#include "Queue.hpp"
#include "Semaphore.hpp"

MergerTest::MergerTest(int producersCount) {
  this->producersCount = producersCount;
  this->canConsume = new Semaphore(0);
  
  this->consumingQueues.resize(this->producersCount);
  for (int index = 0; index < this->producersCount; index++) {
    this->consumingQueues[index] = new Queue<NetworkMessage>(false);
    this->consumingQueues[index]->setSemaphore(canConsume);
  }
}

MergerTest::~MergerTest() {
  delete this->canConsume;
  for (int index = 0; index < this->producersCount; index++) {
    delete this->consumingQueues[index];
  }
}

int MergerTest::run() {
  this->consumeForever();

  this->produce(NetworkMessage());

  Log::append(Log::INFO, "Merger", " redirect");

  return EXIT_SUCCESS;
}

void MergerTest::consume(NetworkMessage data) {
  this->produce(data);
}