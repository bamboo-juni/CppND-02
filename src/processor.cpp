#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    float utilization = 0.0;
    j_set_ = LinuxParser::CpuUtilization();
    const long curr_active = LinuxParser::ActiveJiffies(j_set_);
    const long curr_idle = LinuxParser::IdleJiffies(j_set_);
    const long delta_active = curr_active - pre_active_;
    const long delta_idle = curr_idle - pre_idle_;
    utilization = static_cast<float>(delta_active) / (delta_active + delta_idle);
    pre_active_ = curr_active;
    pre_idle_ = curr_idle;
    return utilization;
}