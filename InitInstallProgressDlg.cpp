// InitInstallProgressDlg.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "InitInstallProgressDlg.h"
#include "downloader.h"
#include "managers.h"
#include "utils.h"

UINT CInitInstallProgressDlg::MyThreadFunction(LPVOID pParam) {
	// CProgressCtrl* myProCtrl = (CProgressCtrl*)pParam;
	CProgressCtrl* myProCtrl = ((CInitInstallProgressDlg*)pParam)->myProCtrl;
	// 线程的实际工作在这里
	Json::Value config = readJsonFile(WORKING_FOLDER / "init_install_checkbox.json");
	// cout << "3 " << (config["git"] == false) << endl;
	myProCtrl->SetPos(10);
	CInitInstallProgressDlg* dlg = (CInitInstallProgressDlg*)pParam;
	dlg->m_installTips.SetWindowTextW(L"正在安装Github加速器");
	if (config["github_acceleration"] == true) {
		ensureFastGithub();
		openFastGithub();
		// Sleep(3000);
		//TODO: find a better method to wait fastgithub open.
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
	myProCtrl->SetPos(20);
	dlg->m_installTips.SetWindowTextW(L"正在安装Git");
	if (config["git"] == true) {
		downloadGit();
		myProCtrl->SetPos(30);
		installGit();
		
	}
	myProCtrl->SetPos(40);
	dlg->m_installTips.SetWindowTextW(L"正在安装miniconda");
	if (config["miniconda"] == true) {
		downloadAnaconda();
		myProCtrl->SetPos(50);
		installAnaconda();
		
	}
	myProCtrl->SetPos(60);
	if (config["vc_runtime"] == true) {
		dlg->m_installTips.SetWindowTextW(L"正在安装VC运行时组件");
		ensureVCR();
	}
	myProCtrl->SetPos(70);
	if (config["vc_build_tools"] == true) {
		dlg->m_installTips.SetWindowTextW(L"正在安装VC构建工具");
		downloadVCB();
		installVCB();
	}
	myProCtrl->SetPos(80);
	dlg->m_installTips.SetWindowTextW(L"正在安装PGPL");
	bool r1 = installPGPLCore();
	if (not r1) {
		dlg->m_installTips.SetWindowTextW(L" 文件被占用，安装失败，请重启电脑后重试 ");
		COLORREF clrBar = RGB(255, 0, 0); // the bar color
		myProCtrl->SendMessage(PBM_SETBARCOLOR, 0, (LPARAM)clrBar);
		return false;
	}
	myProCtrl->SetPos(90);
	dlg->m_installTips.SetWindowTextW(L"安装python包");
	installPythonPackages();
	myProCtrl->SetPos(100);
	dlg->m_installTips.SetWindowTextW(L"正在启动PGPL,如果出现白屏请耐心等待.如果白屏超过5分钟,请检查防火墙设置或重启程序或电脑.在白屏阶段关闭程序可能导致程序结束不完全 ");
	bool r = launchPGPLCore();
	if (r) {
		((CInitInstallProgressDlg*)pParam)->CDialogEx::OnOK();
	}
	else {
		dlg->m_installTips.SetWindowTextW(L"下载失败，您可以关闭并重新启动程序来尝试自动修复。如果反复出现问题，欢迎提交issue并附上pgpl_cpp.log文件");
	}
	stopProxy();
	return r;
	//TODO
	
}


// CInitInstallProgressDlg 对话框

IMPLEMENT_DYNAMIC(CInitInstallProgressDlg, CDialogEx)

CInitInstallProgressDlg::CInitInstallProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INIT_INSTALL_PROGRESS, pParent)
{

}

CInitInstallProgressDlg::~CInitInstallProgressDlg()
{
}

void CInitInstallProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_installTips);
}


BEGIN_MESSAGE_MAP(CInitInstallProgressDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CInitInstallProgressDlg::OnNMCustomdrawProgress1)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CInitInstallProgressDlg 消息处理程序




BOOL CInitInstallProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	

	myProCtrl = (CProgressCtrl*)CWnd::GetDlgItem(IDC_PROGRESS1);
	myProCtrl->SetRange(0, 100);
	CWinThread* MyThread = AfxBeginThread(MyThreadFunction, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInitInstallProgressDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

INT_PTR CInitInstallProgressDlg::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::DoModal();
}


void CInitInstallProgressDlg::LGC_GITINSTALLED(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

}
