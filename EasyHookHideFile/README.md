______________________________________________________________________________________________________________

* [About Library](#abouteasyhooklibrary)
* [Content of project](#contentofproject)
* [Usage](#usage)
* [Expected result](#expectedresult)
______________________________________________________________________________________________________________

# About EasyHook Library
*EasyHook* makes it possible to extend (via hooking) unmanaged code APIs with pure managed functions from within a fully managed environment on 32- or 64-bit Windows XP SP2, Windows Vista x64, Windows Server 2008 x64, Windows 7, Windows 8.1, and Windows 10.
EasyHook supports injecting assemblies built for .NET Framework 3.5 / 4.0+ as well as native DLLs.
The library [GitHub Page](https://github.com/EasyHook/EasyHook).

# Content of project
1. EasyHookHideFilePlugin - The project contains the source code for building the DLL library that hides files with the "+\*.txt" name pattern. The library can be injected into the "Explorer.exe" process.

2. InjectDLL 			   - The project contains the source code for building the executable program that injects DLL into the "Explorer.exe" process to hide files with the "+\*.txt" name pattern.

# Usage
**To test solution:**
1. Clone the project and build it.
2. Navigate to the output folder.
3. Determine the PID of the "Explorer.exe" process (For example, you can do this by Process Explorer).
4. Run the executable program using such command:
```
	>InjectDLL_x64.exe 20676
```

# Expected result
The DLL will be injected into specified process. The DLL will intercept all NtQueryDirectoryFile function calls.
As a result, the files with the "+\*.txt" filename pattern will not be shown to the user.
