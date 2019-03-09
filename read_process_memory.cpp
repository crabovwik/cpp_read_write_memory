#include "pch.h"
#include <iostream>
#include <Windows.h>

int main() {
    setlocale(LC_ALL, "Russian");

    DWORD pid = 0;

    LPCWSTR process_name = L"Безымянный — Блокнот";

    std::wcout << "Searching for the process with the name " << process_name << std::endl;

    HWND hwnd = FindWindowW(nullptr, process_name);
    if (hwnd == nullptr) {
        std::cout << "The process has been not found" << std::endl;
        return 1;
    }

    std::cout << "Process has been found!" << std::endl;
    std::cout << "It's placed at " << (void *) hwnd << std::endl;

    std::cout << "Getting process handle..." << std::endl;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == NULL) {
        std::cout << "Thread of the process has not been found" << std::endl;
        return 1;
    }

    HANDLE process_handle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (process_handle == nullptr) {
        std::cout << "Couldn't get a handle of the process" << std::endl;
        return 1;
    }

    std::cout << "The handle of the process has been found. It's placed at " << process_handle << std::endl;

    LPCVOID read_address = (void *) 0x258B6917450;
    LPVOID read_buffer = calloc(8, sizeof(wchar_t));
    SIZE_T read_size = sizeof(wchar_t) * 7;
    bool read_result = ReadProcessMemory(process_handle, read_address, read_buffer, read_size, nullptr);
    if (read_result == false) {
        std::cout << "Something has gone wrong in reading process" << std::endl;
        return 1;
    }

    *((wchar_t *) read_buffer + 7) = 0x0;

    std::cout << "I have just read the next data:" << std::endl;
    std::wcout << (wchar_t *) read_buffer << std::endl;

    return 0;
}
