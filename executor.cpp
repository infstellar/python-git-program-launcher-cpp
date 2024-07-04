#include "executor.h"
#include <windows.h>
#include <atlconv.h>
#include <direct.h>
#pragma comment(lib,"user32.lib")

HWND hwnd = GetForegroundWindow();
// �ص���������������ִ�е����
void CommandOutputCallback(const char* output) {
    std::cout << "Command output: " << output;
}

void HideWindow() {
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);	//����ָ�����ڵ���ʾ״̬
	}
}

void ShowWindow() {
	if (hwnd) {
		ShowWindow(hwnd, SW_SHOW);	//����ָ�����ڵ���ʾ״̬
	}
}


// ִ�������ȡ����ֵ
int runCommand(std::string command) {
    FILE* fp = nullptr;
    char data[1024] = { 0 };

    // �ض����׼�����������׼���
    std::string cmd_with_redirection = command + " 2>&1";

    std::cout << "Executing: " << command << std::endl;

    fp = _popen(cmd_with_redirection.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "popen failed: " << errno << std::endl;
        return -1;
    }

    // ��ȡ�������
    while (fgets(data, sizeof(data), fp) != nullptr) {
        CommandOutputCallback(data);
    }
    int r = _pclose(fp);
    std::cout << "result: " << r << std::endl;
    // �ر�������ȡִ�н��
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

	// unsigned len = str.size() * 2;// Ԥ���ֽ���
	// setlocale(LC_CTYPE, "");     //������ô˺���
	// wchar_t* p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	// size_t ptoc = len;
	// size_t len2 = len;
	// mbstowcs_s(ptoc, p, str.c_str(), len2);// ת��
	// std::wstring str1(p);
	// delete[] p;// �ͷ�������ڴ�
	// return str1;
}
string ExeCmd(string pszCmd)
{
	wstring pszCmd_w = Str2Wstr("cmd.exe /c " + pszCmd);
	wcout << "pszCmd_w is " << pszCmd_w << endl;
	// CreateProcessW(NULL, (LPWSTR)pszCmd_w.c_str(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	// ���������ܵ�,write->read;
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		cout << "@ CreatePipe failed!" << endl;
		// return -1;
		return (" ");
	}
	cout << "@0" << endl;
	// ���������н���������Ϣ(�����ط�ʽ���������λ�������hWrite
	STARTUPINFO si = { sizeof(STARTUPINFO) }; // Pointer to STARTUPINFO structure;
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE; //���ش��ڣ�
	si.hStdError = hWrite;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite; //�ܵ�������˿����������е������
	// ����������
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
	CloseHandle(hWrite);//�رչܵ�������˿ڣ�
	// ��ȡ�����з���ֵ
	string strRetTmp;
	char buff[1024] = { 0 };
	DWORD dwRead = 0;
	strRetTmp = buff;
	while (ReadFile(hRead, buff, 1024, &dwRead, NULL))//�ӹܵ�������˻�ȡ������д������ݣ�
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
	// CloseHandle(hRead);//�رչܵ�������˿ڣ�
	// return (int)excode;
	return strRetTmp;


	
}

