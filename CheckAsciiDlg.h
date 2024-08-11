#pragma once
#include "afxdialogex.h"


// CCheckAsciiDlg 对话框

class CCheckAsciiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckAsciiDlg)

public:
	CCheckAsciiDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCheckAsciiDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCheckAsciiDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
