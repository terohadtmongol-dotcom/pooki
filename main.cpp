#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include <atomic>
#include <memory>
#include <fstream>
#include <Lmcons.h> 

#include "Sources/overlay.h"
#include "Loading/driver.h"
#include "Loading/map.h"
#include "Sources/slowdown.h"
#include "Loading/detect.h"

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
UINT g_ResizeWidth = 0;
UINT g_ResizeHeight = 0;

KeyAuth::api* keyAuthApp = nullptr;

inline const std::string& GetDriverPath() {
    static std::string s = "C:\\Windows\\System32\\Tasks\\kernel.sys";
    return s;
}

inline const std::string& GetMapperPath() {
    static std::string s = "C:\\Windows\\System32\\Tasks\\mapper.exe";
    return s;
}

std::string GetPCUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    }
    return "User";
}

void TypeText(const std::string& text, int delayMs = 25) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void PrintLine(const std::string& text, int delayMs = 25) {
    TypeText(text, delayMs);
    std::cout << std::endl;
}

bool KeyAuthA() {
    PrintLine("[+] Initializing System...", 30);

    std::string name = OBF("4noks");
    std::string ownerid = OBF("U4RGtggNwe");
    std::string version = OBF("1.0");
    std::string url = OBF("https://keyauth.win/api/1.3/");
    std::string path = OBF("");

    keyAuthApp = new KeyAuth::api(name, ownerid, version, url, path);
    keyAuthApp->init();

    if (!keyAuthApp->response.success) {
        PrintLine("[-] Authentication failed: " + keyAuthApp->response.message, 30);
        return false;
    }

    PrintLine("[+] Connected to authentication server!", 30);
    return true;
}

bool Login() {
    std::string username, password;
    
    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);
    keyAuthApp->login(username, password);

    if (!keyAuthApp->response.success) {
        PrintLine("[-] Login failed: " + keyAuthApp->response.message, 30);
        return false;
    }
    PrintLine("[+] Login successful!", 30);

    if (!AntiDebug::PerformInitialChecks()) {
        return false;
    }

    return true;
}

bool Register() {
    std::string username, password, license;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "License key: ";
    std::getline(std::cin, license);

    keyAuthApp->regstr(username, password, license);

    if (!keyAuthApp->response.success) {
        PrintLine("[-] Registration failed: " + keyAuthApp->response.message, 30);
        return false;
    }

    PrintLine(OBF("[+] Registration successful!"), 30);

    if (!AntiDebug::PerformInitialChecks()) {
        return false;
    }

    return true;
}

bool LoadDriver() {
    PrintLine(OBF("[+] Checking for Libary..."));

    if (kernel->initialize_handle()) {
        PrintLine(OBF("[+] Library found!"));
        return true;
    }
    PrintLine(OBF("[+] Library not found, loading..."));

    std::ofstream driverFile(GetDriverPath(), std::ios::binary);
    if (!driverFile.write(reinterpret_cast<const char*>(Driver), sizeof(Driver))) {
        return false;
    }
    driverFile.close();

    std::ofstream mapperFile(GetMapperPath(), std::ios::binary);
    if (!mapperFile.write(reinterpret_cast<const char*>(Mapper), sizeof(Mapper))) {
        std::filesystem::remove(GetDriverPath());
        return false;
    }
    mapperFile.close();

    std::string commandLine = GetMapperPath() + " " + GetDriverPath();

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(nullptr, const_cast<LPSTR>(commandLine.c_str()), nullptr, nullptr,
        FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        std::filesystem::remove(GetDriverPath());
        std::filesystem::remove(GetMapperPath());
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::filesystem::remove(GetDriverPath());
    std::filesystem::remove(GetMapperPath());

    if (!kernel->initialize_handle()) {
        PrintLine(OBF("[-] Failed to load Library!"));
        return false;
    }
    PrintLine(OBF("[+] Library loaded successfully!"));
    return true;
}

int main()
{
    std::string pcUsername = GetPCUsername();
    PrintLine(OBF("[+] Welcome back, ") + pcUsername, 30);
	Sleep(500);
    AntiDebug::StartMonitoring();

    if (!KeyAuthA()) {
        system("pause");
        return 1;
    }
    int choice = 0;
    std::cout << "\n[1] Login\n[2] Register\n\nSelect: ";
    std::cin >> choice;
    std::cin.ignore();

    bool authSuccess = false;

    if (choice == 1) {
        authSuccess = Login();
    }
    else if (choice == 2) {
        authSuccess = Register();
    }
    else {
        PrintLine(OBF("[-] Invalid selection"), 30);
        system("pause");
        return 1;
    }

    if (!authSuccess) {
        system("pause");
        return 1;
    }
    PrintLine(OBF("[+] Initializing System..."), 50);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (!LoadDriver()) {
        PrintLine(OBF("[-] Library loading failed!"));
        system("pause");
        return 1;
    }

    std::wstring target = L"FortniteClient-Win64-Shipping.exe";
    int pid = 0;

    PrintLine(OBF("[+] Waiting for Fortnite..."), 30);

    int attempts = 0;
    while (!pid) {
        if (AntiDebug::IsDebugged()) {
            AntiDebug::BanLicense("Debugger detected while waiting for Fortnite");
            return 1;
        }
        pid = kernel->get_process_pid(target);
        if (pid) {
            PrintLine("[+] Fortnite found!");
            break;
        }

        attempts++;
        if (attempts % 5 == 0) {
            TypeText(".", 200);
        }
        Sleep(500);
    }

    std::cout << std::endl;

    if (!kernel->attach(pid)) {
        PrintLine(OBF("[-] Failed to attach to process!"));
        system("pause");
        return 1;
    }
    auto baseAddress = kernel->get_image_base(nullptr);
    if (!baseAddress) {
        system("pause");
        return 1;
    }
    kernel->BaseAddress = baseAddress;

    if (!kernel->dtb) {
        system("pause");
        return 1;
    }
    if (AntiDebug::IsDebugged()) {
        AntiDebug::BanLicense("Debugger detected");
        return 1;
    }
    overlay::start();

    system("pause");
    return 0;
}