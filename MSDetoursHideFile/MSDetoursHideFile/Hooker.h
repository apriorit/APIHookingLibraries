#pragma once
//Allow calls to the Detours library-------------------------
#include <detours.h>
//-----------------------------------------------------------

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
    IN           BOOLEAN                RestartScan
);
//-----------------------------------------------------------

void HookFunction();
void UnhookFunction();

//-----------------------------------------------------------
