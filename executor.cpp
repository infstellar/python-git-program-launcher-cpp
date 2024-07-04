#include "executor.h"
#include <windows.h>
#include <atlconv.h>
#include <direct.h>
#pragma comment(lib,"user32.lib")

HWND hwnd = GetForegroundWindow();
// 回调函数，处理命令执行的输出
void CommandOutputCallback(const char* output) {
    std::cout << "Command output: " << output;
}

void HideWindow() {
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);	//设置指定窗口的显示状态
	}
}

void ShowWindow() {
	if (hwnd) {
		ShowWindow(hwnd, SW_SHOW);	//设置指定窗口的显示状态
	}
}


// 执行命令并获取返回值
int runCommand(std::string command) {
    FILE* fp = nullptr;
    char data[1024] = { 0 };

    // 重定向标准错误输出到标准输出
    std::string cmd_with_redirection = command + " 2>&1";

    std::cout << "Executing: " << command << std::endl;

    fp = _popen(cmd_with_redirection.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "popen failed: " << errno << std::endl;
        return -1;
    }

    // 读取命令输出
    while (fgets(data, sizeof(data), fp) != nullptr) {
        CommandOutputCallback(data);
    }
    int r = _pclose(fp);
    std::cout << "result: " << r << std::endl;
    // 关闭流并获取执行结果
    return r;
}

int runCommandHidden(std::string command) {
	HideWindow();
	int r = runCommand(command);
	ShowWindow();
	return r;
}

using namespace std;
wstring Str2Wstr(string str)
{
	const char* CStr = str.c_str();

	size_t len = strlen(CStr) + 1;

	size_t converted = 0;

	wchar_t* WStr;

	WStr = (wchar_t*)malloc(len * sizeof(wchar_t));

	mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);

	return std::wstring(WStr);

	// unsigned len = str.size() * 2;// 预留字节数
	// setlocale(LC_CTYPE, "");     //必须调用此函数
	// wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	// size_t ptoc = len;
	// size_t len2 = len;
	// mbstowcs_s(ptoc, p, str.c_str(), len2);// 转换
	// std::wstring str1(p);
	// delete[] p;// 释放申请的内存
	// return str1;
}
string ExeCmd(string pszCmd)
{
	wstring pszCmd_w = Str2Wstr("cmd.exe /c " + pszCmd);
	wcout << "pszCmd_w is " << pszCmd_w << endl;
	// CreateProcessW(NULL, (LPWSTR)pszCmd_w.c_str(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	// 创建匿名管道,write->read;
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		cout << "@ CreatePipe failed!" << endl;
		// return -1;
		return (" ");
	}
	cout << "@0" << endl;
	// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite
	STARTUPINFO si = { sizeof(STARTUPINFO) }; // Pointer to STARTUPINFO structure;
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE; //隐藏窗口；
	si.hStdError = hWrite;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite; //管道的输入端口连接命令行的输出；
	// 启动命令行
	PROCESS_INFORMATION pi;// Pointer to PROCESS_INFORMATION structure;
	if (!CreateProcessW(
		NULL,
		(LPWSTR)pszCmd_w.c_str(),
		NULL,
		NULL,
		TRUE,
		//FALSE,          // Set handle inheritance to FALSE
		NULL,
		//0,              // No creation flags
		NULL,
		NULL,
		&si,
		&pi))
	{
		cout << "@ CreateProcess failed!" << endl;
		// return -1;
		return ("Cannot create process");
	}
	CloseHandle(hWrite);//关闭管道的输入端口；
	// 读取命令行返回值
	string strRetTmp;
	char buff[1024] = { 0 };
	DWORD dwRead = 0;
	strRetTmp = buff;
	while (ReadFile(hRead, buff, 1024, &dwRead, NULL))//从管道的输出端获取命令行写入的数据；
	{
		cout << "buff = " << buff << endl;
		strRetTmp += buff;
	}
	
	cout << "strRetTmp:" << strRetTmp << endl;
	// auto r2 = GetLastError();
	// DWORD excode = 0;
	// LPDWORD lpExitCode = &excode;
	// bool r = GetExitCodeProcess(hRead, lpExitCode);
	// cout << "exit code " << excode << " | " << r << "|" << r2 << endl;
	// CloseHandle(hRead);//关闭管道的输出端口；
	// return (int)excode;
	return strRetTmp;


	
}

