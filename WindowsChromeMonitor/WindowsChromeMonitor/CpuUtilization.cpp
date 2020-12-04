#include "CpuUtilization.h"

#include <Windows.h>
#include <thread>
#include <unordered_map>
#include <algorithm>

#include "Exceptions.h"
#include "win32_utils.h"

ProcessId locate_process_by_cpu_utilizing_over_time(const std::string& process_name, std::chrono::seconds locate_time) {
	std::unordered_map<ProcessId, size_t> process_with_max_cpu_usage_frequency_count;
	
	auto starting_time = std::chrono::system_clock::now();
	do {
		ProcessId process_id = locate_process_by_cpu_utilizing(process_name);
		if (process_with_max_cpu_usage_frequency_count.find(process_id) == process_with_max_cpu_usage_frequency_count.end()) {
			process_with_max_cpu_usage_frequency_count[process_id] = 0;
		}
		else {
			process_with_max_cpu_usage_frequency_count[process_id] += 1;
		}
	} while (std::chrono::duration<double>(std::chrono::system_clock::now() - starting_time).count() < locate_time.count());
	auto max_element = std::max_element(process_with_max_cpu_usage_frequency_count.begin(),
		process_with_max_cpu_usage_frequency_count.end(),
		[](const std::pair<ProcessId, size_t>& p1, const std::pair<ProcessId, size_t>& p2) {return p1.second < p2.second;});
	return max_element->first;
}

ProcessId locate_process_by_cpu_utilizing(const std::string& process_name) {
	auto processes_id = win32_utils::get_processes_pid_by_name(process_name);
	ProcessCpuUsage max_process_cpu_usage = {0, 0};
	for (const auto process_id : processes_id) {
		try {
			double process_utilization = get_process_cpu_utilization_percentage(process_id);
			if (max_process_cpu_usage.cpu_usage < process_utilization) {
				max_process_cpu_usage.cpu_usage = process_utilization;
				max_process_cpu_usage.pid = process_id;
			}
		}
		catch (const OpenProcessException&) {
			// Occurs when process is died between finding the process id and when opening the actual handle (Race)
		}
	}
	return max_process_cpu_usage.pid;
}

int64_t to_int64(FILETIME file_time) {
	return static_cast<int64_t>(file_time.dwHighDateTime) << 32 | file_time.dwLowDateTime;
}

/*
 Get two time system overall cpy usage and our target process cpu usage
 The cpu usage is counted from computer booting but our process doesn't exist from boot
 Therefore, we will take overall system cpu usage twice with some seconds diff
 After will take the two examinations of cpu usage we will calc the diff
 Now, we got the system overall cpu usage between these two examinations
 We will do the same for process cpu usage, two examinations and diff

 Lets say we took the first process and system overall usage examinations in time T1
 And lets say we took the second process and system overall usage examinations in time T2
 Now we can calculate the process cpu usage percentage between T1 - T2 by this formula:

 (T2SystemOverallCpuTime - T1SystemOverallCpuTime) / (T2ProcessOverallCpuTime - T1ProcessOverallCpuTime) * 100
 */
double get_process_cpu_utilization_percentage(ProcessId pid, std::chrono::seconds seconds) {
	auto process_handle = win32_utils::open_process_by_id(pid, PROCESS_QUERY_INFORMATION);
	win32_utils::ProcessTimes first_process_times = win32_utils::get_process_times(process_handle.get());
	win32_utils::SystemTimes first_system_times = win32_utils::get_system_times();

	// Examine the T1 process and system time
	uint64_t first_process_overall_cpu_usage = to_int64(first_process_times.kernel_mode_time) + to_int64(
		first_process_times.user_mode_time);
	uint64_t first_system_overall_cpu_usage = to_int64(first_system_times.kernel_mode_time) + to_int64(
		first_system_times.user_mode_time);

	// Wait some seconds between each examinations
	std::this_thread::sleep_for(seconds);

	// Examine the T2 process and system time
	win32_utils::ProcessTimes second_process_times = win32_utils::get_process_times(process_handle.get());
	win32_utils::SystemTimes second_system_times = win32_utils::get_system_times();

	/*
	 Calculate the process cpu usage percentage by the formula mentioned above
	 */
	uint64_t second_process_overall_cpu_usage = to_int64(second_process_times.kernel_mode_time) + to_int64(
		second_process_times.user_mode_time);
	uint64_t second_system_overall_cpu_usage = to_int64(second_system_times.kernel_mode_time) + to_int64(
		second_system_times.user_mode_time);

	auto overall_system_diff = static_cast<double>(second_system_overall_cpu_usage - first_system_overall_cpu_usage);
	auto overall_process_diff = static_cast<double>(second_process_overall_cpu_usage - first_process_overall_cpu_usage);

	return overall_process_diff / overall_system_diff * 100;
}
