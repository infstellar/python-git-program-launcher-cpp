#include "managers.h"
#include "downloader.h"


bool ensureVCR() {
	if (isVCRuntimeInstall() == false) {
		downloadVCR();
		runCommand("explorer cache\\vc_redist.x64.exe");
	}
	else {
		return true;
	}
}

bool installVCB() {
	return runCommand("explorer cache\\visualcppbuildtools_full.exe");
}