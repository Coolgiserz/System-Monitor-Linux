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

Process::Process(int pid): _pid(pid){}
// Return this process's ID
int Process::Pid() { return _pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{ return _cpu; }


// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(const Process &  a) const { 
        return this->CpuUtilization() < a.CpuUtilization();
 }

// Overload the "greater than" comparison operator for Process objects
 bool Process::operator>(const Process & a) const { 
    return this->CpuUtilization() > a.CpuUtilization();
 }

// caculate this process's CPU utilization
void Process::CpuUtilization(long process_util, long sys_util){
    long process_duration = process_util- _cache_process_tick;
    long duration = sys_util - _cache_system_tick;
    _cpu = static_cast<float>(process_duration)/duration;
    // _cpu = static_cast<float>(process_duration)/static_cast<float>(duration);
    // _cpu = （float)process_duration/duration;

    _cache_system_tick = sys_util;
    _cache_process_tick = process_util;
}
