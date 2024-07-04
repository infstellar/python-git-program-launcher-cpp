#include "managers.h"
#include <tchar.h>
#include <comdef.h>

bool installPGPLCore() {
    auto del_f = WORKING_FOLDER / "python-git-program-launcher";
    string command0 = "rmdir /S /Q \"" + del_f.string() + "\"";
    if (std::filesystem::exists(del_f)) {
        runCommand(command0); // python-git-program-launcher\installed_python\3.7.6_GIA_Launcher_Download_Lib\Lib删不掉，服了
    }
    if (std::filesystem::exists(del_f)) {
        return false;
    }
    // existThenRemoveDirectory(WORKING_FOLDER / "python-git-program-launcher");
    string git = getGitPath();
    string command1 = git + " config --local http.proxy false";
    string command2 = git + " config --local https.proxy false";
    string command3 = git + " clone " + "https://github.com/infstellar/python-git-program-launcher";
    string command4 = git + " checkout " + "cpp_launcher";
    string command31 = git + " fetch";
    string command32 = git + " pull -f";
    runCommand(command1); runCommand(command2); runCommand(command3); 
    _chdir("./python-git-program-launcher");
    runCommand(command31); runCommand(command32); runCommand(command4);
    _chdir("../");
    installPythonPackages();
    //TODO BOOL
    return true;
}

string getPython() {
    //auto localPython = WORKING_FOLDER / "toolkit" / "Miniconda" / "python.exe";
    fs::path localPython = "toolkit\\Miniconda\\python.exe";
    if (fs::exists(localPython)) {
        return localPython.string();
    }
    else {
        return "python";
    }
}

bool installPythonPackages() {
    if (isProtectedByGreatWall()) {
        useDotCondarc();
    }
    
    auto python = getPython();
    auto req = WORKING_FOLDER / "python-git-program-launcher" / "requirements.txt";
    // runCommand("explorer \"" + req.string() + "\"");
    // getCmdResult(python + " -m pip install -r \"" + req.string() + "\"");
    runCommand((python + " -m pip install -r \"" + req.string() + "\"").c_str());
    removeDotCondarc();
    return true;
}

bool killPython() {
    string python = "toolkit\\Miniconda\\python.exe";
    string abs_python = (WORKING_FOLDER / python).string();
    KillSpecifiedProcess(abs_python);
}

bool launchPGPLCore() {
    //TODO Better cmd
    // auto python = getPython();
    string python = "toolkit\\Miniconda\\python.exe";
    string abs_python = (WORKING_FOLDER / python).string();
    wstring dsf = L"123";
    auto ewr = dsf.c_str();
    auto pgpl = WORKING_FOLDER/"python-git-program-launcher" / "gui.py";
    // std::string str = "toolkit\\Miniconda\\python.exe python-git-program-launcher\\gui.py";
    // BSTR b = _com_util::ConvertStringToBSTR(str.c_str());
    // LPWSTR lp = b;
    // return (getCmdResult(python + " \"" + pgpl.string() + "\" ") == "2");runCommandHidden
    // return (runCommand("start /min /wait \"\" " +python + " \"" + pgpl.string() + "\" ") == 0);
    // return (runCommandHidden(lp) == 0);
    killPython();
    string r = ExeCmd(python + " \"" + pgpl.string() + "\" ");
    auto r2 = r.find("this output is used by CPP launcher to check if the python-version PGPL is successfully opened.");
    //结束后终止python进程
    killPython();
    return (not(r2 == string::npos));
}