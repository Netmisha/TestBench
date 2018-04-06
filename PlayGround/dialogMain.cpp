#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"
#include <thread>


class CErrorCascade:
    public CDialog
{
public:
    enum {IDD = IDD_DERROR};
    CErrorCascade() :
        CDialog(CErrorCascade::IDD)
    {}
    CErrorCascade(CWnd* parent):
        CDialog(CErrorCascade::IDD, parent)
    {}
    ~CErrorCascade()
    {}
    DECLARE_MESSAGE_MAP();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        if (CDialog::OnCreate(lpCreateStruct) == -1)
            return -1;

        static int i = 0;
        CRect pos;
        this->GetParent()->GetWindowRect(pos);
        pos.left += 50;
        pos.top += 50;
        pos.right += 50;
        pos.bottom += 50;
        if (i++ < 1000)
        {
            CErrorCascade* ec = new CErrorCascade(this);
            ec->Create(IDD_DERROR, this);
            ec->SetWindowPos(this->GetParent(), 50, 50, 50, 50, SW_SHOW);
            ec->ShowWindow(SW_NORMAL);
        }

        return 0;
    }

};

BEGIN_MESSAGE_MAP(CErrorCascade, CDialog)
    ON_WM_CREATE()
END_MESSAGE_MAP();



class CMyDialog : 
    public CDialog
{
public:
    enum { IDD = IDD_DIALOG1 };

    CMyDialog()
        : CDialog(CMyDialog::IDD)
    {}
    ~CMyDialog()
    {}

    DECLARE_MESSAGE_MAP();

    afx_msg void OnClose()
    {
        CDialog::OnClose();
    }
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        if (CDialog::OnCreate(lpCreateStruct) == -1)
            return -1;

        // TODO:  Add your specialized creation code here
        /*CMyDialog dl;
        dl.DoModal();*/
        
        CErrorCascade ec(nullptr);
        ec.DoModal();
        return 0;
    }

    afx_msg void OnBnClickedOk();
};

class CDialogApp :
    public CWinApp
{
public:
    BOOL InitInstance()
    {
        CMyDialog Dlg;

        m_pMainWnd = &Dlg;
        Dlg.DoModal();

        return TRUE;
    }
    DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, &CMyDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDialogApp, CWinApp)

END_MESSAGE_MAP()

CDialogApp theApp;

void CMyDialog::OnBnClickedOk()
{
    {
        this->SetWindowTextW(CString(L"Topkek"));
    }
    //CDialog::OnOK();
}
