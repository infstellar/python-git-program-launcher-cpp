#include "managers.h"
#include "downloader.h"

bool installGit() {
	existThenRemoveDirectory(WORKING_FOLDER / "toolkit" / "Git");
	return unzip(WORKING_FOLDER / "cache" / "PortableGit.7z.exe", WORKING_FOLDER / "toolkit" / "Git");
}

string getGitPath() {
	auto localGit = WORKING_FOLDER / "toolkit" / "Git" / "bin" / "git.exe";
	if (fs::exists(localGit)) {
		return "\"" + localGit.string() + "\"";
	}
	else {
		return "git";
	}
}