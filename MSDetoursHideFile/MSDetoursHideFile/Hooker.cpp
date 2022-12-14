#include "pch.h"
#include "Hooker.h"

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

void HookFunction()
{
    DetourRestoreAfterWith();

    printf("MSDetoursHideFile.dll:Starting.\n");
    fflush(stdout);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueNtQueryDirectoryFile, HookNtQueryDirectoryFile);
    const LONG error = DetourTransactionCommit();

    if (error == NO_ERROR) 
    {
        printf("MSDetoursHideFile.dll: Detoured NtQueryDirectoryFile().\n");
    }
    else 
    {
        printf("MSDetoursHideFile.dll: Error detouring NtQueryDirectoryFile(): %ld\n", error);
    }
}

void UnhookFunction()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TrueNtQueryDirectoryFile, HookNtQueryDirectoryFile);
    const LONG error = DetourTransactionCommit();

    printf("MSDetoursHideFile.dll: Removed NtQueryDirectoryFile() (result=%ld)\n", error);

    fflush(stdout);
}
