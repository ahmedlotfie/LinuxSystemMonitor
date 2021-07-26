#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int processId):pId(processId){
    cpu = CPUUtilizationCalculation();
    command = LinuxParser::Command(processId);
    user = LinuxParser::User(processId);
    upTime = LinuxParser::UpTime(processId);
    ram = LinuxParser::Ram(processId);
}

// TODO: Return this process's ID
int Process::Pid() { return pId; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu; }

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if(a.pId< this->pId){return true;}
    return false; 
}

float Process::CPUUtilizationCalculation()
{
    long currentCpuTotalUpTime = LinuxParser::UpTime();
    std::vector<float>CpuUtilis = LinuxParser::CpuUtilizationforProcess(pId);
    if(CpuUtilis.size() == 5)
    {
        float totalprocessUptime = CpuUtilis[LinuxParser::ProcesIdStatIndex::kCutime]+
         CpuUtilis[LinuxParser::ProcesIdStatIndex::kCstime] +
         CpuUtilis[LinuxParser::ProcesIdStatIndex::kStime]+
         CpuUtilis[LinuxParser::ProcesIdStatIndex::kUtime];

        float seconds = currentCpuTotalUpTime - CpuUtilis[LinuxParser::ProcesIdStatIndex::kStartTime];
        return (totalprocessUptime/ seconds);
    }
    return 0;
}