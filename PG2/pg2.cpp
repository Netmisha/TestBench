#include "pg2.h"

BEGIN_MESSAGE_MAP(Cpg2App, CWinApp)
END_MESSAGE_MAP()
BOOL Cpg2App::InitInstance()
{
    HCURSOR crossCursor = AfxGetApp()->LoadCursor(IDC_POINTER);
    SetCursor(crossCursor);

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDI_PG2_MAINFRAME,
        RUNTIME_CLASS(Cpg2Doc),
        RUNTIME_CLASS(Cpg2MainFrame),
        RUNTIME_CLASS(Cpg2View));
    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    //CChildFrame ch;

    //LPCTSTR RWC = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, 0, (HBRUSH)(RGB(~0, 0, 0)));
    //CRect   frameRect = CRect(200, 120, 640, 400);
    //ch.Create(RWC, L"PlayGround", WS_CHILD | WS_VISIBLE, frameRect, NULL);
    //ch.ShowWindow(SW_NORMAL);

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
BOOL Cpg2MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.cx = 900;
    cs.cy = 450;
    return CFrameWnd::PreCreateWindow(cs);
}

INT CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    //AfxSetResourceHandle(::AfxGetInstanceHandle())->LoadFrame()
   

    return TRUE;
}
IMPLEMENT_DYNCREATE(Cpg2MainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(Cpg2MainFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(Cpg2Doc, CDocument)
BEGIN_MESSAGE_MAP(Cpg2Doc, CDocument)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(Cpg2View, CView)
BEGIN_MESSAGE_MAP(Cpg2View, CView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE (CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP   (CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


int Cpg2MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == 0)
        return 0;
   
    return -1;
}

Cpg2App theApp;

void Cpg2View::OnDraw(CDC * pDC)
{
    Cpg2Doc* doc = dynamic_cast<Cpg2Doc*>(GetDocument());

    CView::OnDraw(pDC);

    CPen pen(PS_SOLID, 1, RGB(0xff, 0, 0));
    CBrush bruh(HS_DIAGCROSS, RGB(0, 0xff, 0));

    pDC->SelectObject(&pen);
    pDC->SelectObject(&bruh);

    pDC->Polygon(doc->points, 4);
}
