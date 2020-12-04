#include <Windows.h>

#include <thread>

#include "CpuUtilization.h"

constexpr auto PROCESS_LOCATE_TIME = std::chrono::minutes(5);
constexpr double MINIMUM_CPU_USAGE = 1.2;
const std::string TARGET_PROCESS = "chrome.exe";

const auto SLEEP_TIME_BETWEEN_EXAMINATIONS = std::chrono::minutes(1);

// This time represent the enough time that the process is off and we can shutdown the system
const auto MINIMAL_TIME_THAT_PROCESS_IS_OFF_TO_SHUTDOWN = std::chrono::minutes(5);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	ProcessId target_process_id = locate_process_by_cpu_utilizing_over_time(TARGET_PROCESS, PROCESS_LOCATE_TIME);
	auto last_time_the_was_running = std::chrono::system_clock::now();
	while (std::chrono::duration_cast<std::chrono::minutes>(
		std::chrono::system_clock::now() - last_time_the_was_running).count() <
		MINIMAL_TIME_THAT_PROCESS_IS_OFF_TO_SHUTDOWN.count()) {
		double cpu_usage = get_process_cpu_utilization_percentage(target_process_id);
		if (MINIMUM_CPU_USAGE < cpu_usage) {
			std::this_thread::sleep_for(SLEEP_TIME_BETWEEN_EXAMINATIONS);
			last_time_the_was_running = std::chrono::system_clock::now();
		}
		else {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	win32_utils::shutdown();
	return EXIT_SUCCESS;
}