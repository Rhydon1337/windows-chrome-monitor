#include "win32_utils.h"

#include <tlhelp32.h>

#include "Exceptions.h"

using namespace win32_utils;

PROCESSENTRY32 get_first_process_from_running_processes_snapshot(HANDLE snapshot) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &pe32)) {
		throw Process32FirstException("Get first process failed, error code: " + std::to_string(GetLastError()));
	}
	return pe32;
}

std::vector<ProcessId> win32_utils::get_processes_pid_by_name(const std::string& processes_name) {
	std::vector<ProcessId> processes_pid;
	const auto snapshot = get_running_processes_snapshot();
	PROCESSENTRY32 pe32 = get_first_process_from_running_processes_snapshot(snapshot.get());
	do {
		if (processes_name == pe32.szExeFile) {
			processes_pid.push_back(pe32.th32ProcessID);
		}
	}
	while (Process32Next(snapshot.get(), &pe32));
	return processes_pid;
}

AutoHandle win32_utils::get_running_processes_snapshot() {
	AutoHandle current_running_processes(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), &CloseHandle);
	if (INVALID_HANDLE_VALUE == current_running_processes.get()) {
		throw CreateRunningProcessesSnapshotException(
			"Create processes snapshot failed, error code: " + std::to_string(GetLastError()));
	}
	return current_running_processes;
}

AutoHandle win32_utils::open_process_by_id(ProcessId pid, DWORD permissions) {
	AutoHandle process_handle(OpenProcess(permissions, FALSE, pid), &CloseHandle);
	if (nullptr == process_handle.get()) {
		throw OpenProcessException("Open processes failed, error code: " + std::to_string(GetLastError()));
	}
	return process_handle;
}

ProcessTimes win32_utils::get_process_times(HANDLE process_handle) {
	ProcessTimes process_times = {0};
	if (!GetProcessTimes(process_handle, &process_times.creation_time, &process_times.exit_time,
	                     &process_times.kernel_mode_time, &process_times.user_mode_time)) {
		throw GetProcessTimesException("Get process times failed, error code: " + std::to_string(GetLastError()));
	}
	return process_times;
}

SystemTimes win32_utils::get_system_times() {
	SystemTimes system_times = { 0 };
	if (!GetSystemTimes(&system_times.idle_time, &system_times.kernel_mode_time, &system_times.user_mode_time)) {
		throw GetProcessTimesException("Get system times failed, error code: " + std::to_string(GetLastError()));
	}
	return system_times;
}
