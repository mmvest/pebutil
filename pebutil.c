#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>

#include <stdio.h>

#include "pebutil.h"

/**
 * @brief Get the address of the PEB for the calling process
 * 
 * @return PPEB
 */
PPEB GetCurrentPebAddress()
{
    DWORD pebOffset = 0x60;
    return (PPEB) __readgsqword(pebOffset);
}

/**
 * @brief Get a copy of the calling process's PEB
 * 
 * @return PEB 
 */
PEB GetCurrentPeb()
{
    return *(GetCurrentPebAddress());
}

/**
 * @brief Get the address to a target process's PEB. *NOTE* the calling process will need PROCESS_QUERY_INFORMATION access
 * for the handle to the target process for this to work.
 * 
 * @param hProc - handle to the target process
 * @return PPEB 
 */
PPEB GetProcessPebAddress(HANDLE hProc)
{
    PROCESS_BASIC_INFORMATION procInfo = { 0 };
    static fnNtQueryInformationProcess NtQueryInformationProcess = NULL; //This is static so that repeated calls to this function won't require this function to be imported each time.

    if(NtQueryInformationProcess == NULL)
    {
        // Get handle to Ntdll.dll
        HANDLE hNtdll = GetModuleHandleA("Ntdll.dll");
        if(hNtdll == NULL)
        {
            printf("Failed to get handle for Ntdll.dll. Error: %d\n", GetLastError());
            return NULL;
        }

        NtQueryInformationProcess = (fnNtQueryInformationProcess) GetProcAddress(hNtdll, "NtQueryInformationProcess");
    }




    //Query the target process for the process basic information which contains the PEB
    NTSTATUS retVal = NtQueryInformationProcess(hProc, ProcessBasicInformation, (PVOID)&procInfo, sizeof(PROCESS_BASIC_INFORMATION), NULL);
    if(!NT_SUCCESS(retVal))
    {
        printf("Failed to query process information using NtQueryInformationProcess(). NTSTATUS Code: %08X\n", retVal);
        return NULL;
    }

    //Return the target process PEB base address
    return procInfo.PebBaseAddress;
}

/**
 * @brief Get a copy of the target process's PEB. *NOTE* the calling process will need PROCESS_QUERY_INFORMATION | PROCESS_VM_READ access
 * for the handle to the target process for this to work.
 * 
 * @param hProc - handle to the target process
 * @param outPeb - pointer to the PEB struct to receive the contents of the target process's PEB
 * @return BOOL
 */
BOOL GetProcessPeb(HANDLE hProc, PPEB outPeb)
{
    PEB tempPeb;
    BOOL retVal = ReadProcessMemory(hProc, GetProcessPebAddress(hProc), &tempPeb, sizeof(PEB), NULL);
    if(retVal == 0)
    {
        printf("Error reading target process memory. Error: %d\n", GetLastError());
        return PEB_UTIL_FAILURE;
    }

    *outPeb = tempPeb;
    return PEB_UTIL_SUCCESS;
}