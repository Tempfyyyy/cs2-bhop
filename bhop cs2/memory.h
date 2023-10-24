#pragma once

// Include the necessary libraries
#include <windows.h>
#include <TlHelp32.h>

// Define WIN32_LEAN_AND_MEAN to exclude rarely-used services from windows.h
#define WIN32_LEAN_AND_MEAN

// Namespace for variables and functions related to process and memory management
namespace VARS {
    // Function to get the process ID of a target process
    DWORD GetProcess(const wchar_t* Target) {
        // Create a snapshot of all processes
        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return NULL;
        }

        PROCESSENTRY32W processEntry = { };
        processEntry.dwSize = sizeof(PROCESSENTRY32W);

        // Iterate over all processes in the snapshot
        if (Process32FirstW(snapshotHandle, &processEntry)) {
            do {
                // If the process name matches the target, return its ID
                if (_wcsicmp(processEntry.szExeFile, Target) == 0) {
                    CloseHandle(snapshotHandle);
                    return processEntry.th32ProcessID;
                }
            } while (Process32NextW(snapshotHandle, &processEntry));
        }

        CloseHandle(snapshotHandle);
        return NULL;
    }

    // Function to get the base address of a module within a process
    uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget) {
        // Create a snapshot of all modules within the process
        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return NULL;
        }

        MODULEENTRY32W moduleEntry = { };
        moduleEntry.dwSize = sizeof(MODULEENTRY32W);

        // Iterate over all modules in the snapshot
        if (Module32FirstW(snapshotHandle, &moduleEntry)) {
            do {
                // If the module name matches the target, return its base address
                if (_wcsicmp(moduleEntry.szModule, ModuleTarget) == 0) {
                    CloseHandle(snapshotHandle);
                    return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                }
            } while (Module32NextW(snapshotHandle, &moduleEntry));
        }

        CloseHandle(snapshotHandle);
        return NULL;
    }

    // Get the process ID of "cs2.exe"
    DWORD processId = GetProcess(L"cs2.exe");

    // Get the base address of "client.dll" within the cs2.exe process
    uintptr_t baseAddress = GetModuleBaseAddress(processId, L"client.dll");

    // Open a handle to the cs2.exe process with all possible access rights
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    // Template function to read memory from a static pointer within a process
    template <typename type>
    type memRead(uintptr_t pointerStatic) {
        type value = { };
        ReadProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
        return value;
    }

    // Template function to write memory to a static pointer within a process
    template <typename type>
    bool memWrite(uintptr_t pointerStatic, type value) {
        return WriteProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
    }
}
