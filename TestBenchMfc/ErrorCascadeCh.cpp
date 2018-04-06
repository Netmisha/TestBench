#include "stdafx.h"
#include "ErrorCascadeCh.h"


CErrorCh::CErrorCh():
    CDialogEx(CErrorCh::IDD)
{}

//INT Ec(LPVOID*)
//{
//    CErrorCh ec;
//    ec.DoModal();
//    return 0;
//}
//
//class CEcThread :
//    public CWinThread
//{
//public:
//    BOOL InitInstance()
//    {
//        return TRUE;
//    }
//    INT Run()
//    {
//        CErrorCh ec;
//        ec.DoModal();
//        return 0;
//    }
//};

int CErrorCh::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

BEGIN_MESSAGE_MAP(CErrorCh, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BUTTON_IMOK, &CErrorCh::OnBnClickedButtonImok)
END_MESSAGE_MAP();

void CErrorCh::OnBnClickedButtonImok()
{
    CDialogEx::OnCancel();
}
