// CPGPL2_FUNC1.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "CPGPL2_FUNC1.h"
#include "PGPLMFC2Dlg.h"
#include "InitInstallProgressDlg.h"
#include <json/value.h>
#include "managers.h"
#include "utils.h"
#include "CloseProxyDlg.h"+
#include "InstallTipsDlg.h"
#include "EnvCheckTipsDlg.h"


// CPGPL2_FUNC1 对话框

IMPLEMENT_DYNAMIC(CPGPL2_FUNC1, CDialogEx)

CPGPL2_FUNC1::CPGPL2_FUNC1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_FUNCTION_1, pParent)
{
	// theApp.m_pMainWnd = this;
}

CPGPL2_FUNC1::~CPGPL2_FUNC1()
{
}

void CPGPL2_FUNC1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, minicondaCheckbox);
	DDX_Control(pDX, IDC_CHECK2, m_gitCheckbox);
	DDX_Control(pDX, IDC_CHECK3, m_githubAccCheckbox);
	DDX_Control(pDX, IDC_EDIT1, m_tipsEDIT);
	DDX_Control(pDX, IDC_CHECK4, m_vcrCheckbox);
	DDX_Control(pDX, IDC_CHECK5, m_vcbCheckbox);
}


BEGIN_MESSAGE_MAP(CPGPL2_FUNC1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPGPL2_FUNC1::OnBnClickedOk)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
	ON_WM_ENTERIDLE()
END_MESSAGE_MAP()


// CPGPL2_FUNC1 消息处理程序

void CPGPL2_FUNC1::TipInit()
{
	if (m_TipMsg.m_hWnd == NULL)
	{
		m_TipMsg.Create(this);
		m_TipMsg.Activate(true);
		m_TipMsg.AddTool(this, TTS_ALWAYSTIP);//TTS_ALWAYSTIP
		m_TipMsg.SetMaxTipWidth(20);//设定显示宽度，超长内容自动换行
		// m_TipMsg.SetTipBkColor(APP_MAIN_TXT_COLOR);//(RGB(210,210,255)); 
		// m_TipMsg.SetTipTextColor(APP_MAIN_BK_NIGHT_COLOR);//(RGB(0,106,53)); 
		m_TipMsg.SetDelayTime(TTDT_INITIAL, 0); //鼠标指向多久后显示提示，毫秒
		m_TipMsg.SetDelayTime(TTDT_AUTOPOP, 20000); //鼠标保持指向，提示显示多久，毫秒
		//m_TipMsg.SetDelayTime(TTDT_RESHOW, 1000);;//鼠标从当前控件移动到另一个控件后，需要等待1秒后弹出提示；	
	}
}

BOOL CPGPL2_FUNC1::OnInitDialog() {
	CDialogEx::OnInitDialog();
	envDlg = new CEnvCheckTipsDlg();
	envDlg->Create(IDD_ENV_CHECK_DIALOG);
	envDlg->ShowWindow(SW_SHOW);
	envDlg->UpdateWindow();

	auto r_g = isGitInstall();
	envDlg->UpdateWindow();
	auto r_a = isAnacondaInstall();
	auto r_vcr = isVCRuntimeInstall();
	auto r_vcb = isVCBuildToolInstall();
	auto r_inC = isProtectedByGreatWall();
	minicondaCheckbox.SetCheck(BST_CHECKED);
	minicondaCheckbox.EnableWindow(r_a);
	m_gitCheckbox.SetCheck(BST_CHECKED);
	m_gitCheckbox.EnableWindow(r_g);
	m_githubAccCheckbox.SetCheck(r_inC);
	m_vcrCheckbox.SetCheck(r_vcr == false);
	envDlg->DestroyWindow();
	if (not (r_g or r_a)) {
		OnBnClickedOk();
		return TRUE;
	}
	TipInit();
	EnableToolTips(TRUE);

	return TRUE;
}


void CPGPL2_FUNC1::OnBnClickedOk()
{	
	
	Json::Value checkboxValue;

	checkboxValue["miniconda"] = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1));
	checkboxValue["git"] = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2));
	checkboxValue["github_acceleration"] = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3));
	checkboxValue["vc_runtime"] = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4));
	checkboxValue["vc_build_tools"] = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5));

	writeJsonFile(WORKING_FOLDER / "init_install_checkbox.json", checkboxValue);

	//在此添加控件通知处理程序代码
	EndDialog();
	
}

void CPGPL2_FUNC1::EndDialog() {
	CInitInstallProgressDlg dlg;
	CCloseProxyDlg dlg2;
	CInstallTipsDlg dlg3;
	// m_pFunc1Dlg->Create(IDD_SELECT_FUNCTION_1, this);
	// 显示非模态对话框   

	// m_pFunc1Dlg.ShowWindow(SW_SHOW);
	//下载之前的检查
	dlg2.DoModal();
	INT_PTR nResponse = dlg3.DoModal();
	if (nResponse == IDCANCEL) {
		CPGPL2_FUNC1::OnOK();
		return;
	}
	theApp.m_pMainWnd = &dlg;
	this->SendMessage(WM_DESTROY);
	this->SendMessage(WM_CLOSE);
	dlg.DoModal();
	CPGPL2_FUNC1::OnOK();
}

BOOL CPGPL2_FUNC1::SetTipText(UINT id, NMHDR* pTTTStruct, LRESULT* pResult)
{
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pTTTStruct;
	UINT nID = pTTTStruct->idFrom;   //得到相应窗口ID，有可能是HWND   

	//表明nID是否为HWND
	if (pTTT->uFlags & TTF_IDISHWND)
	{
		//从HWND得到ID值，当然你也可以通过HWND值来判断
		nID = ::GetDlgCtrlID((HWND)nID);
		if (NULL == nID)
			return FALSE;

		switch (nID)
		{
		case(IDC_CHECK1):
			lstrcpy(pTTT->lpszText, L"安装Python包管理器。即使您已安装过Anaconda，仍推荐选择此选项以避免对原Anaconda的污染。不会修改系统变量和注册表。");
			break;
		case(IDC_CHECK2):
			lstrcpy(pTTT->lpszText, L"安装Git。如果您已安装过Git，可以取消此选项。PGPL将使用系统的Git。不会修改系统变量和注册表。");
			break;
		case(IDC_CHECK3):
			lstrcpy(pTTT->lpszText, L"下载Github访问加速服务。推荐下载，您可以在之后手动设置是否打开。如果您确认不需要，可以取消此选项。不会修改系统变量和注册表。");
			break;
		case(IDC_CHECK4):
			lstrcpy(pTTT->lpszText, L"安装VC++ 运行时组件。此为用户层面的安装，您稍后需要手动确认其安装程序。推荐安装。");
			break;
		case(IDC_CHECK5):
			lstrcpy(pTTT->lpszText, L"安装VC++ 构建工具。此为用户层面的安装，您稍后需要手动确认其安装程序。若PGPL运行失败且提示安装时才需要安装。");
			break;
		default:
			break;
		}

		return TRUE;
	}
	return FALSE;
}





void CPGPL2_FUNC1::OnEnterIdle(UINT nWhy, CWnd* pWho)
{
	CDialogEx::OnEnterIdle(nWhy, pWho);

	//在此处添加消息处理程序代码
}
