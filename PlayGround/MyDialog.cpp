#include "MyDialog.h"

CMyDialog::CMyDialog():
    CDialog(CMyDialog::IDD)
{}

CMyDialog::~CMyDialog()
{}

class CDialogApp :
    public CWinApp
{
public:
    BOOL InitInstance()
    {
        CMyDialog md;
        m_pMainWnd = &md;

        return TRUE;
    }
};

CDialogApp dapp;
