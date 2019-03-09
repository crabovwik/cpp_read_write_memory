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

    HANDLE process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (process_handle == nullptr) {
        std::cout << "Couldn't get a handle of the process" << std::endl;
        return 1;
    }

    std::cout << "The handle of the process has been found. It's placed at " << process_handle << std::endl;

    LPVOID write_address = (void *) 0x258B693C9C0;
    LPCVOID write_buffer = L"what a beautiful day";
    SIZE_T write_buffer_size = (SIZE_T) wcslen((WCHAR *) write_buffer) * sizeof(WCHAR);
    SIZE_T count_of_written_bytes = 0;
    bool write_result = WriteProcessMemory(process_handle, write_address, write_buffer, write_buffer_size, &count_of_written_bytes);
    if (write_result == false) {
        std::cout << "Something has gone wrong in writing process" << std::endl;
        return 1;
    }

    std::cout << "I have just written the next data: " << write_buffer << std::endl;
    std::cout << "A count of written bytes is: " << count_of_written_bytes << std::endl;

    return 0;
}
