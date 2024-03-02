#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float cpuUsage{0};
    long active = LinuxParser::ActiveJiffies();
    long idle = LinuxParser::IdleJiffies();
    long duration_active = active - activeCached;
    long duration_idle = idle - idleCached;
    long duration = duration_active + duration_idle;
    cpuUsage = static_cast<float>(duration_active) / duration;
    activeCached = active;
    idleCached = idle;
   
   return cpuUsage;
}