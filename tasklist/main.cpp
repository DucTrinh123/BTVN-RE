#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>  // Thư viện hỗ trợ định dạng đầu ra

void ListProcesses()
{
    // Tạo một snapshot (ảnh chụp) của tất cả các tiến trình đang chạy.
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Lấy danh sách các tiến trình trong hệ thống.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Không thể tạo ảnh chụp các tiến trình." << std::endl;
        return;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32))
    {
        std::cerr << "Không thể lấy thông tin tiến trình đầu tiên." << std::endl;
        CloseHandle(hProcessSnap);  // Đóng handle ảnh chụp tiến trình.
        return;
    }

    std::wcout << std::left << std::setw(30) << L"Image Name"
               << std::setw(10) << L"PID" << std::endl;
    std::wcout << L"---------------------------------------------" << std::endl;

    do
    {
        std::wcout << std::left << std::setw(30) << pe32.szExeFile   // Đặt độ rộng 30 ký tự cho Image Name
                   << std::setw(10) << pe32.th32ProcessID << std::endl;  // Đặt độ rộng 10 ký tự cho PID
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

int main()
{
    ListProcesses();
    return 0;
}
