#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  void CpuUtilization(long process_util, long sys_util);
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process & a) const;  // TODO: See src/process.cpp
  bool operator>(const Process & a) const;  // TODO: See src/process.cpp

  Process(int pid);
  // TODO: Declare any necessary private members
 private:
 int _pid;
 float _cpu{0};
 long _cache_system_tick{0};
 long _cache_process_tick{0};
};

#endif