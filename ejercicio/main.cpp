#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4096

void parentProcess(HANDLE hMapFile, LPCTSTR pBuf) {
    // Writing to shared memory
    for (int i = 0; i < 10; i++) {
        sprintf((char*)pBuf, "Number: %d", i);
        printf("Ruben wrote to shared memory: %s\n", (char*)pBuf);
        Sleep(1000); // Simulate some work
    }

    // Unmap and close handles
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

void childProcess(HANDLE hMapFile, LPCTSTR pBuf) {
    // Reading from shared memory
    for (int i = 0; i < 10; i++) {
        printf("Ruben cuenta hasta: %s\n", (char*)pBuf);
        Sleep(1000); // Simulate some work
    }

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