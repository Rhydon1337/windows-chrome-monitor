#include <Windows.h>

#include "CpuUtilization.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    locate_process_by_cpu_utilizing_over_time("chrome.exe", std::chrono::seconds(2));
    return true;
}