#include <vector>
#include <numeric>
#include <string>
#include "processor.h"
#include "linux_parser.h"
using std::string;
Processor::Processor(){
    _tick_all = LinuxParser::Jiffies();
    _tick_idle = LinuxParser::IdleJiffies();

}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    //  std::vector<string> cpu_times = LinuxParser::CpuUtilization();

    //  std::vector<float> cpu_times_int;
    //  for(string s: cpu_times){
    //      cpu_times_int.push_back(stof(s));
    //  }
     float cached_all = _tick_all;
    float cached_idle = _tick_idle;

    _tick_all = LinuxParser::Jiffies();
    _tick_idle = LinuxParser::IdleJiffies();
    float v = ((_tick_all-cached_all)-(_tick_idle-cached_idle))/(_tick_all-cached_all);
    return (v>0.0)?v:0.0;
    // return (cpu_times_int[0] + cpu_times_int[1])/std::accumulate(cpu_times_int.begin(), cpu_times_int.end(), 0.0);

}