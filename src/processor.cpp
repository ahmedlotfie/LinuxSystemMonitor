#include "processor.h"

Processor::Processor():prevIdle(0.0),prevTotal(0.0){};
// Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> cpuUtilsStrs = LinuxParser::CpuUtilization(); 
    vector<unsigned long long> cpuUtilsUll = {};
    for(string value : cpuUtilsStrs)
    {
        try
        {
            cpuUtilsUll.push_back(std::stoull(value));
        }
        catch(...)
        {
            cpuUtilsUll.push_back((unsigned long long )0);
        }
    }
    double nonIdle = cpuUtilsUll[LinuxParser::CPUStates::kUser_] +cpuUtilsUll[LinuxParser::CPUStates::kNice_]+
                    cpuUtilsUll[LinuxParser::CPUStates::kSystem_] + cpuUtilsUll[LinuxParser::CPUStates::kIRQ_]+
                    cpuUtilsUll[LinuxParser::CPUStates::kSoftIRQ_] + cpuUtilsUll[LinuxParser::CPUStates::kSteal_];
    double idle = cpuUtilsUll[LinuxParser::CPUStates::kIdle_] + cpuUtilsUll[LinuxParser::CPUStates::kIOwait_];
    
    double total = idle + nonIdle;
    double totalDiff = total - prevTotal;
    double idleDiff = idle - prevIdle;

    float cpuPercentage = static_cast<float>((totalDiff - idleDiff) /totalDiff);
    
    prevIdle = idle;
    prevTotal = total;
    
    return cpuPercentage;
}