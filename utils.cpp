#include "utils.h"
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <cpr/cpr.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/static_string.hpp>
#include <boost/algorithm/string.hpp>


std::filesystem::path WORKING_FOLDER = std::filesystem::current_path();

int ensureDirectory(std::string path) {
    int len = path.length();
    char tmpDirPath[256] = { 0 };
    for (int i = 0; i < len; i++) {
        tmpDirPath[i] = path[i];
        if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/') {
            if (_access(tmpDirPath, 0) == -1) {
                int ret = _mkdir(tmpDirPath);
                if (ret == -1)
                    return ret;
            }
        }
    }
    return 0;
}

int ensureDirectory(std::filesystem::path path) {
    using namespace std;
    namespace fs = filesystem;
    if (not fs::exists(path)) {
        fs::create_directories(path);
        cout << "Folder " << path << " is created." << endl;
    }
    else {
        cout << "Folder " << path << " has existed." << endl;
    }
    return 0;

}

int existThenRemove(std::filesystem::path filePath) {
    namespace fs = std::filesystem;
    if (fs::exists(filePath)) {
        fs::remove(filePath);
        std::cout << "File " << filePath << " exists and has been deleted." << std::endl;
    }
    else {
        std::cout << "File " << filePath << " does not exist." << std::endl;
    }
    return 0;
}


int existThenRemoveDirectory(std::filesystem::path filePath) {
    namespace fs = std::filesystem;
    if (fs::exists(filePath)) {
        fs::remove_all(filePath);
        std::cout << "File " << filePath << " exists and has been deleted." << std::endl;
    }
    else {
        std::cout << "File " << filePath << " does not exist." << std::endl;
    }
    return 0;
}

std::string replaceSubstring(const std::string& input, const std::string& target, const std::string& replacement) {
    std::string result = input;
    size_t pos = result.find(target);
    while (pos != std::string::npos) {
        result.replace(pos, target.length(), replacement);
        pos = result.find(target, pos + replacement.length());
    }
    return result;
}
using namespace std;
namespace fs = filesystem;
Json::Value readJsonFile(fs::path filename)
{
    //1.打开文件
    ifstream ifile;
    ifile.open(filename);

    //2.创建json读工厂对象
    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;//utf8支持

    //3.创建json对象，等会要返回这个对象
    Json::Value root;

    //4.把文件转变为json对象，要用到上面的三个变量,数据写入root
    string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
    if (!ok) {
        cerr << "json解析错误";
    }
    cout << "1 " << (root["git"] == "false") << endl;
    cout << "2 " << (root["git"] == false) << endl;
    // cout << (root["git"] == "false") + " | " << (root["git"] == false) << endl;
    //5.返回存有数据的json，这个json对象已经能用了
    return root;
}


void writeFileFromString(const string& filename, const string& body)
{
    ofstream ofile(filename);
    ofile << body;
    ofile.close();
}

void writeJsonFile(std::filesystem::path filename, const Json::Value& root) {
    //TODO: filename -> path
    Json::StreamWriterBuilder writebuild;
    writebuild["emitUTF8"] = true;//utf8支持
    //2.把json对象转变为字符串
    string document = Json::writeString(writebuild, root);
    //3.调用前面的写文件函数,写入文件
    writeFileFromString(filename.string(), document);
}

bool isPGPLInstalled() {
    bool r1 = fs::exists(WORKING_FOLDER / "toolkit" / "Miniconda");
    bool r2 = fs::exists(WORKING_FOLDER / "toolkit" / "Git");
    return (r1 or r2);
}


int IsProcessRunning(const wchar_t* processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    int match_times = 0;
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                match_times++;
                // CloseHandle(hSnapshot);
                // return true;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return match_times;
}

bool reinitFolder(std::filesystem::path filePath) {
    //TODO: BOOL
    existThenRemove(filePath);
    ensureDirectory(filePath);
    return true;
}

bool isProtectedByGreatWall() {
    using namespace cpr;
    auto responses = Get(Url{ "www.x.com" }, Timeout{ 1500 });
    cout << "great wall code:" << responses.status_code << endl;
    return (responses.status_code > 210 or responses.status_code == 0);
}



