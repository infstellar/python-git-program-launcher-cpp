#include "managers.h"
#include <tchar.h>
#include <comdef.h>
#include "downloader.h"

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
    // string command1 = git + " config --local --unset http.proxy";
    // string command2 = git + " config --local --unset https.proxy";
    auto sp1 = TestSpeed("https://github.com");
    auto sp2 = TestSpeed("https://gitee.com");
    string url = "";
    if (sp1 < sp2) {
        url = "https://github.com/infstellar/python-git-program-launcher";
    }
    else {
        url = "https://gitee.com/infstellar/python-git-program-launcher";
    }

    
    string command3 = git + " clone " + url;
    string command4 = git + " checkout " + "cpp_launcher";
    string command31 = git + " fetch";
    string command32 = git + " pull -f";
    //runCommand(command1); runCommand(command2); 
    runCommand(command3); 
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
    /*if (isProtectedByGreatWall()) {
        useDotCondarc();
    }*/
    
    auto python = getPython();
    auto req = WORKING_FOLDER / "python-git-program-launcher" / "requirements.txt";
    // runCommand("explorer \"" + req.string() + "\"");
    // getCmdResult(python + " -m pip install -r \"" + req.string() + "\"");
    string mirrorUrl = SelectFastestURLV2({ "pypi.org", "pypi.tuna.tsinghua.edu.cn","mirrors.aliyun.com" }, 
        { "https://pypi.org/simple", "https://pypi.tuna.tsinghua.edu.cn/simple", "https://mirrors.aliyun.com/pypi/simple" });
    runCommand((python + " -m pip install -r \"" + req.string() + "\" -i " + mirrorUrl).c_str());
    /*removeDotCondarc();*/
    return true;
}

bool killPython() {
    string python = "toolkit\\Miniconda\\python.exe";
    string abs_python = (WORKING_FOLDER / python).string();
    KillSpecifiedProcess(abs_python);
    return true;
}

bool launchPGPLCore() {
    //TODO Better cmd
    // auto python = getPython();
    string python = "toolkit\\Miniconda\\python.exe";
    string python_floder = "toolkit\\Miniconda";
    string abs_python = (WORKING_FOLDER / python).string();
    string abs_python_floder = (WORKING_FOLDER / python_floder).string();
    wstring dsf = L"123";
    cout << "PGPLC v0.5.0" << endl;
    auto ewr = dsf.c_str();
    auto pgpl = WORKING_FOLDER/"python-git-program-launcher" / "gui.py";
    // std::string str = "toolkit\\Miniconda\\python.exe python-git-program-launcher\\gui.py";
    // BSTR b = _com_util::ConvertStringToBSTR(str.c_str());
    // LPWSTR lp = b;
    // return (getCmdResult(python + " \"" + pgpl.string() + "\" ") == "2");runCommandHidden
    // return (runCommand("start /min /wait \"\" " +python + " \"" + pgpl.string() + "\" ") == 0);
    // return (runCommandHidden(lp) == 0);
    killPython();
    char* libvar;
    size_t requiredSize;

    getenv_s(&requiredSize, NULL, 0, "PATH");
    if (requiredSize == 0)
    {
        printf("PATH doesn't exist!\n");
    }

    libvar = (char*)malloc(requiredSize * sizeof(char));
    if (!libvar)
    {
        printf("Failed to allocate memory!\n");
    }
    getenv_s(&requiredSize, libvar, requiredSize, "PATH");
    cout << "Original PATH variable is: "<< libvar<<endl;
    string pathvar = string(libvar);
    string full_env = abs_python_floder + ";" + pathvar;
    _putenv_s("PATH", full_env.data());

    getenv_s(&requiredSize, NULL, 0, "PATH");

    libvar = (char*)realloc(libvar, requiredSize * sizeof(char));
    if (!libvar)
    {
        printf("Failed to allocate memory!\n");
        exit(1);
    }

    // Get the new value of the LIB environment variable.
    getenv_s(&requiredSize, libvar, requiredSize, "PATH");
    cout<<"New PATH variable is: "<< libvar<<endl;

    string r = ExeCmd(python + " \"" + pgpl.string() + "\" ");
    auto r2 = r.find("this output is used by CPP launcher to check if the python-version PGPL is successfully opened.");
    //结束后终止python进程
    killPython();
    return (not(r2 == string::npos));
}