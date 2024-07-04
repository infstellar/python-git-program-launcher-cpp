
// PGPLMFC2Dlg.h: 头文件
//

#pragma once
#include "CPGPL2_FUNC1.h"

// CPGPLMFC2Dlg 对话框
class CPGPLMFC2Dlg : public CDialogEx
{
// 构造
public:
	CPGPLMFC2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PGPLMFC2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	static UINT MyThreadFunction(LPVOID pParam);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInitInstall();
};
