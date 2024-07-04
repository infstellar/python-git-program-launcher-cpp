// InstallTipsDlg.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "InstallTipsDlg.h"
#include "utils.h"


// CInstallTipsDlg 对话框

IMPLEMENT_DYNAMIC(CInstallTipsDlg, CDialogEx)

CInstallTipsDlg::CInstallTipsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSTALL_TIPS_DIALOG, pParent)
{

}

CInstallTipsDlg::~CInstallTipsDlg()
{
}

void CInstallTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_DRIVER_TIP, m_CDriverTip);
}


BEGIN_MESSAGE_MAP(CInstallTipsDlg, CDialogEx)
END_MESSAGE_MAP()


// CInstallTipsDlg 消息处理程序


BOOL CInstallTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//在此添加额外的初始化
	//C盘检测
	if (WORKING_FOLDER.string().find("C:") != WORKING_FOLDER.string().npos) {
		m_CDriverTip.ShowWindow(SW_SHOW);
	}
	else {
		m_CDriverTip.ShowWindow(SW_HIDE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInstallTipsDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
