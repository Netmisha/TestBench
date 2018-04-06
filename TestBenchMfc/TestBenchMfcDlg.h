
// TestBenchMfcDlg.h : header file
//

#pragma once


// CTestBenchMfcDlg dialog
class CTestBenchMfcDlg : public CDialogEx
{
// Construction
public:
	CTestBenchMfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTBENCHMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//    afx_msg void OnBnClickedPointGroupD();
//    afx_msg void OnBnDoubleclickedPointGroupD();
    afx_msg void OnHelpAbout();
//    afx_msg void OnBnClickedPointGroupD();
    afx_msg void OnNeverseenbeforeKek();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnDoubleclickedOk();
    afx_msg void OnEditCleanall();
};
