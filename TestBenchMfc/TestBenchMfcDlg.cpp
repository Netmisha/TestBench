
// TestBenchMfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestBenchMfc.h"
#include "TestBenchMfcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnHelpAbout();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_COMMAND(ID_HELP_ABOUT, &CAboutDlg::OnHelpAbout)
END_MESSAGE_MAP()


// CTestBenchMfcDlg dialog



CTestBenchMfcDlg::CTestBenchMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTBENCHMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestBenchMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestBenchMfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//    ON_BN_CLICKED(IDC_POINT_GROUP_D, &CTestBenchMfcDlg::OnBnClickedPointGroupD)
//    ON_BN_DOUBLECLICKED(IDC_POINT_GROUP_D, &CTestBenchMfcDlg::OnBnDoubleclickedPointGroupD)
ON_COMMAND(ID_HELP_ABOUT, &CTestBenchMfcDlg::OnHelpAbout)
//ON_BN_CLICKED(IDC_POINT_GROUP_D, &CTestBenchMfcDlg::OnBnClickedPointGroupD)
ON_COMMAND(ID_NEVERSEENBEFORE_KEK, &CTestBenchMfcDlg::OnNeverseenbeforeKek)
ON_BN_CLICKED(IDOK, &CTestBenchMfcDlg::OnBnClickedOk)
ON_BN_DOUBLECLICKED(IDOK, &CTestBenchMfcDlg::OnBnDoubleclickedOk)
ON_COMMAND(ID_EDIT_CLEANALL, &CTestBenchMfcDlg::OnEditCleanall)
ON_BN_CLICKED(IDC_SORT_VERTICES, &CTestBenchMfcDlg::OnBnClickedSortVertices)
END_MESSAGE_MAP()


// CTestBenchMfcDlg message handlers

BOOL CTestBenchMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    /*InitializeTriangle*/
    {
        triangle.InitTriangle(this, IDC_PICTURE, IDC_POINT_EDIT_A, IDC_POINT_EDIT_B, IDC_POINT_EDIT_C, IDC_POINT_EDIT_D);
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestBenchMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestBenchMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

    /*DrawTriangleIfInitialized*/
    {
        if (triangle.IsInitialized())
            triangle.DrawTriangle();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestBenchMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAboutDlg::OnHelpAbout()
{}

void CTestBenchMfcDlg::OnHelpAbout()
{
    CAboutDlg about;
    about.DoModal();
}

void CTestBenchMfcDlg::OnNeverseenbeforeKek()
{
    CWnd* pGroup = GetDlgItem(IDC_POINT_GROUP_D);
    if (pGroup)
    {
        if (pGroup->IsWindowVisible())
        {
            pGroup->ShowWindow(SW_HIDE);
        }
        else
        {
            pGroup->ShowWindow(SW_SHOW);
        }
        pGroup->UpdateData(FALSE);
    }
    CWnd* pBox = GetDlgItem(IDC_SORT_VERTICES);
    if (pBox)
    {
        pBox->ShowWindow(pBox->IsWindowVisible() ? SW_HIDE : SW_SHOW);
        pBox->UpdateData(FALSE);
    }
    CWnd* pButton = GetDlgItem(IDC_POINT_EDIT_D);
    if (pButton) 
    {
        pButton->ShowWindow(pButton->IsWindowVisible() ? SW_HIDE : SW_SHOW);
        pButton->UpdateData(FALSE);
    }
    
}

void CTestBenchMfcDlg::OnBnClickedOk()
{
    WCHAR* textChangeValue[] = { L"OK", L"KO"};
    static INT currentButtonText = 0;

    GetDlgItem(IDOK)->SetWindowTextW(textChangeValue[currentButtonText++]);
    currentButtonText %= sizeof(textChangeValue) / sizeof(WCHAR*);

    triangle.UpdateTriangle();
    triangle.DrawTriangle();
}


void CTestBenchMfcDlg::OnBnDoubleclickedOk()
{}


void CTestBenchMfcDlg::OnEditCleanall()
{
    INT items[] = { 
        IDC_POINT_EDIT_A, 
        IDC_POINT_EDIT_B, 
        IDC_POINT_EDIT_C, 
        IDC_POINT_EDIT_D };

    for (int i = 0; i < sizeof(items) / sizeof(INT); ++i)
    {
        GetDlgItem(items[i])->SetWindowTextW(L"");
    }
}


void CTestBenchMfcDlg::OnBnClickedSortVertices()
{
    triangle.SortVertices();
    triangle.DrawTriangle();
}