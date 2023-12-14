#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4096

void parentProcess(HANDLE hMapFile, LPCTSTR pBuf) {
    // Writing to shared memory
    strcpy((char*)pBuf, "Hello, child process!");

    printf("Parent process wrote to shared memory: %s\n", (char*)pBuf);

    // Unmap and close handles
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

void childProcess(HANDLE hMapFile, LPCTSTR pBuf) {
    // Simulating some work in the child process
    Sleep(1000);

    // Reading from shared memory
    printf("Child process reads: %s\n", (char*)pBuf);

    // Unmap and close handles
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    exit(EXIT_SUCCESS);
}

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;

    // Create file mapping
    hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZE, L"SharedMemory");
    if (hMapFile == NULL) {
        perror("CreateFileMapping");
        exit(EXIT_FAILURE);
    }

    // Map view of the file into memory
    pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SIZE);
    if (pBuf == NULL) {
        perror("MapViewOfFile");
        CloseHandle(hMapFile);
        exit(EXIT_FAILURE);
    }

    DWORD pid = GetCurrentProcessId();

    if (pid == 0) {
        // Child process logic
        childProcess(hMapFile, pBuf);
    } else {
        // Parent process logic
        parentProcess(hMapFile, pBuf);
    }

    return 0;
}
