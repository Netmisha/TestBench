#include <afxwin.h>
#include <afxdlgs.h>
#include <afxcmn.h>  
#include <afxext.h>
//#include "MyDialog.h"
//
//
////class CMainDialog :
////    public CDialog
////{
////public:
////    CMainDialog()
////    {
////        DLGTEMPLATE tmpl{ WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN , 0,1,10,10,100,100 };
////        /*Create(NULL,
////               L"Playground App",
////               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
////               CRect(CPoint(300, 300), CSize(900, 675)),
////               NULL,
////               NULL,
////               WS_EX_OVERLAPPEDWINDOW);*/
////        m_hWnd = CreateDialogIndirect(nullptr, &tmpl, nullptr, nullptr);
////        ::SetForegroundWindow(m_hWnd);
////
////    }
////public:
////    virtual BOOL OnInitDialog() override
////    {
////        CDialog::OnInitDialog();
////
////        CRect rect(85, 110, 180, 210);
////        m_edit.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER, rect, this, );
////        m_edit.SetFocus();
////
////        return TRUE;
////    }
////protected:
////    CRect m_rekt;
////    CEdit m_edit;
////};
//
////class CMainFrame :
////    public CFrameWnd
////{
////public:
////    CMainFrame()
////    {
////        //CFrameWnd::cr
////        /*Create(NULL, 
////               L"Playground App", 
////               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
////               CRect(CPoint(300, 300), CSize(900, 675)), 
////               NULL, 
////               NULL, 
////               WS_EX_OVERLAPPEDWINDOW);*/
////    }
////
////    DECLARE_MESSAGE_MAP();
////   // afx_msg BOOL OnInit
////public:
////    BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) override
////    {
////        CCreateContext context;
////        context.m_pNewViewClass = RUNTIME_CLASS(CONTROL_PANEL_VIEW);
////        context.m_pCurrentDoc = NULL;
////        view = static_cast<CONTROL_PANEL_VIEW*>(CreateView(&context));
////        ASSERT(view  && view->IsKindOf(RUNTIME_CLASS(CONTROL_PANEL_VIEW)));
////    }
////
////};
////
////
//
//
//
//class CMainDialog :
//    public CDialog
//{
//public: 
//    CMainDialog():
//        CDialog()
//    {
//    }
//public:
//    virtual BOOL OnInitDialog() override
//    {
//        CDialog::OnInitDialog();
//
//        DLGTEMPLATE tmpl;
//
//        tmpl.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
//        tmpl.x = 10; tmpl.y = 10; tmpl.cx = 100; tmpl.cy = 100;
//
//        this->CreateDlgIndirect(&tmpl, this, ::GetModuleHandle(0));
//        
//        this->ShowWindow(SW_SHOW);
//
//        return FALSE;
//    }
//};
//class CMainApp :
//    public CWinApp
//{
//public:
//    //CMainApp()
//    //{
//    //    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
//    //}
//public:
//    virtual BOOL InitInstance() override
//    {
//        AfxSetAmbientActCtx(FALSE);
//        // Remainder of function definition omitted.
//
//        CWinApp::InitInstance();
//
//        // Initialize OLE libraries
//        if (!AfxOleInit())
//        {
//            AfxMessageBox(_T("OleInit failed."));
//            return FALSE;
//        }
//
//        // Standard initialization
//        // If you are not using these features and wish to reduce the size
//        // of your final executable, you should remove from the following
//        // the specific initialization routines you do not need
//        // Change the registry key under which our settings are stored
//        // TODO: You should modify this string to be something appropriate
//        // such as the name of your company or organization
//        SetRegistryKey(_T("Local AppWizard-Generated Applications"));
//        LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
//                                    // Register the application's document templates.  Document templates
//                                    //  serve as the connection between documents, frame windows and views
//        CMultiDocTemplate* pDocTemplate;
//        pDocTemplate = new CMultiDocTemplate(IDR_MFCListViewTYPE,
//                                             RUNTIME_CLASS(CMFCListViewDoc),
//                                             RUNTIME_CLASS(CChildFrame), // custom MDI child frame
//                                             RUNTIME_CLASS(CMyListView));
//        if (!pDocTemplate)
//            return FALSE;
//        AddDocTemplate(pDocTemplate);
//
//        // create main MDI Frame window
//        CMainFrame* pMainFrame = new CMainFrame;
//        if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
//        {
//            delete pMainFrame;
//            return FALSE;
//        }
//        m_pMainWnd = pMainFrame;
//        // call DragAcceptFiles only if there's a suffix
//        //  In an MDI app, this should occur immediately after setting m_pMainWnd
//
//
//        // Parse command line for standard shell commands, DDE, file open
//        CCommandLineInfo cmdInfo;
//        ParseCommandLine(cmdInfo);
//
//
//        // Dispatch commands specified on the command line.  Will return FALSE if
//        // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
//        if (!ProcessShellCommand(cmdInfo))
//            return FALSE;
//        // The main window has been initialized, so show and update it
//        pMainFrame->ShowWindow(m_nCmdShow);
//        pMainFrame->UpdateWindow();
//
//        return TRUE;
//    }
//
//};
//
//CMainApp theApp;

