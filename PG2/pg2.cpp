#include "pg2.h"

#ifndef C_MAIN_APP
BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
END_MESSAGE_MAP()
BOOL CMainApp::InitInstance()
{
    CWinApp::InitInstance();

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDI_PG2_MAINFRAME,
        RUNTIME_CLASS(CTriangleDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CMainView));
    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
#endif // !C_MAIN_APP

#ifndef C_MAIN_FRAME
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_BN_CLICKED(buttonId, CMainFrame::OnButtonClickedKEK)
END_MESSAGE_MAP()

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    m_Rect = CRect(CPoint(200,220), CSize(800, 500));

    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    cs.lpszName = _T("TestBench");
    
    cs.style = NULL;
    {
        cs.style |= WS_VISIBLE;
        cs.style |= WS_OVERLAPPEDWINDOW;
        cs.style |= WS_SYSMENU;
        cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    }

    cs.x  = m_Rect.left;
    cs.y  = m_Rect.top;
    cs.cx = m_Rect.Size().cx;
    cs.cy = m_Rect.Size().cy;

    return TRUE;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect client;
    {
        GetClientRect(client);

        canvas.m_Rect = client;
        canvas.m_Rect.DeflateRect(10, 10, 5 + client.Size().cx / 4, 10);
        canvas.Create(this);

        panel.m_Rect = client;
        panel.m_Rect.DeflateRect(5 + client.Size().cx * 3 / 4, 10, 10, 10);
        panel.Create(this);
        panel.BringWindowToTop();
    }

    panel.buttOk.BringWindowToTop();

    return 0;
}
void CMainFrame::OnButtonClickedKEK()
{
    MessageBox(_T("U clicked!"), _T("msg"));
}


BOOL CMainFrame::CCanvas::Create(CWnd* parent)
{
    BOOL result;
    {
        LONG style = WS_CHILD | WS_VISIBLE | SS_WHITERECT | WS_BORDER;
        result = CStatic::Create(NULL, style, m_Rect, parent, CCanvas::ID);
    }
    return result;
}

BEGIN_MESSAGE_MAP(CMainFrame::CPanel, CStatic)
    ON_WM_CREATE()
END_MESSAGE_MAP()
BOOL CMainFrame::CPanel::Create(CWnd * parent)
{
    BOOL result;
    {
        LONG style = WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_BORDER;
        result = CButton::Create(_T("Control Panel"), style, m_Rect, parent, CPanel::ID);
    }
    return result;
}
BOOL CMainFrame::CPanel::RedrawWindow()
{
    BOOL result = CButton::RedrawWindow();

    for (int i = 0; i < 3; ++i)
        editPoints[i].RedrawWindow();

    buttOk.RedrawWindow();

    return result;
}
INT CMainFrame::CPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRect rect;
    {
        GetClientRect(rect);
        rect.DeflateRect(10, 10, 10, 10 + rect.Size().cy * (5.0 / 6.0));
        rect.MoveToY(30);
    }

    for (int i = 0; i < 3; ++i)
    {
        editPoints[i].Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, CtrId::id_edit_point + i);
        rect.MoveToY(rect.bottom + 20);
    }

    rect;
    {
        GetClientRect(rect);
        rect.DeflateRect(10, 10 + rect.Size().cy * (5.0 / 6.0), 10, 10);
    }
    buttOk.Create(L"OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER, rect, this, CtrId::id_button_ok);

    return true;
}

#endif // !C_MAIN_FRAME







IMPLEMENT_DYNCREATE (CTriangleDoc, CDocument)
BEGIN_MESSAGE_MAP   (CTriangleDoc, CDocument)   
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE (CMainView, CView)
BEGIN_MESSAGE_MAP   (CMainView, CView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE (CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP   (CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()
//
//BEGIN_MESSAGE_MAP(CMainFrame::CCanvas, CStatic)
//END_MESSAGE_MAP()




CMainApp theApp;

void CMainView::OnDraw(CDC * pDC)
{
    //CTriangleDoc* doc = dynamic_cast<CTriangleDoc*>(GetDocument());
    //
    //CPen pen(PS_SOLID, 1, RGB(0xff, 0, 0));
    //CBrush bruh(HS_DIAGCROSS, RGB(0, 0xff, 0));

    //pDC->SelectObject(&pen);
    //pDC->SelectObject(&bruh);

    //pDC->Polygon(doc->points, 4);

    CBrush br;
    br.CreateSolidBrush(RGB(192,192,192));
    pDC->SelectObject(br);
    CRect rect;
    pDC->GetClipBox(&rect);
    pDC->Rectangle(rect);

    auto frame = (CMainFrame*)GetParentFrame();
    frame->canvas.RedrawWindow();
    frame->panel.RedrawWindow();

    frame->UpdateWindow();

    CView::OnDraw(pDC);
}

BOOL CTriangleDoc::OnNewDocument()
{
    CDocument::OnNewDocument();

    points[0] = /*            */  CPoint(20, 20);
    points[1] = /*            */  CPoint(120, 45);
    points[2] = points[3] = /*CPoint(0,0);*/  CPoint(350, 350);

    return true;;
}
