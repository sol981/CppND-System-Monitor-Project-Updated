#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <bits/stdc++.h> 

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line,key,unit; 
  float memTotal, memFree, buffer, cached;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "MemTotal:")
      {
        linestream >> memTotal;
      }
      else if(key == "MemFree:"){
        linestream >> memFree;
      }
      else if(key == "Buffers:"){
        linestream >> buffer;
      }
      else if(key == "Cached:")
      {
        linestream >> cached;
      }
    }
  }
  return (memTotal - memFree - buffer - cached) / memTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  int systemTime,idleTime;

  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if(filestream.is_open())
  {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> systemTime >> idleTime; 
  }
  return systemTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }
  long jiffies{0};
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> CpuUtilizationData = CpuUtilization();
  return (stol(CpuUtilizationData[CPUStates::kUser_]) + stol(CpuUtilizationData[CPUStates::kNice_]) +
          stol(CpuUtilizationData[CPUStates::kSystem_]) + stol(CpuUtilizationData[CPUStates::kIRQ_]) +
          stol(CpuUtilizationData[CPUStates::kSoftIRQ_]) + stol(CpuUtilizationData[CPUStates::kSteal_]) +
          stol(CpuUtilizationData[CPUStates::kGuest_]) + stol(CpuUtilizationData[CPUStates::kGuestNice_]));
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idle{0};
  vector<string> CpuUtilizationData = CpuUtilization();
  idle = stol(CpuUtilizationData[CPUStates::kIdle_]) + stol(CpuUtilizationData[CPUStates::kIOwait_]);
  return idle;
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string cpu, token;
  string line;
  vector<string> CpuUtilizationData;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> cpu;
      if(cpu == "cpu")
      {
        while (linestream >> token) {
          CpuUtilizationData.push_back(token);
        }
        break;
      }
    }
  }
  return CpuUtilizationData;
} 

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, line;
  int totalProcesses{0};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> totalProcesses)
      {
        if(key == "processes")
        {
          return totalProcesses;
        }
      }
    }
  }
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, line;
  int runningProcesses{0};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> runningProcesses)
      {
        if(key == "procs_running")
        {
          return runningProcesses;
        }
      }
    }
  }
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd;
  string pathCmd = kProcDirectory + to_string(pid) + kCmdlineFilename;
  std::ifstream filestream(pathCmd);
  if(filestream.is_open())
  {
    std::getline(filestream, cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key, vmsize,line;
  int ram{0};
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatusFilename);
  if(filestream.is_open())
  {
    while(getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> vmsize)
      {
        if(key == "VmSize:")
        {
          ram = stoi(vmsize) / 1000;
          return to_string(ram);
        }
      }
    }
  } 
  return to_string(ram);
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string token, Uid, line;

  std::ifstream file{kProcDirectory + to_string(pid) + kStatusFilename};
  if(file.is_open())
  {
    while(std::getline(file, line))
    {
      std::stringstream stream{line};
      stream >> token >> Uid;
      if(token == "Uid:")
      {
        return Uid;
      }
    }
  }
  return Uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uID = Uid(pid);
  string line, user, token;

  std::ifstream file{kPasswordPath};
  if(file.is_open())
  {
    while(std::getline(file, line))
    {
      std::stringstream stream{line};
      getline(stream, user, ':');
      getline(stream, token, ':');
      getline(stream, token, ':');
      if(token == uID)
        return user; 
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long time{0};
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i)
      if (i == 11) {
        long time{stol(token)};
        time /= sysconf(_SC_CLK_TCK);
        return time;
      }
  }
  return time;
}