#include "resource.h"
#include <vector>

struct CMainFrame :
   public CFrameWnd
{
    CMainFrame()
    {
        HCURSOR crossCursor = AfxGetApp()->LoadCursor(IDC_POINTER);
        HBRUSH  blackBrush  = (HBRUSH)GetStockObject(BLACK_BRUSH);
        HICON   someIcon    = AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION);
        LPCTSTR RWC = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, crossCursor, blackBrush, someIcon);
        CRect   frameRect = CRect(200, 120, 640, 400);
        Create(RWC, L"PlayGround", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, frameRect, NULL);
    }
    DECLARE_DYNCREATE(CMainFrame);
public:
    #ifdef _DEBUG
    virtual void AssertValid() const override
    {
        CFrameWnd::AssertValid();
    }
    virtual void Dump(CDumpContext& dc) const override
    {
        CFrameWnd::Dump(dc);
    }
    #endif // _DEBUG
    DECLARE_MESSAGE_MAP();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint p)
    {
        this->GetDC()->SetPixel(p, RGB(~0, 0, 0));
        CFrameWnd::OnLButtonDown(nFlags, p);

        CRect wndRect;
        this->GetClientRect(wndRect);
        CSize size(wndRect.Size());
        p.x = (p.x + 0.0) * 100 / size.cx;
        p.y = (p.y + 0.0) * 100 / size.cy;
        redDots.push_back(p);
    }
    afx_msg void OnMouseMove(UINT nFlags, CPoint p)
    {
        if (nFlags & MK_LBUTTON)
        {
            OnLButtonDown(nFlags, p);
        }

        CFrameWnd::OnMouseMove(nFlags, p);
    }
    afx_msg void OnActivate(UINT nFlags, CWnd* other, BOOL bMinimize)
    {
       CFrameWnd::OnActivate(nFlags, other, bMinimize);
       // TODO: Add your message handler code here
       switch (nFlags)
       {
           case WA_ACTIVE:      SendMessage(WM_SETTEXT, 0, (LPARAM)L"Wnd's been activated, without the mouse!");           break;
           case WA_INACTIVE:    SendMessage(WM_SETTEXT, 0, (LPARAM)L"Wnd's been deactivated and cannot be changed now!!"); break;
           case WA_CLICKACTIVE: SendMessage(WM_SETTEXT, 0, (LPARAM)L"Wnd's been activated using the mouse!!!");            break;
       }
    }
    afx_msg void OnPaint()
    {
        CFrameWnd::OnPaint();
        CRect wndRect;
        this->GetClientRect(wndRect);
        CSize size(wndRect.Size());
        for (CPoint& p: redDots)
        {
            CPoint dr = p;
            dr.x = (size.cx + 0.0) * dr.x / 100;
            dr.y = (size.cy + 0.0) * dr.y / 100;
            this->GetDC()->SetPixel(dr, RGB(~0, 0, 0));
        }
    }
    afx_msg void OnSize(UINT nFlags, INT cx, INT cy)
    {
        CFrameWnd::OnSize(nFlags, cx, cy);
    }
protected:
    std::vector<CPoint> redDots;
};
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd);
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_ACTIVATE()
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP();


struct CExperiementApp :
   public CWinApp
{
    virtual BOOL InitInstance() override
    {
        CWinApp::InitInstance();

        m_pMainWnd = new CMainFrame();
        {
            m_pMainWnd->ShowWindow(SW_NORMAL);
            m_pMainWnd->UpdateWindow();
            MessageBox(m_pMainWnd->GetSafeHwnd(),_T("bem-belem!"), _T("topkek"), MB_OK);
        }
        
        return TRUE;
    }
};

CExperiementApp theApp;