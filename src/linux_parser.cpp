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
        if (key == filterOSName) {
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
// return percent without *100, ncurses_display *100
float LinuxParser::MemoryUtilization() { 
  string line;
  string key, value;
  float mem_total, mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == filterMemTotalString) {
          mem_total = stof(value);
        }
        else if(key == filterMemFreeString) {
          mem_free = stof(value);
        }
        else break;
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// DONE: Read and return the system uptime
// /proc/uptime value already in seconds
long LinuxParser::UpTime() { 
  long uptime = 0;
  string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    uptime = stol(value);
    return uptime;
  }
  return uptime;
}

// Read and return a set of jiffies
vector<long> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string j_user, j_nice, j_sys, j_idle;
  vector<long> j_set;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> j_user >> j_nice >> j_sys >> j_idle) {
        if (key == filterCpu) {
          j_set.push_back(stol(j_user));
          j_set.push_back(stol(j_nice));
          j_set.push_back(stol(j_sys));
          j_set.push_back(stol(j_idle));
          return j_set;
        }
      }
    }
  }
  return j_set;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(vector<long> j_set) { 
  return j_set[0] + j_set[1] + j_set[2] + j_set[3];
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long utime, stime, cutime, cstime;
  string line, t_time;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 17; i++) {
      linestream >> t_time;
      if(i == 13) {
        utime = stol(t_time);
      }
      else if(i == 14) {
        stime = stol(t_time);
      }
      else if(i == 15) {
        cutime = stol(t_time);
      }
      else if(i == 16) {
        cstime = stol(t_time);
      }
      else {}
    }
  }
  return utime + stime + cutime + cstime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(vector<long> j_set) {
  return j_set[0] + j_set[1] + j_set[2];
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(vector<long> j_set) {
  return j_set[3];
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int num_total;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterProcesses) {
          num_total = stoi(value);
          return num_total;
        }
      }
    }
  }
  return num_total;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int num_ru;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterRunningProcesses) {
          num_ru = stoi(value);
          return num_ru;
        }
      }
    }
  }
  return num_ru;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value, ram_string;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterProcMem) {
          ram_string = to_string(stol(value) / 1000);
          return ram_string;
        }
      }
    }
  }
  return ram_string;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterUID) {
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(string uid) {
  string line;
  string key;
  string index, value;
  //uid_s = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> index >> value) {
        if (value == uid) {
          return key;
        }
      }
    }
  }
  return key;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string uptime;
  string line;
  long uptime_l = 0;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++) {
      linestream >> uptime;
    }
    uptime_l = stol(uptime) / sysconf(_SC_CLK_TCK);
    return LinuxParser::UpTime() - uptime_l;
  }
  return LinuxParser::UpTime() - uptime_l;
}