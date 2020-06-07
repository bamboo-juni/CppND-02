#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string hh_s, mm_s, ss_s;
  const long remain_hh = seconds % 3600;
  long hh = (seconds - remain_hh) / 3600;
  long ss = remain_hh % 60;
  long mm = (remain_hh - ss) / 60;
  // make sure hh, mm, ss always two numbers
  if(hh < 10)
  {
    hh_s = "0" + std::to_string(hh);
  }
  else
  {
    hh_s = std::to_string(hh);
  }

  if(mm < 10)
  {
    mm_s = "0" + std::to_string(mm);
  }
  else
  {
    mm_s = std::to_string(mm);
  }

  if(ss < 10)
  {
    ss_s = "0" + std::to_string(ss);
  }
  else
  {
    ss_s = std::to_string(ss);
  }

  string times = hh_s + ":" + mm_s + ":" + ss_s;
  return times;
}