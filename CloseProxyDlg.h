#pragma once
#include "afxdialogex.h"


// CCloseProxyDlg 对话框

class CCloseProxyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCloseProxyDlg)

public:
	CCloseProxyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCloseProxyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSE_PROXY_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CStatic m_ProxyTip;
	virtual BOOL OnInitDialog();
};
