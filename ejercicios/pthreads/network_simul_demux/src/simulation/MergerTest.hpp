#ifndef MERGERTEST_HPP
#define MERGERTEST_HPP

#include "Merger.hpp"
#include "NetworkMessage.hpp"

class MergerTest : public Merger<NetworkMessage> {
  DISABLE_COPY(MergerTest);

  protected:

  public:
    explicit MergerTest(int producersCount);
    ~MergerTest();

    int run() override;
    void consume(NetworkMessage data) override;
};

#endif  // MERGERTEST_HPP
