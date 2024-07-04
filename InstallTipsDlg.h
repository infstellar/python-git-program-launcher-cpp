#pragma once
#include "afxdialogex.h"


// CInstallTipsDlg 对话框

class CInstallTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInstallTipsDlg)

public:
	CInstallTipsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInstallTipsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALL_TIPS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_CDriverTip;
	virtual void OnCancel();
};
