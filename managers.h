#pragma once
#include "executor.h"
#include "utils.h"
using namespace std;
namespace fs = filesystem;

bool installAnaconda();
bool startProxy();
bool stopProxy();
bool isAnacondaInstall();
bool isPythonInstall();
bool isGitInstall();
bool isDotNetInstall();
bool isVCRuntimeInstall();
bool isVCBuildToolInstall();
bool installGit();
string getGitPath();
bool launchPGPLCore();
bool installPythonPackages();
bool installPGPLCore();
bool isVCRuntimeInstall();
bool ensureVCR();
bool installVCB();
bool useDotCondarc();
bool removeDotCondarc();
int isSysProxyEnabled(string* proxyAddress);
