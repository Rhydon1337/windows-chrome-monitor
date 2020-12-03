#include "CpuUtilization.h"

#include <Windows.h>
#include <chrono>
#include <thread>


#include "win32_utils.h"

int64_t to_int64(FILETIME filetime) {
	return static_cast<int64_t>(filetime.dwHighDateTime) << 32 | filetime.dwLowDateTime;
}

double get_process_cpu_utilization_percentage(win32_utils::ProcessId pid, size_t seconds) {
	auto process_handle = win32_utils::open_process_by_id(pid, PROCESS_QUERY_INFORMATION);
	win32_utils::ProcessTimes first_process_times = win32_utils::get_process_times(process_handle.get());
	win32_utils::SystemTimes first_system_times = win32_utils::get_system_times();

	uint64_t first_process_overall_cpu_usage = to_int64(first_process_times.kernel_mode_time) + to_int64(
		first_process_times.user_mode_time);
	uint64_t first_system_overall_cpu_usage = to_int64(first_system_times.kernel_mode_time) + to_int64(
		first_system_times.user_mode_time);

	std::this_thread::sleep_for(std::chrono::seconds(seconds));

	win32_utils::ProcessTimes second_process_times = win32_utils::get_process_times(process_handle.get());
	win32_utils::SystemTimes second_system_times = win32_utils::get_system_times();

	uint64_t second_process_overall_cpu_usage = to_int64(second_process_times.kernel_mode_time) + to_int64(
		second_process_times.user_mode_time);
	uint64_t second_system_overall_cpu_usage = to_int64(second_system_times.kernel_mode_time) + to_int64(
		second_system_times.user_mode_time);

	auto overall_system_diff = static_cast<double>(second_system_overall_cpu_usage - first_system_overall_cpu_usage);
	auto overall_process_diff = static_cast<double>(second_process_overall_cpu_usage - first_process_overall_cpu_usage);

	return overall_process_diff / overall_system_diff * 100;
}

#include "Exceptions.h"

win32_utils::ProcessId get_target_process_by_cpu_utilizing(const std::string& process_name) {
	while (true) {
		auto processes_id = win32_utils::get_processes_pid_by_name(process_name);
		for (const auto process_id : processes_id) {
			try {
				double process_utilization = get_process_cpu_utilization_percentage(process_id, 1);
				OutputDebugStringA(("pid: " + std::to_string(process_id) + " " + std::to_string(process_utilization) + "\n").c_str());
			}
			catch (const OpenProcessException&) {
				// Occurs when process is died between finding all process and opening the actual handles
			}
		}
		Sleep(500);
	}
}
