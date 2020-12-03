#pragma once
#include <stdexcept>

#define DEF_EXCEPTION(exception_name, inherit_from) \
	class exception_name : public inherit_from { \
	public: \
		explicit exception_name(const std::string& _Message)\
			: inherit_from(_Message) {} \
		explicit exception_name(const char* _Message) \
			: inherit_from(_Message) {} \
	}
	
	
// Main Exception
DEF_EXCEPTION(WindowsChromeMonitorException, std::runtime_error);

// Win32 Exceptions
DEF_EXCEPTION(Win32Exception, WindowsChromeMonitorException);
DEF_EXCEPTION(CreateRunningProcessesSnapshotException, Win32Exception);
DEF_EXCEPTION(Process32FirstException, Win32Exception);
