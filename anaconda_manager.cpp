#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

#include "managers.h"
#include <iostream>
#include <fstream>
#include <string>

bool installAnaconda() {
	using namespace std;
	namespace fs = std::filesystem;
	reinitFolder(WORKING_FOLDER / "toolkit" / "Miniconda");
	fs::path minicondaPath = WORKING_FOLDER / "cache" / "miniconda.exe";
	fs::path toolkitPath = WORKING_FOLDER / "toolkit" / "Miniconda";
	string command = "start /min /wait \"\" $minicondaPath /InstallationType=JustMe /RegisterPython=0 /S /D=$toolkitPath";
	command = replaceSubstring(command, "$minicondaPath", "\"" + minicondaPath.string() + "\"");
	command = replaceSubstring(command, "$toolkitPath", toolkitPath.string());
	auto r = runCommand(command);
	if (r != -1) {
		return true;
	}
	else {
		return false;
	}
	
}



bool useDotCondarc() {
	string CONDARC_FILE_PATH = std::getenv("USERPROFILE");
	CONDARC_FILE_PATH = CONDARC_FILE_PATH + "/.condarc";
	string CONDARC_MARK_PATH = std::getenv("USERPROFILE");
	CONDARC_MARK_PATH = CONDARC_MARK_PATH + "/.condarc_flag";

	if (!fs::exists(CONDARC_FILE_PATH)) {
		bool CONDARC_NOT_FOUND_flag = true;
		std::cout << "The .condarc not found, creating one." << std::endl;

		// Copy .condarc from a toolkit directory (adjust the path accordingly)
		fs::copy_file(WORKING_FOLDER / "resource" / ".condarc", CONDARC_MARK_PATH);
		// Create a copy of .condarc for marking
		fs::copy_file(WORKING_FOLDER / "resource" / ".condarc", CONDARC_FILE_PATH);
		
	}
	else {
		std::cout << "The .condarc already exists. You may have installed Anaconda/Miniconda, and the original configuration file will be used." << std::endl;
	}

	return true;
}

bool removeDotCondarc() {
	//TODO: BOOL
	string CONDARC_FILE_PATH = std::getenv("USERPROFILE");
	CONDARC_FILE_PATH = CONDARC_FILE_PATH + "/.condarc";
	string CONDARC_MARK_PATH = std::getenv("USERPROFILE");
	CONDARC_MARK_PATH = CONDARC_MARK_PATH + "/.condarc_flag";
	if (fs::exists(CONDARC_MARK_PATH)) {
		existThenRemove(CONDARC_FILE_PATH);
		existThenRemove(CONDARC_MARK_PATH);
	}
	return true;
}
