#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

// Hàm kết thúc tiến trình theo tên
bool terminateProcessByName(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess == NULL) continue;
                if (TerminateProcess(hProcess, 0)) {
                    CloseHandle(hProcess);
                    CloseHandle(hSnapshot);
                    return true;
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return false;
}

// Hàm kết thúc tiến trình theo ID
bool terminateProcessByID(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == NULL) return false;
    bool result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <method> <name_or_id>\n";
        std::cerr << "Methods: name, id\n";
        return 1;
    }

    std::string method = argv[1];
    std::string target = argv[2];

    if (method == "name") {
        if (terminateProcessByName(target)) {
            std::cout << "Process terminated successfully.\n";
        } else {
            std::cerr << "Failed to terminate process.\n";
        }
    } else if (method == "id") {
        DWORD processID = std::stoul(target);
        if (terminateProcessByID(processID)) {
            std::cout << "Process terminated successfully.\n";
        } else {
            std::cerr << "Failed to terminate process.\n";
        }
    } else {
        std::cerr << "Invalid method. Use 'name' or 'id'.\n";
        return 1;
    }

    return 0;
}