LPCWSTR stringToLPCWSTR(string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = new wchar_t[sizeof(wchar_t) * (orig.length() - 1)];
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
    return wcstring;
}



bool KillSpecifiedProcess(const std::string& p_strPath)
{
    const int READ_ONE_NUM = 4096;
    /*
    C:\Users\10139>wmic process where name="notepad.exe" get executablepath,processid
    ExecutablePath                   ProcessId
    C:\WINDOWS\system32\notepad.exe  6196
    C:\WINDOWS\system32\notepad.exe  6056


    C:\Users\10139>taskkill /F /PID 6196 /PID 6056
    成功: 已终止 PID 为 6196 的进程。
    成功: 已终止 PID 为 6056 的进程。
    */
    if (!boost::filesystem::exists(p_strPath))
    {
        cout << p_strPath << " not exist" << endl;
        return false;
    }
    int index = p_strPath.rfind("\\");
    std::string strName = p_strPath.substr(index + 1);
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    HANDLE hStdOutRead = NULL, hStdOutWrite = NULL;
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0))
    {
        cout << "create pipe error," << GetLastError() << endl;
        return false;
    }
    STARTUPINFOA startInfo;
    PROCESS_INFORMATION procInfo;
    BOOL bSuccess = FALSE;
    ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&startInfo, sizeof(STARTUPINFOA));
    startInfo.cb = sizeof(STARTUPINFOA);
    startInfo.hStdOutput = hStdOutWrite;
    startInfo.dwFlags |= (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    startInfo.wShowWindow = SW_HIDE;

    boost::format fmt("wmic process where name=\"%1%\" get executablepath,processid");
    fmt% strName;
    std::string strSQL = fmt.str();
    bSuccess = CreateProcessA(NULL, (char*)strSQL.data(), NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInfo);
    if (!bSuccess)
    {
        cout << "create process error," << GetLastError() << endl;
        return false;
    }
    WaitForSingleObject(procInfo.hProcess, INFINITE);
    CloseHandle(hStdOutWrite);
    DWORD byteRead = 0;
    std::string strContent;
    char buffer[READ_ONE_NUM] = { 0 };
    while (true)
    {
        byteRead = 0;
        memset(buffer, 0, READ_ONE_NUM);
        BOOL bRead = ReadFile(hStdOutRead, buffer, (READ_ONE_NUM - 1) * sizeof(buffer[0]), &byteRead, NULL);
        if (!bRead)
        {
            break;
        }
        strContent.append(buffer);
    }
    CloseHandle(hStdOutRead);
    std::vector<std::string> splitVec;
    boost::split(splitVec, strContent, boost::is_any_of("\r\n"), boost::token_compress_on);
    if (splitVec.size() > 0)
    {
        if (!boost::icontains(splitVec[0], "ExecutablePath"))
        {
            // 没有这个进程名
            cout << strName << " is not runing" << endl;
            return false;
        }
        // 下面for代码：可以优化使用正则表达式来获取程序完整路径和程序PID
        // 第1行和最后1行都不是
        for (int i = 1; i < splitVec.size() - 1; i++)
        {
            std::vector<std::string> splitVec2;
            boost::split(splitVec2, splitVec[i], boost::is_any_of(" "), boost::token_compress_on);
            int size = splitVec2.size();
            if (size >= 3)
            {
                std::string exePath;
                // 取到同名程序的完整路径
                for (int i = 0; i < size - 1 - 1; i++)
                {
                    exePath.append(splitVec2[i]);
                    exePath.append(" ");
                }
                // 判定路径是否完全匹配
                if (!boost::icontains(exePath, p_strPath))
                {
                    continue;
                }

                // 程序路径可能有空格，倒数第2项为pid
                std::string pId = splitVec2[size - 1 - 1];
                std::string cmd = "taskkill /F /PID ";
                cmd.append(pId);
                cout << p_strPath << "->" << cmd << endl;
                WinExec(cmd.c_str(), SW_HIDE);
            }
        }
    }
    return true;
}
//————————————————
//
//版权声明：本文为博主原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接和本声明。
//
//原文链接：https ://blog.csdn.net/qq_29542611/article/details/122003681