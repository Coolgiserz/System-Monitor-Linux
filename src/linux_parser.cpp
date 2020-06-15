#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#define printVariableNameAndValue(x) std::cout<<"The name of variable **"<<(#x)<<"** and the value of variable is => "<<x<<"\n"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read and return the operation system
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

// Read and return the system kernel information
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> kernel;
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
// 读取并返回系统内存利用
float LinuxParser::MemoryUtilization() {
  float mem_total{0.0};
  float mem_free{0.0};
  float mem_buffer{0.0};
  std::string token;
  
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if(filestream.is_open()){

      while(filestream >> token){
        if(token==LinuxParser::filterMemTotal){
          filestream >> token;
          mem_total = stof(token);
        }else if(token==LinuxParser::filterMemFree){
          filestream >> token;
          mem_free = stof(token);
        }else if(token==LinuxParser::filterBuffer){
          filestream >> token;
          mem_buffer = stof(token);
        }
      }

    // }
  }
  return 1 - mem_free/(mem_total-mem_buffer);
   
}

// Read and return the system uptime
// 读取并返回系统启动时间
long LinuxParser::UpTime() { 
    std::string uptime_str;
    std::string line;
    std::ifstream filestream(kProcDirectory+kUptimeFilename);//proc/uptime
    if(filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream stringstream(line);

        if(stringstream >> uptime_str){
          return stoi(uptime_str);
        }
    }
    return 0; 

 }

// Read and return the number of jiffies for the system
// 读取并返回系统CPU时间（总）
long LinuxParser::Jiffies() {
    vector<string> cpu_utils = CpuUtilization();
    return (stol(cpu_utils[CPUStates::kUser_]) + stol(cpu_utils[CPUStates::kNice_]) +
            stol(cpu_utils[CPUStates::kSystem_]) + stol(cpu_utils[CPUStates::kIRQ_]) +
            stol(cpu_utils[CPUStates::kSoftIRQ_]) + stol(cpu_utils[CPUStates::kSteal_]) +
            stol(cpu_utils[CPUStates::kGuest_]) + stol(cpu_utils[CPUStates::kGuestNice_]) +
            stol(cpu_utils[CPUStates::kIdle_]) + stol(cpu_utils[CPUStates::kIOwait_])
            );
 }

// Read and return the number of active jiffies for a PID
// 读取并返回进程活动时间
long LinuxParser::ActiveJiffies(int pid) { 
  string line, token;
  string u, k, cu, ck;
  long jiffies{0};

  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i=0;
    while (i!=14) {
      linestream >> token;
      i++;
    }
    linestream >> u >> k >> cu >> ck;
     jiffies = stol(u) + stol(k) + stol(cu) + stol(ck);
  }
  return jiffies;
 }

// Read and return the number of active jiffies for the system
// 读取并返回系统活动时间
long LinuxParser::ActiveJiffies() { 
    vector<string> cpu_utils = CpuUtilization();
  return (stol(cpu_utils[CPUStates::kUser_]) + stol(cpu_utils[CPUStates::kNice_]) +
          stol(cpu_utils[CPUStates::kSystem_]) + stol(cpu_utils[CPUStates::kIRQ_]) +
          stol(cpu_utils[CPUStates::kSoftIRQ_]) + stol(cpu_utils[CPUStates::kSteal_]) +
          stol(cpu_utils[CPUStates::kGuest_]) + stol(cpu_utils[CPUStates::kGuestNice_]));
}

// Read and return the number of idle jiffies for the system
// 读取并返回系统闲置时间
long LinuxParser::IdleJiffies() { 
    vector<string>  cpu_utils = LinuxParser::CpuUtilization();
    return stol(cpu_utils[CPUStates::kIdle_]) + stol(cpu_utils[CPUStates::kIOwait_]);
 }

// Read and return CPU utilization
// 读取并返回CPU时间
vector<string> LinuxParser::CpuUtilization() { 
    std::ifstream filestream(kProcDirectory+kStatFilename);
  std::string token;
  // int cpu_times[10];
  vector<string> cpu_times;
  if(filestream.is_open()){
    std::string line;
    std::getline(filestream, line);
    std::istringstream stringstream(line);
    if(stringstream >> token){
      if(token == LinuxParser::filterCpu){
         while(stringstream >> token){
           cpu_times.push_back(token);
         }
        return cpu_times;

      }
    }
  }
  return {};//https://stackoverflow.com/questions/1626597/should-functions-return-null-or-an-empty-object
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

    int total_processes{0};
  std::string line;
  std::string key;
  std::string value;
  std::ifstream fstream(kProcDirectory+kStatFilename);
  if(fstream.is_open()){
    while(std::getline(fstream, line)){
      std::istringstream str_stream(line);
      while(str_stream>>key>>value){
        if(key==LinuxParser::filterProcess){
          total_processes = stoi(value);
          return total_processes;
        }
      }
    }
  }
  return 0; 

}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    int running_processes{0};
    std::string line;
    std::string token;
    std::ifstream fstream(kProcDirectory+kStatFilename);
    while(std::getline(fstream, line)){
      std::istringstream str_stream(line);
      if(str_stream>>token && token==LinuxParser::filterRunningProcess){
          if(str_stream>>token){
          running_processes = stoi(token);
          return running_processes;
          }

      }
    }
    return 0; 
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
    std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(filestream.is_open()){
    std::string line;
    if(std::getline(filestream, line)){
      return line.substr(0, 20);
    }
  }
  return string();
 }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
      string token;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                        LinuxParser::kStatusFilename);
    if (stream.is_open()) {
      while (stream >> token) {
        if (token == LinuxParser::filterProcessMem) {
          if (stream >> token) return std::to_string(stoi(token) / 1024);
        }
      }
    }
    return string("0");
 }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  std::string line;
  std::string token;
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
       std::istringstream stringstream(line);
       if(stringstream >> token){
         if(token == LinuxParser::filterUid){
           stringstream >> token;
           return token;
         }
       }
    }
  }

  return string(); 
}

// Read and return the user associated with a process
// 读取并返回与进程相关的用户
string LinuxParser::User(int pid) {
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  std::string name, token, line;

  if(filestream.is_open()){
    while(std::getline(filestream, line)){
       std::istringstream stringstream(line);
       auto flag = line.find("x:"+uid);
       if(flag!=string::npos){// or if(flag!=line.npos) 
         return line.substr(0, flag-1);
       }
    }
  }
   return string();
  // return uid;
}

// Read and return the uptime of a process
// reference: https://man7.org/linux/man-pages/man5/proc.5.html
long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    string line;
    string token;
    std::getline(filestream, line);
    int i = 0;
    std::istringstream stringstream(line);
    while(i!=22){//when i equal to 22, this loop will stop. So I get the 22nd token, not the 23rd.
      stringstream >> token;
      i++;
    }
    return LinuxParser::UpTime()-stol(token)/sysconf(_SC_CLK_TCK);//stol(token)/sysconf(_SC_CLK_TCK) measure "time the process started after system boot"
  }

  return 0; 
  }