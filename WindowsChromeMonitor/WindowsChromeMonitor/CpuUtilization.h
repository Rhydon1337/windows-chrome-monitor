#pragma once

#include "win32_utils.h"

win32_utils::ProcessId get_target_process_by_cpu_utilizing(const std::string& process_name);

/**
 * @brief get running process cpu percentage
 * @param[in] pid - the process id
 * @return the percentage
 */
float get_process_cpu_utilization_percentage(win32_utils::ProcessId pid);