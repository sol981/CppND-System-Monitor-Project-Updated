#include "processor.h"

#include "linux_parser.h"

Processor::Processor(int id) { id_ = id; }

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  float cpuUsage{0};
  long active = LinuxParser::CpuActiveJiffies(CpuID());
  long idle = LinuxParser::CpuIdleJiffies(CpuID());
  long duration_active = active - activeCached_;
  long duration_idle = idle - idleCached_;
  long duration = duration_active + duration_idle;
  cpuUsage = static_cast<float>(duration_active) / duration;
  activeCached_ = active;
  idleCached_ = idle;

  return cpuUsage;
}

int Processor::CpuID() { return id_; }