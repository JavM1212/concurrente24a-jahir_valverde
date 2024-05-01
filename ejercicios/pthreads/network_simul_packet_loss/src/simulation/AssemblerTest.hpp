/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include "Assembler.hpp"
#include "NetworkMessage.hpp"

/**
 * @brief An Assembler class example
 */
class AssemblerTest : public Assembler<int, NetworkMessage> {
  DISABLE_COPY(AssemblerTest);

 protected:
  float probability = 0.0;

 public:
  /// Constructor
  explicit AssemblerTest(float probability);
  /// Start redirecting network messages
  int run() override;
};

#endif  // ASSEMBLERTEST_HPP
