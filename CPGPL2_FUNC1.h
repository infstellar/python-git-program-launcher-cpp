#pragma once
#include "afxdialogex.h"
#include "EnvCheckTipsDlg.h"



// CPGPL2_FUNC1 对话框

class CPGPL2_FUNC1 : public CDialogEx
{
	DECLARE_DYNAMIC(CPGPL2_FUNC1)

public:
	CPGPL2_FUNC1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPGPL2_FUNC1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_FUNCTION_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		BOOL OnInitDialog();
	void OnBnClickedOk();
	BOOL SetTipText(UINT id, NMHDR* pTTTStruct, LRESULT* pResult);
	void TipInit();
	CButton minicondaCheckbox;
	CButton m_gitCheckbox;
	CButton m_githubAccCheckbox;
	CEdit m_tipsEDIT;
	CToolTipCtrl m_TipMsg;
	void EndDialog();
	CButton m_vcrCheckbox;
	CButton m_vcbCheckbox;
	CEnvCheckTipsDlg *envDlg;
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
};
