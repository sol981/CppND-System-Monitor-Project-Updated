#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  Process(int pid);
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization() const;
  void CpuUtilization(long active_ticks, long system_ticks);
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;
  
  // DONE: Declare any necessary private members
 private:
    int pid_;
    int cpu_;
    long cached_active_ticks_{0};
    long cached_idle_ticks_{0};
    long cached_system_ticks_{0};
};

#endif