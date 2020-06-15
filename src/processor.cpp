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
// Return the aggregate CPU utilization
float Processor::Utilization() { 

    float cached_all = _tick_all;
    float cached_idle = _tick_idle;

    _tick_all = LinuxParser::Jiffies();
    _tick_idle = LinuxParser::IdleJiffies();
    float v = ((_tick_all-cached_all)-(_tick_idle-cached_idle))/(_tick_all-cached_all);
    return (v>0.0)?v:0.0;

}