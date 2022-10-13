#include <stdio.h>

#include "pebutil.h"

BOOL ValidateArguments(_In_ int argc, _In_ char** argv, _Out_ DWORD* pid)
{
    // First check that we have the correct number of arguments: The command and the PID of the process
    if(argc != 2)
    {
        printf("The number of arguments provided is invalid. There should only be one argument.\n");
        return 1;
    }

    // Now check that the PID is not any special cases
    int decimalBase = 10;
    DWORD temp_pid = strtoul(argv[1], NULL, decimalBase);
    if(temp_pid == 0 || errno == ERANGE)
    {
        return 1;
    }

    *pid = temp_pid;
    return 0;
}

void PrintUsage()
{
    printf("Usage: pebutil_test.exe [pid]\n");
    printf("- pid --- The pid of the target process to retrieve the PEB of.\n");
    return;
}

BOOL TestGetProcessPebAddress(DWORD pid)
{
    printf("===== GetProcessPebAddress Test =====\n");
    // Get a handle to the target process so we can get it's PEB information
    HANDLE hTargetProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if(hTargetProc == NULL)
    {
        printf("Failed to open target process to retrieve PEB location. Error: %d\n", GetLastError());
        CloseHandle(hTargetProc);   // Don't need this anymore so close it
        return 1;
    }

    // Grab the address of the target process's PEB
    PPEB targetProcPEBAddress = GetProcessPebAddress(hTargetProc);
    if(targetProcPEBAddress == NULL)
    {
        printf("Failed to get the target process PEB Address.Exiting...\n");
        return 1;
    }

    printf("Target process PEB Address: %p\n", targetProcPEBAddress);

    CloseHandle(hTargetProc);   // Don't need this anymore so close it
    printf("===============\n\n");
    return 0;
}

BOOL TestGetProcessPeb(DWORD pid)
{
    printf("===== GetProcessPeb Test =====\n");
    // Get a handle to the target process so we can get it's PEB information
    HANDLE hTargetProc = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, pid);
    if(hTargetProc == NULL)
    {
        printf("Failed to open target process to retrieve PEB. Error: %d\n", GetLastError());
        CloseHandle(hTargetProc);   // Don't need this anymore so close it
        return 1;
    }

    // Grab the target process's PEB
    PEB targetProcPEB = {0};
    BOOL retVal = GetProcessPeb(hTargetProc, &targetProcPEB);
    if(retVal == PEB_UTIL_FAILURE)
    {
        printf("Failed to get the target process PEB.Exiting...\n");
        return 1;
    }

    printf("Target process PEB num of processors: %lu\n", *(((BYTE*)&targetProcPEB) + 0xB8)); //sanity check that the copy worked

    CloseHandle(hTargetProc);   // Don't need this anymore so close it
    printf("==============\n\n");
    return 0;
}

BOOL TestGetCurrentPebAddress()
{
    printf("===== GetCurrentPebAddress Test =====\n");
    // Let's test grabbing our own current process's PEB
    PPEB currentPEB = GetCurrentPebAddress();
    if(currentPEB == NULL)
    {
        printf("Failed to get the current process PEB.Exiting...\n");
        return 1;
    }

    printf("Current process PEB Address: %p\n", currentPEB);
    wprintf(L"Commandline Param from PEB: %S\n", currentPEB->ProcessParameters->CommandLine.Buffer); // Sanity check that we correctly grabbed our PEB
    printf("===============\n\n");
    return 0;
}

BOOL TestGetCurrentPeb()
{
    printf("===== GetCurrentPeb Test =====\n");
    PEB pebCopy = GetCurrentPeb();
    printf("PEB Copy Address: %p\n", &pebCopy); // This should be different than the actual PEB address
    wprintf(L"Commandline param from PEB copy: %S\n", pebCopy.ProcessParameters->CommandLine.Buffer); //This should be the same as the actual PEB
    printf("===============\n\n");
    return 0;
}

int main(int argc, char** argv)
{
    DWORD pid = 0;  // Will contain the PID entered as the commandline arg
    if(ValidateArguments(argc, argv, &pid))
    {
        PrintUsage();
        return 1;
    }

    if(TestGetProcessPebAddress(pid))
    {
        return 1;
    }

    if(TestGetProcessPeb(pid))
    {
        return 1;
    }

    if(TestGetCurrentPebAddress())
    {
        return 1;
    }

    if(TestGetCurrentPeb())
    {
        return 1;
    }

    system("pause");
    return 0;
}

