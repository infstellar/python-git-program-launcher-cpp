#include "managers.h"
#include <tchar.h>

bool isAnacondaInstall() {
	return (runCommand("anaconda --version") == 0);
}

bool isPythonInstall() {
	return (runCommand("python --version") == 0);
}

bool isGitInstall() {
	return (runCommand("git --version") == 0);
}

bool isDotNetInstall() {
	return (runCommand("dotnet --version") == 0);
}

bool isVCRuntimeInstall() {
	// HKEY hKey;
	// TCHAR strDir[MAX_PATH] = {};
	// DWORD nLength = MAX_PATH;

	long result = RegGetValueW(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64"), _T("Version"), RRF_RT_REG_SZ, NULL, NULL, NULL);
	//TODO
	return (result == ERROR_SUCCESS);
}

bool isVCBuildToolInstall() {
	bool r1 = (runCommand("g++ --version") == 0);
	bool r2 = (runCommand("cmake --version") == 0);
	return (r1 or r2);
}

