#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long remain_hh = seconds % 3600;
  long hh = (seconds - remain_hh) / 3600;
  long ss = remain_hh % 60;
  long mm = (remain_hh - ss) / 60;
  string times = std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
  return times;
}