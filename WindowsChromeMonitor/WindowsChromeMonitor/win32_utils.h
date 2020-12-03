#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <string>

using ProcessId = size_t;
using AutoHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&CloseHandle)>;

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