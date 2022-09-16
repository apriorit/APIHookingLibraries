______________________________________________________________________________________________________________

* [About Library](#aboutdeviarelibrary)
* [Content of project](#contentofproject)
* [Usage](#usage)
* [Expected result](#expectedresult)
______________________________________________________________________________________________________________

# About Deviare Library
*Deviare In-Proc* is a code interception engine for Microsoft Windows® developed by Nektra Advanced Computing.
This library is at the core of our Deviare v2.0 and SpyStudio Application Monitor technologies.
Deviare is a professional hooking engine for instrumenting arbitrary Win32 functions, COM objects, and functions whose symbols are located in program databases (PDBs). 
It can intercept unmanaged code in 32-bit and 64-bit applications. It is implemented as a COM component, so it can be integrated with all the programming languages which support COM, such as C/C++, VB, C#, Delphi, and Python.
The library [GitHub Page](https://github.com/nektra/Deviare2).

# Content of project
1. DeviareHideFilePlugin - The project contains the source code for building the DLL library that hides files with the "+\*.txt" name pattern. The library can be injected into the "Explorer.exe" process.

2. InjectDLL 			  - The project contains the source code for building the executable program that injects DLL into the "Explorer.exe" process to hide files with the "+\*.txt" name pattern.

# Usage
**To test solution:**
1. Clone the project and build it.
2. Navigate to the output folder.
3. Determine the PID of the "Explorer.exe" process (For example, you can do this by Process Explorer).
4. Run the executable program from CMD using such command:
```
	>InjectDLL_x64.exe 20676
```

# Expected result
The specified process will be restarted, and the DLL will be injected into it. The DLL will intercept all NtQueryDirectoryFile function calls.
As a result, the files with the "+\*.txt" filename pattern will not be shown to the user.
