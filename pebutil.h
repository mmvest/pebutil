#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>

#define PEB_UTIL_FAILURE 1
#define PEB_UTIL_SUCCESS 0

typedef NTSTATUS(WINAPI* fnNtQueryInformationProcess)(
    _In_        HANDLE           ProcessHandle,
    _In_        PROCESSINFOCLASS ProcessInformationClass,
    _Out_       PVOID            ProcessInformation,
    _In_        ULONG            ProcessInformationLength,
    _Out_opt_   PULONG           ReturnLength
);

PPEB    GetCurrentPebAddress();
PEB     GetCurrentPeb();
PPEB    GetProcessPebAddress(HANDLE hProc);
BOOL    GetProcessPeb(HANDLE hProc, PPEB outPeb);