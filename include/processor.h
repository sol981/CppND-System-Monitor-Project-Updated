#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  Processor(int id);
  float Utilization();  // DONE: See src/processor.cpp
  int CpuID();

  // DONE: Declare any necessary private members
private:
  long activeCached_;
  long idleCached_;
  int id_;
};

#endif