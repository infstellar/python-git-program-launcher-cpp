#pragma once
#include "resource.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <wtypes.h>

int runCommand(std::string command);
// std::string getCmdResult(const std::string& strCmd);
// int runCommandHidden(LPWSTR command);
std::string ExeCmd(std::string pszCmd);
int runCommandHidden(std::string command);