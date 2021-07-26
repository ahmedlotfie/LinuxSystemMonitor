#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

#include "linux_parser.h"
using namespace std;

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp
  
  // TODO: Declare any necessary private members
 private:
    double prevIdle , prevTotal;
};

#endif