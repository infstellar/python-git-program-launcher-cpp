#include "utils.h"

std::string SelectFastestURL(std::string originUrl, std::array<std::string, 2> originMirrorUrl, std::array<std::string, 2> originMirrorDomain);
std::string SelectFastestURLV2(std::array<std::string, 3> DomainUrl, std::array<std::string, 3> DownloadUrl);
double TestSpeed(std::string domain);
bool getRemoteFile(const std::string& url, const std::string& filename);
bool downloadFiles(std::string fileURL, std::string fileName);
bool downloadAnaconda();
bool downloadGit();
bool ensure7zip();
bool ensureFastGithub();
bool openFastGithub();
const bool REDOWNLOAD = true;
bool unzip(std::filesystem::path zipFile, std::filesystem::path targetPath);
bool downloadVCR();
bool downloadVCB();