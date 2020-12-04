#pragma once

#include <chrono>

#include "win32_utils.h"

constexpr auto SECONDS_TO_WAIT_BETWEEN_TIMES_EXAMINATIONS = std::chrono::seconds(1);

struct ProcessCpuUsage {
	double cpu_usage;
	ProcessId pid;
};

ProcessId locate_process_by_cpu_utilizing(const std::string& process_name);

ProcessId locate_process_by_cpu_utilizing_over_time(const std::string& process_name, std::chrono::seconds locate_time);

/**
 * @brief get running process cpu percentage
 * @param[in] pid - the process id
 * @param[in] seconds - the time to wait between each examination 
 * @return the percentage
 */
double get_process_cpu_utilization_percentage(ProcessId pid, std::chrono::seconds seconds=SECONDS_TO_WAIT_BETWEEN_TIMES_EXAMINATIONS);