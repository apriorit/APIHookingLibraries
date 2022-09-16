#include <iostream>
#include <string>

//Allow calls to the EasyHook library-------------------------
#include <easyhook.h>
//-----------------------------------------------------------

int main(__in int argc, __in wchar_t* argv[])
{
	if (argc < 2)
	{
		std::wcout << L"Use: InjectDLL_(Platform).exe Process-ID" << std::endl;
		return 1;
	}

	// Trying to parse the Process ID
	const DWORD dwPid = _wtoi(argv[1]);
	if (dwPid == 0 || dwPid == INT_MAX || dwPid == INT_MIN)
	{
		char errmsg[80];
		std::wcout << L"Error: Unable to parse \"Process-ID\" : " << strerror_s(errmsg, 80, errno) << std::endl;
		std::wcout << L"Use: InjectDLL_(Platform).exe Process-ID, where Process-ID is a digit" << std::endl;
		return 1;
	}

	if (dwPid == GetCurrentProcessId())
	{
		std::wcout << L"Error: Cannot inject a dll into Process-ID." << std::endl;
		return 1;
	}
	
	NTSTATUS result = 0;
#if _WIN64
	const WCHAR* dllToInjectNameW = L"EasyHookHideFilePlugin_x64.dll";
	// Inject dllToInject into the target process Id
	result = RhInjectLibrary(
		dwPid,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		NULL, // 32-bit
		const_cast<WCHAR*>(dllToInjectNameW), // 64-bit
		NULL, // data to send to injected DLL entry point
		0 // size of data to send
	);
#else
	const WCHAR* dllToInjectNameW = L"EasyHookHideFilePlugin_Win32.dll";
	// Inject dllToInject into the target process Id
	result = RhInjectLibrary(
		dwPid,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		const_cast<WCHAR*>(dllToInjectNameW), // 32-bit
		NULL, // 64-bit
		NULL, // data to send to injected DLL entry point
		0 // size of data to send
	);
#endif

	if (result != 0)
	{
		printf("Error: RhInjectLibrary failed with error code = %d\n", result);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
		return 1;
	}

	std::wcout << L"InjectDLL.exe: Library injected successfully.\n";
	return 0;
}
