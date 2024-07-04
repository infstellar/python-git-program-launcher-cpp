// EnvCheckTipsDlg.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "EnvCheckTipsDlg.h"
#include "utils.h"


// CEnvCheckTipsDlg 对话框

IMPLEMENT_DYNAMIC(CEnvCheckTipsDlg, CDialogEx)

CEnvCheckTipsDlg::CEnvCheckTipsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENV_CHECK_DIALOG, pParent)
{

}

CEnvCheckTipsDlg::~CEnvCheckTipsDlg()
{
}

void CEnvCheckTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEnvCheckTipsDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnvCheckTipsDlg 消息处理程序


BOOL CEnvCheckTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
