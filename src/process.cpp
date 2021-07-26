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

// Return this process's ID
int Process::Pid() { return pId; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpu; }

// Return the command that generated this process
string Process::Command() { return command; }

// Return this process's memory utilization
string Process::Ram() { return ram; }

// Return the user (name) that generated this process
string Process::User() { return user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return upTime; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    if(a.cpu < cpu){return true;}
    return false; 
}

float Process::CPUUtilizationCalculation()
{
    long currentCpuTotalUpTime = LinuxParser::UpTime();
    std::vector<float>CpuUtilis = LinuxParser::CpuUtilizationforProcess(pId);
    if(CpuUtilis.size() == 5)
    {
        float totalprocessUptime = CpuUtilis[0]+
         CpuUtilis[1] +
         CpuUtilis[2]+
         CpuUtilis[3];

        float seconds = currentCpuTotalUpTime - CpuUtilis[LinuxParser::ProcesIdStatIndex::kStartTime];
        return std::abs(totalprocessUptime/ seconds);
    }
    return 0;
}