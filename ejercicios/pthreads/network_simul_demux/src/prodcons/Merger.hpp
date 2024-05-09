#ifndef MERGER_HPP
#define MERGER_HPP

#include <cassert>
#include <iostream>
#include <vector>

#include "Queue.hpp"
#include "Semaphore.hpp"
#include "Thread.hpp"


template <typename DataType>
class Merger : public virtual Thread {
  DISABLE_COPY(Merger);

  protected:
    int producersCount = 0;
    const DataType stopCondition = DataType();
    std::vector<Queue<DataType>*> consumingQueues;
    Queue<DataType>* producingQueue;
    Semaphore* canConsume = nullptr;

  public:
    explicit Merger() {}
    virtual ~Merger() {}

    virtual void consumeForever() {
      bool go = true;
      while (go) {
        this->canConsume->wait(); 
        for (auto consumingQueue : this->consumingQueues) {
          if (!consumingQueue->isEmpty()) {
            const DataType& data = consumingQueue->dequeue();
            if ( data == this->stopCondition ) {
              go = false;
              break;
            }
            this->consume(data);
          }
        }
      }
    }

    virtual void consume(DataType) = 0;

    virtual void produce(const DataType& data) {
      this->producingQueue->enqueue(data);
    }

    Queue<DataType>* getConsumingQueue(int index) {
      return this->consumingQueues[index];
    }

    void setProducingQueue(Queue<DataType>* producingQueue) {
      this->producingQueue = producingQueue;
    }
};

#endif  // MERGER_HPP