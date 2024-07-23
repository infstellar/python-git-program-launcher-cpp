#include <iostream>
#include <UrlMon.h>
#pragma comment(lib, "urlmon.lib")
#include <array>
#include <cpr/cpr.h>
#include <chrono>
#include <string>
#include <io.h>
#include <direct.h>
#include "downloader.h"
#include "utils.h"
#include "executor.h"
#include "managers.h"


enum mirrorSite {Origin, Tsinghua, Aliyun};
const int retryTimes = 4;




double TestSpeed(std::string domain) {
    using namespace cpr;
    double delay = 0;

    for (int i = 0; i <= retryTimes-1; i++) {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        auto responses = Get(Url{ domain }, Timeout{1500});
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        double timeCost = elapsed_seconds.count();
        if (responses.status_code != 200 and responses.status_code != 206) {
            timeCost = 4;
        }
        std::cout << i << ": " << timeCost << " | " << responses.status_code << std::endl;
        delay = delay + timeCost;
    }
    return delay / retryTimes;
}



std::string SelectFastestURL(std::string originUrl, std::array<std::string, 2> originMirrorUrl, std::array<std::string, 2> originMirrorDomain) {
    using namespace std;
    // extract domain
    size_t startPos = originUrl.find("https://");
    size_t endPos = originUrl.find("/", startPos + 8); // Adding 8 to skip "https://"
    string originDomain, suffix;
    if (startPos != string::npos && endPos != string::npos) {
        originDomain = originUrl.substr(startPos + 8, endPos - startPos - 8);
        cout << "Extracted part: " << originDomain << endl;
        string url;
    }
    else {
        cout << "Invalid URL format." << endl;
        return "";
    }


    size_t thirdSlashPos = originUrl.find("/", originUrl.find("/", originUrl.find("/") + 1) + 1);

    if (thirdSlashPos != string::npos) {
        suffix = originUrl.substr(thirdSlashPos + 1);
        cout << "Extracted part: " << suffix << endl;
    }
    else {
        cout << "Invalid URL format." << endl;
        return "";
    }

    array<string, 3> domains = {originDomain, originMirrorDomain[0], originMirrorDomain[1]};
    array<string, 3> mirrorUrl = { "https://" + originDomain, originMirrorUrl[0], originMirrorUrl[1] };
    double delays[3];
    double min_cost = 99;

    for (int i = 0; i <= 2; i++) {
        delays[i] = TestSpeed(domains[i]);
        cout << delays[i] << endl;
    }
    
    int min_num = min_element(delays, delays+3) - delays;
    string rUrl = mirrorUrl[min_num] + "/" + suffix;
    cout << "Fastest url: " << rUrl;
    return rUrl;

}

bool getRemoteFile(const std::string& url, const std::string& filename)
{
    static uint64_t s_totalSize = 0;
    std::string tmp_filename = filename + ".tmp";
    std::ofstream of(tmp_filename, std::ios::binary | std::ios::app);
    auto pos = of.tellp();
    std::cout << "of.tellp: " << pos << std::endl;

    cpr::Url cpr_url{ url };
    cpr::Session s;
    s.SetVerifySsl(cpr::VerifySsl{ false });
    s.SetUrl(cpr_url);
    //s.SetHeader(cpr::Header{{"Accept-Encoding", "gzip"}});

    auto fileLength = s.GetDownloadFileLength();
    s.SetRange(cpr::Range{ pos, fileLength - 1 });

    cpr::Response response = s.Download(of);
    s_totalSize += response.downloaded_bytes;

    std::cout << "response.status_code: " << response.status_code
        << ", response.downloaded_bytes: " << response.downloaded_bytes
        << std::endl;

    if (s_totalSize >= fileLength)
    {
        s_totalSize = 0;
        of.close();
        std::cout << "rename: " << rename(tmp_filename.c_str(), filename.c_str()) << std::endl;
        return true;
    }

    return false;
}

bool downloadFiles(std::string fileURL, std::string fileName) {
    using namespace std;
    fs::path p = WORKING_FOLDER / "cache" / fileName;
    string filePath = p.string();
    if (REDOWNLOAD) {
        existThenRemove(filePath);
    }
    else {
        if (std::filesystem::exists(filePath)) {
            return true;
        }
    }
    ensureDirectory(filePath);
    existThenRemove(filePath+".tmp");
    bool success = false;
    cout << "downloading: " << fileURL << endl;
    while (!success)
    {
        success = getRemoteFile(fileURL, filePath);
    }
    // bool r = getRemoteFile(fileURL, filePath);
    return success;
}

bool downloadAnaconda() {
    using namespace std;
    string url = "https://repo.anaconda.com/miniconda/Miniconda3-py310_24.4.0-0-Windows-x86_64.exe";
    url = SelectFastestURL(url, { "https://mirrors.tuna.tsinghua.edu.cn/anaconda", "https://mirrors.tuna.tsinghua.edu.cn/anaconda" }, {"mirrors.tuna.tsinghua.edu.cn","mirrors.tuna.tsinghua.edu.cn"});
    auto r = downloadFiles(url, "miniconda.exe");
    return r;
}

bool downloadGit() {
    using namespace std;
    string url = "https://github.com/git-for-windows/git/releases/download/v2.45.2.windows.1/PortableGit-2.45.2-64-bit.7z.exe";
    auto r = downloadFiles(url, "PortableGit.7z.exe");
    return r;
}
using namespace std;
namespace fs = filesystem;

bool unzip(fs::path zipFile, fs::path targetPath) {
    
    fs::path unzip = WORKING_FOLDER/"resource"/"7za.exe";
    ensureDirectory(targetPath);
    string command = "start /min /B /wait \"\" \"" +  unzip.string() + "\"" + " " + "x" + " " + "\"" + zipFile.string() + "\"" + " " + "-o" + "\"" + targetPath.string() + "\"";
    bool r = runCommand(command);
    return r;
    
}

bool ensure7zip() {
    using namespace std;
    namespace fs = filesystem;
    fs::path filePath = WORKING_FOLDER / "toolkit" / "7z.exe";
    if (not fs::exists(filePath)) {
        ensureDirectory(filePath.parent_path());
        string url = "https://7-zip.org/a/7zr.exe";
        bool r = downloadFiles(url, "7z.exe");
        if (r) {
            fs::copy_file(WORKING_FOLDER / "cache" / "7z.exe", filePath);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
        
    
}
using namespace std;
namespace fs = filesystem;
bool ensureFastGithub() {
    fs::path filePath = WORKING_FOLDER / "toolkit" / "fastgithub_win-x64" / "fastgithub.exe";
    if (not fs::exists(filePath)) {
        ensureDirectory(filePath.parent_path());
        string url = "https://gitee.com/XingYuan55/FastGithub/releases/download/2.1.4/fastgithub_win-x64.zip";
        bool r = downloadFiles(url, "fastgithub.zip");
        if (r) {
            return unzip(WORKING_FOLDER / "cache" / "fastgithub.zip", WORKING_FOLDER / "toolkit");
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
}

bool openFastGithub() {
    return startProxy();
}

bool downloadVCR() {
    string url = "https://aka.ms/vs/17/release/vc_redist.x64.exe";
    return downloadFiles(url, "vc_redist.x64.exe");

}

bool downloadVCB() {
    string url = "https://download.microsoft.com/download/5/F/7/5F7ACAEB-8363-451F-9425-68A90F98B238/visualcppbuildtools_full.exe";
    return downloadFiles(url, "visualcppbuildtools_full.exe");
}