#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();

  // DONE: Declare any necessary private members
 private:
  std::vector<long> j_set_{};
  long pre_active_{0};
  long pre_idle_{0};
};

#endif