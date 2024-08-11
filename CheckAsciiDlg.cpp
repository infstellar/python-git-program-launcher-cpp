// CheckAsciiDlg.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "CheckAsciiDlg.h"
#include "utils.h"


// CCheckAsciiDlg 对话框

IMPLEMENT_DYNAMIC(CCheckAsciiDlg, CDialogEx)

CCheckAsciiDlg::CCheckAsciiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCheckAsciiDlg, pParent)
{

}

CCheckAsciiDlg::~CCheckAsciiDlg()
{
}

void CCheckAsciiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckAsciiDlg, CDialogEx)
END_MESSAGE_MAP()

void CCheckAsciiDlg::OnBnClickedOk()
{
	exit(0);
}

BOOL CCheckAsciiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (isAscii(WORKING_FOLDER.string())) {
		CDialogEx::OnOK();
	}
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CCheckAsciiDlg 消息处理程序
