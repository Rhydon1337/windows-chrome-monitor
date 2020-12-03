#include "win32_utils.h"

#include <tlhelp32.h>

#include "Exceptions.h"

PROCESSENTRY32 get_first_process_from_running_processes_snapshot(HANDLE snapshot) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &pe32)) {
		throw Process32FirstException("Get first process failed, error code: " + std::to_string(GetLastError()));
	}
	return pe32;
}

std::vector<ProcessId> get_processes_pid_by_name(const std::string& processes_name) {
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

AutoHandle get_running_processes_snapshot() {
	AutoHandle current_running_processes(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), &CloseHandle);
	if (INVALID_HANDLE_VALUE == current_running_processes.get()) {
		throw CreateRunningProcessesSnapshotException("Create processes snapshot failed, error code: " + std::to_string(GetLastError()));
	}
	return current_running_processes;
}
