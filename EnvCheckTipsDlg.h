﻿#pragma once
#include "afxdialogex.h"


// CEnvCheckTipsDlg 对话框

class CEnvCheckTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvCheckTipsDlg)

public:
	CEnvCheckTipsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnvCheckTipsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENV_CHECK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
