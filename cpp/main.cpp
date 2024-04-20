#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#include <iostream>
#include <windows.h>
#include <string>

void clrScr() {
    system("cls");
}

bool getProxySettings(std::string& proxyServer) {
    HKEY hKey;
    DWORD proxyEnable;
    DWORD dataSize = sizeof(proxyEnable);
    bool isProxyEnabled = false;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, TEXT("ProxyEnable"), NULL, NULL, reinterpret_cast<LPBYTE>(&proxyEnable), &dataSize) == ERROR_SUCCESS) {
            isProxyEnabled = proxyEnable > 0;
        }
        if (isProxyEnabled) {
            char buffer[255];
            dataSize = sizeof(buffer);
            if (RegQueryValueEx(hKey, TEXT("ProxyServer"), NULL, NULL, reinterpret_cast<LPBYTE>(&buffer), &dataSize) == ERROR_SUCCESS) {
                proxyServer = std::string(buffer);
            }
        }
        RegCloseKey(hKey);
    }
    return isProxyEnabled;
}

void setProxy(bool enable, const std::string& proxyServer) {
    HKEY hKey;
    DWORD proxyEnable = enable ? 1 : 0;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, TEXT("ProxyEnable"), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&proxyEnable), sizeof(proxyEnable));
        if (enable) {
            RegSetValueEx(hKey, TEXT("ProxyServer"), 0, REG_SZ, reinterpret_cast<const BYTE*>(proxyServer.c_str()), proxyServer.length() + 1);
        }
        RegCloseKey(hKey);
    }
}

int main() {
    clrScr();
    std::string proxyServer;
    bool proxyEnabled = getProxySettings(proxyServer);
    std::cout << "Proxy Changer by Abdul Rehman \n@festerduck" << std::endl;
    std::cout << "**************************************************************************************" << std::endl;
    if (proxyEnabled) {
        std::cout << "\tCurrent proxy is\t[" << proxyServer << "]\n";
    } else {
        std::cout << "\nNo proxy is set..\n";
    }
    std::cout << "\nSet the proxy: [Default: 172.30.10.11:3128]?\n\n[1] Enable Proxy\t[2] Disable Proxy\t[3] Change Proxy Server\n\n";
    std::cout << "**************************************************************************************" << std::endl;
    std::cout << "\n$ ";
    std::string opt;
    std::getline(std::cin, opt);
    std::string defaultProxy = "http://172.30.10.11:3128";

    if (opt.empty()) {
        setProxy(true, defaultProxy);
    } else if (opt == "1") {
        setProxy(true, defaultProxy);
    } else if (opt == "2") {
        setProxy(false, "");
    } else if (opt == "3") {
        std::string newProxy;
        std::cout << "Enter new proxy server: ";
        std::getline(std::cin, newProxy);
        setProxy(true, newProxy);
    } else {
        std::cout << "Invalid input!\n";
    }

    proxyEnabled = getProxySettings(proxyServer);
    if (proxyEnabled) {
        std::cout << "Proxy has been enabled. [" << proxyServer << "]\n";
    } else {
        std::cout << "Proxy has been disabled\n";
    }

    std::cout << "\nPress any key to exit...";
    std::cin.get();

    return 0;
}
