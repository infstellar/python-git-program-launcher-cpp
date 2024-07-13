#include "managers.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <Windows.h>

using namespace std;
namespace fs = filesystem;

bool startProxy() {
	string command = "\"" + (WORKING_FOLDER / "toolkit" / "fastgithub_win-x64" / "fastgithub.exe").string() + "\"" + " start";
	runCommand(command);
	return true;
}

bool stopProxy() {
    string command = "\"" + (WORKING_FOLDER / "toolkit" / "fastgithub_win-x64" / "fastgithub.exe").string() + "\"" + " stop";
    runCommand(command);
    return true;
}


//https://blog.csdn.net/u010745620/article/details/98471504
int isSysProxyEnabled(string* proxyAddress)
{
    HKEY key;
    auto ret = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_READ, &key);
    if (ret != ERROR_SUCCESS) {
        std::cout << "open failed: " << ret << std::endl;
        return -1;
    }

    DWORD values_count, max_value_name_len, max_value_len;
    ret = RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL,
        &values_count, &max_value_name_len, &max_value_len, NULL, NULL);
    if (ret != ERROR_SUCCESS) {
        std::cout << "query failed" << std::endl;
        return -1;
    }

    std::vector<std::tuple<std::shared_ptr<char>, DWORD, std::shared_ptr<BYTE>>> values;
    for (int i = 0; i < values_count; i++) {
        std::shared_ptr<char> value_name(new char[max_value_name_len + 1],
            std::default_delete<char[]>());
        DWORD value_name_len = max_value_name_len + 1;
        DWORD value_type, value_len;
        RegEnumValueA(key, i, value_name.get(), &value_name_len, NULL, &value_type, NULL, &value_len);
        std::shared_ptr<BYTE> value(new BYTE[value_len],
            std::default_delete<BYTE[]>());
        value_name_len = max_value_name_len + 1;
        RegEnumValueA(key, i, value_name.get(), &value_name_len, NULL, &value_type, value.get(), &value_len);
        values.push_back(std::make_tuple(value_name, value_type, value));
    }

    DWORD ProxyEnable = 0;
    for (auto x : values) {
        if (strcmp(std::get<0>(x).get(), "ProxyEnable") == 0) {
            ProxyEnable = *(DWORD*)(std::get<2>(x).get());
        }
    }

    if (ProxyEnable) {
        for (auto x : values) {
            if (strcmp(std::get<0>(x).get(), "ProxyServer") == 0) {
                *proxyAddress = (string)(char*)(std::get<2>(x).get());
                int port = (int)(std::get<2>(x).get());
                std::cout << "ProxyServer: " << (char*)(std::get<2>(x).get()) << std::endl;
                return 1;
            }
        }
    }
    else {
        std::cout << "Proxy not Enabled" << std::endl;
        return 0;
    }

    return 0;
}
