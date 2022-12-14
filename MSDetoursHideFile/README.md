______________________________________________________________________________________________________________

* [About Library](#aboutmicrosoftdetourslibrary)
* [Content of project](#contentofproject)
* [Usage](#usage)
* [Expected result](#expectedresult)
______________________________________________________________________________________________________________

# About Microsoft Detours Library
*Detours* is a software package for monitoring and instrumenting API calls on Windows. Detours has been used by many ISVs and is also used by product teams at Microsoft.
Detours makes it possible to add debugging instrumentation and to attach arbitrary DLLs to any existing Win32 binary.
The library [GitHub Page](https://github.com/microsoft/Detours).

# Content of project
1. MSDetoursHideFile - The project contains the source code for building the DLL library that hides files with the "+\*.txt" name pattern. The library can be injected into the "Explorer.exe" process.

2. InjectDLL 		  - The project contains the source code for building the executable program that injects DLL into the "Explorer.exe" process to hide files with the "+\*.txt" name pattern.

# Usage
**To test solution for Detours Lib:**
1. Clone the project and build it.
2. Build samples from MS Detours Library.
3. Move the builded "withdll.exe" execution file to the folder with the DLL file you want to inject.
4. Run the "withdll.exe" execution file with the filename of the DLL that you want to inject, and the filepath for the process that you want to inject DLL into it.
    For example, you can use the such command:
```
	>withdll.exe /d:MSDetoursHideFileWin32.dll "c:\windows\explorer.exe"
```

# Expected result
The specified process will be restarted, and the DLL will be injected into it. The DLL will intercept all NtQueryDirectoryFile function calls.
As a result, the files with the "+\*.txt" filename pattern will not be shown to the user.
*NOTE: The test was performed on Windows 10 x86*