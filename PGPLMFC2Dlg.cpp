
// PGPLMFC2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PGPLMFC2.h"
#include "PGPLMFC2Dlg.h"
#include "afxdialogex.h"
#include "utils.h"
#include "CPGPL2_FUNC1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "managers.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPGPLMFC2Dlg 对话框



CPGPLMFC2Dlg::CPGPLMFC2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PGPLMFC2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// m_pFunc1Dlg = NULL;
}

void CPGPLMFC2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPGPLMFC2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT_INSTALL, &CPGPLMFC2Dlg::OnBnClickedInitInstall)
END_MESSAGE_MAP()


// CPGPLMFC2Dlg 消息处理程序

UINT CPGPLMFC2Dlg::MyThreadFunction(LPVOID pParam) {
	int* p_succ = (int*)pParam;
	int r = launchPGPLCore();
	if (r == true) {
		*p_succ = 1;
		return 1;
	}
	else {
		*p_succ = -1;
		return 0;
	}
}

BOOL CPGPLMFC2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	if (not isPGPLInstalled()) {
		CPGPL2_FUNC1 dlg;
		// m_pFunc1Dlg->Create(IDD_SELECT_FUNCTION_1, this);

		// m_pFunc1Dlg.ShowWindow(SW_SHOW);
		theApp.m_pMainWnd = &dlg;
		this->SendMessage(WM_DESTROY);
		this->SendMessage(WM_CLOSE);
		dlg.DoModal();
		CDialogEx::OnOK();
		return TRUE;
	}
	else {
		bool succ = true;
		succ = launchPGPLCore();
		// int *p_succ=0;
		// CWinThread* MyThread = AfxBeginThread(MyThreadFunction, p_succ, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		// for (int i = 0; i <= 10; i++) {
		//  	Sleep(1000);
		//  
		// }
		if (succ ==true) {
			CDialogEx::OnOK();
		}
		else {

		}
	}
	GetDlgItem(IDC_FULLY_REINSTALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UNINSTALL)->ShowWindow(SW_HIDE);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPGPLMFC2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPGPLMFC2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPGPLMFC2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPGPLMFC2Dlg::OnBnClickedInitInstall()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 创建非模态对话框实例   
	CPGPL2_FUNC1 m_pFunc1Dlg;
	// m_pFunc1Dlg->Create(IDD_SELECT_FUNCTION_1, this);
	// 显示非模态对话框   

	// m_pFunc1Dlg.ShowWindow(SW_SHOW);
	theApp.m_pMainWnd = &m_pFunc1Dlg;
	this->SendMessage(WM_DESTROY);
	this->SendMessage(WM_CLOSE);
	m_pFunc1Dlg.DoModal();
		
	// 将各控件中的数据保存到相应的变量   

	// 将被加数和加数的加和赋值给m_editSum   

	// 根据各变量的值更新相应的控件。和的编辑框会显示m_editSum的值   
	// UpdateData(FALSE);
	
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
