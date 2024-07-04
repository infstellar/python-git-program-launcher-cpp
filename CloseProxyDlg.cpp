// CloseProxyDlg.cpp: 实现文件
//

#include "pch.h"
#include "PGPLMFC2.h"
#include "afxdialogex.h"
#include "CloseProxyDlg.h"
#include "managers.h"


// CCloseProxyDlg 对话框

IMPLEMENT_DYNAMIC(CCloseProxyDlg, CDialogEx)

CCloseProxyDlg::CCloseProxyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSE_PROXY_DIALOG1, pParent)
{

}

CCloseProxyDlg::~CCloseProxyDlg()
{
}

void CCloseProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT1, m_ProxyTip);
}


BEGIN_MESSAGE_MAP(CCloseProxyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCloseProxyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCloseProxyDlg 消息处理程序


void CCloseProxyDlg::OnBnClickedOk()
{
	string port = "";
	if (isSysProxyEnabled(&port)) {
		if (port.find("7890") != port.npos) {
			m_ProxyTip.SetWindowTextW(L"检测到Clash代理程序开启，请关闭Clash。");
		}
		else {
			m_ProxyTip.SetWindowTextW(L"系统代理未关闭。请关闭代理软件或在设置->代理中手动关闭。");
		}
	}
	else {
		CDialogEx::OnOK();
	}
	
}

BOOL CCloseProxyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	OnBnClickedOk();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
