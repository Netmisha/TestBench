#include <afxwin.h>

#include "MyDialog.h"

struct CMainFrame :
    public CFrameWnd
{
    CMainFrame()
    {
        Create(NULL, 
               L"Playground App", 
               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
               CRect(CPoint(300, 300), CSize(900, 675)), 
               NULL, 
               NULL, 
               WS_EX_OVERLAPPEDWINDOW);
    }
};

struct CExperiementApp :
    public CWinApp
{
    BOOL InitInstance()
    {
        CMainFrame* frame = new CMainFrame();
        m_pMainWnd = frame;

        frame->ShowWindow(SW_NORMAL);
        frame->UpdateWindow();

        return TRUE;
    }
};

CExperiementApp theApp;