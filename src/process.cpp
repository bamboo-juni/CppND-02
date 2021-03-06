#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  cpuuti_ = static_cast<float>(LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK)) / LinuxParser::UpTime(pid_);
  return cpuuti_;
}

// DONE: Return the command that generated this process
string Process::Command() {
  string com_out = LinuxParser::Command(pid_);
  if(com_out.size() > 40) {
      com_out = com_out.substr(0, 50) + "...";
  }
  return com_out;
}

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(LinuxParser::Uid(pid_)); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    if(pid_ == a.pid_)
    {
        return false;
    }
    else
    {
        if(cpuuti_ != a.cpuuti_)
        {
            return cpuuti_ > a.cpuuti_;
        }
        else
        {
            return false;
        }
    }
}