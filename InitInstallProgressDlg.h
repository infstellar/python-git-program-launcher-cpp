#pragma once
#include "afxdialogex.h"


// CInitInstallProgressDlg 对话框

class CInitInstallProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInitInstallProgressDlg)

public:
	static UINT MyThreadFunction(LPVOID pParam);
	
	CInitInstallProgressDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInitInstallProgressDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INIT_INSTALL_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		BOOL OnInitDialog();
	void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual INT_PTR DoModal();
	afx_msg void LGC_GITINSTALLED(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CProgressCtrl* myProCtrl = NULL;
	// UINT MyThreadFunction(LPVOID pParam);
	
	CEdit m_installTips;
};
