#include <Windows.h>

#include "CpuUtilization.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    get_target_process_by_cpu_utilizing("chrome.exe");
    return true;
}