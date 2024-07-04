#include <io.h>
#include <direct.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <json/json.h>
#include <fstream>
#include<cstdlib>
typedef const wchar_t* LPCWSTR;


int ensureDirectory(std::string path);
int ensureDirectory(std::filesystem::path path);
int existThenRemove(std::filesystem::path filePath);

std::string replaceSubstring(const std::string& input, const std::string& target, const std::string& replacement);
void writeJsonFile(std::filesystem::path filename, const Json::Value& root);
Json::Value readJsonFile(std::filesystem::path filename);
bool isPGPLInstalled();
int existThenRemoveDirectory(std::filesystem::path);
bool reinitFolder(std::filesystem::path filePath);
bool isProtectedByGreatWall();
LPCWSTR stringToLPCWSTR(std::string orig);
bool KillSpecifiedProcess(const std::string& p_strPath);
bool killPython();




#ifndef UTILS_H  // 头文件宏保护
#define UTILS_H

extern std::filesystem::path WORKING_FOLDER;

#endif  // UTILS_H
