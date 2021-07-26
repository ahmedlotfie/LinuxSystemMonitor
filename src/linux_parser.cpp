#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key,value;
  double memTotal = -1,memFree = -1;
  try{
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if(! stream.is_open())
      return -100; //ErrorCode
    while(std::getline(stream,line))
    {
      std::replace(line.begin(), line.end(),':',' ');
      std::stringstream ss(line);
      while(ss >>key>>value)
      {
        if(key == MEMTOTALSTR){
          memTotal = std::stod(value);
          break;
        }
        else if (key == MEMFREESTR){
          memFree = std::stod(value);
          break;
        }
        if(memTotal != -1 && memFree != -1)
          break;
      }
      
    }

    float memUtilization = (memTotal - memFree) / memTotal;
    
    return memUtilization; 
  }
  catch(...)
  {
    return -100;
  }

}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string upTime;
  try{
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (!stream.is_open())
      return 0;

    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> upTime)
      {
        return std::stoull(upTime);
      }
    }
  }
  catch(...)
  {
    return 0;
  }
  return 0;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
 {
  vector<string>cpuUtils = {};
  string line, cpuName,user,nice,system,idle, iowait,irq,softirq,steal,guest,guestNice;

  try
  {
    std::ifstream stream(kProcDirectory + kStatFilename);
    if(! stream.is_open())
      return {};
    while(std::getline(stream,line))
    {
      std::stringstream ss(line);
      while(ss >>cpuName>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guestNice)
      {
        if(cpuName == CPUSTR)
        {
          cpuUtils.push_back(user);
          cpuUtils.push_back(nice);
          cpuUtils.push_back(system);
          cpuUtils.push_back(idle);
          cpuUtils.push_back(iowait);
          cpuUtils.push_back(irq);
          cpuUtils.push_back(softirq);
          cpuUtils.push_back(steal);
          cpuUtils.push_back(guest);
          cpuUtils.push_back(guestNice);

          return cpuUtils;
        }

      }
      
    }

  }
  catch(...)
  {
    return {};
  }
  return {};
}

// Read and return the total number of  processes
int LinuxParser::TotalProcesses() {

  string line, processesName,totalNumber;
  try{
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (!stream.is_open())
      return -102;

    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> processesName >> totalNumber)
      {
        if (processesName == PROCESSESSTR){
          return std::stoi(totalNumber);
        }
      }
    }
  }
  catch(...)
  {
    return -102;
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, processesName,totalNumber;
    try{
      std::ifstream stream(kProcDirectory + kStatFilename);
      if (!stream.is_open())
        return -103;

      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        while(linestream >> processesName >> totalNumber)
        {
          
          if (processesName == PROCESSESRUNNINGSTR){
            return std::stoi(totalNumber);
            }
        }
      }
    }
    catch(...)
    {
      return -103;
    }
    return 0;  
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command = "";
  std::ifstream stream(kProcDirectory + "/"+std::to_string(pid)+ kCmdlineFilename);
  if(!stream.is_open())
    return string();
  std::getline(stream,command);
  return command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == KEYWORD) {
          long ram = std::stol(value);
          ram/=1000;
          return std::to_string(ram);
        }
      }
    }
  }
  return "0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
   string line, key,value;
    try
    {
      std::ifstream stream(kProcDirectory + "/"+ std::to_string(pid)+ kStatusFilename);
      if (!stream.is_open())
        return "";

      while(std::getline(stream, line))
      {
        std::replace(line.begin(),line.end(),':',' ');
        std::istringstream linestream(line);
        while(linestream >>key>>value)
        {
          if(key == UIDSTR)
            return value;
        }

      }
  
    }
    catch(...)
    {
      return "";
    }
    return "";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{
  string line, user,temp, key, userId;
  userId = Uid(pid);

  try{
    std::ifstream stream(kPasswordPath);
    if (!stream.is_open())
      return "";

    while(std::getline(stream, line))
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream >> user >> temp >>key){
        if(key == userId)
          return user;
      }
    }
    
  }
  catch(...)
  {
    return "";
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;

  try{
    std::ifstream stream(kProcDirectory +"/"+std::to_string(pid)+ kStatFilename);
    if (!stream.is_open())
      return 0;

    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);

      for(int i = 1;i<=ProcesIdStatIndex::kStartTime; i++)
      {
        linestream >>value;
        if(i == ProcesIdStatIndex::kStartTime)
          {
            long uptime = UpTime() - (std::stoull(value) / sysconf(_SC_CLK_TCK));
            return uptime;
          }      
      }
    }
  }
  catch(...)
  {
    return 0;
  }
  return 0;
}

std::vector<float> LinuxParser::CpuUtilizationforProcess(int pid)
{
  std::vector<float> values{};
  string line, value;
  try{
    std::ifstream stream(kProcDirectory +"/"+std::to_string(pid)+ kStatFilename);
    if (!stream.is_open())
      return {};

    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);

      for(int i = 1;i<=ProcesIdStatIndex::kStartTime; i++)
      {
        linestream >>value;
        if(i == ProcesIdStatIndex::kCstime || i == ProcesIdStatIndex::kCutime ||
            i == ProcesIdStatIndex::kStime || i == ProcesIdStatIndex::kUtime || i == ProcesIdStatIndex::kStartTime)
          {
            float temp = std::stof(value) / sysconf(_SC_CLK_TCK);
            values.push_back(temp);
          }      
      }
    }
  }
  catch(...)
  {
    return {};
  }
  return values;
}