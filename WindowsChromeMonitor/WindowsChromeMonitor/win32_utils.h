#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <string>

using ProcessId = size_t;

namespace win32_utils {

	using AutoHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&CloseHandle)>;

	struct SystemTimes {
		FILETIME idle_time;
		FILETIME kernel_mode_time;
		FILETIME user_mode_time;
	};

	struct ProcessTimes {
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_mode_time;
		FILETIME user_mode_time;
	};

	/**
	 * @brief get list of processes pid matching to the wanted processes name
	 * @param[in] processes_name - the processes name (one process might has some instances e.g chrome.exe)
	 * @return list of processes pid
	 */
	std::vector<ProcessId> get_processes_pid_by_name(const std::string& processes_name);

	/**
	 * @brief get the current running processes snapshot
	 * @return handle to the snapshot
	 */
	AutoHandle get_running_processes_snapshot();

	/**
	 * @brief open an handle to running process by its process id
	 * @param[in] pid - the process id
	 * @param permissions - the open process handle needed permission
	 * @return handle to the open process
	 */
	AutoHandle open_process_by_id(ProcessId pid, DWORD permissions);
	
	/**
	 * @brief get process_handle cpu utilizing
	 * @param[in] process_handle - handle to the process_handle
	 * @return process_handle cpu utilizing
	 */
	ProcessTimes get_process_times(HANDLE process_handle);

	/**
	 * @brief get overall system cpu utilizing
	 * @return overall system cpu utilizing
	 */
	SystemTimes get_system_times();
}