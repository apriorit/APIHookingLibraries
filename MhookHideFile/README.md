______________________________________________________________________________________________________________

* [About Library](#aboutmhooklibrary)
* [Content of project](#contentofproject)
* [Usage](#usage)
______________________________________________________________________________________________________________

# About MHook Library
*Mhook* is an open source API hooking library for intercepting function calls (setting hooks). In other words, it's a library for embedding code into application threads. 
This library comes in handy when you need to monitor and log system function calls or run your own code before, after, or instead of a system call.
The library [GitHub Page](https://github.com/apriorit/mhook).

# Content of project
1. MhookHideFile - The project contains the source code for building the DLL library that hides files with the "+\*.txt" name pattern. The library can be injected into the "Explorer.exe" process.

# Usage
**To test solution:**
Clone the project and build it. To test this solution, I used the *AppInit_DLLs infrastructure*. The AppInit_DLLs infrastructure loads a predefined set of DLLs to all user-mode processes connected with the User32.dll library.
To set up the AppInit_DLLs infrastructure, you can visit [Windows API Hooking Tutorial](https://www.apriorit.com/dev-blog/160-apihooks) or [AppInit DLLs](https://pentestlab.blog/2020/01/07/persistence-appinit-dlls/).
*NOTE: "Starting in Windows 8, the AppInit_DLLs infrastructure is disabled when secure boot is enabled.".*
