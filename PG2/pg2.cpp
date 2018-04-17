#include "pg2.h"
#include <atlstr.h>

#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
using String = std::basic_string<WCHAR>;
using Regex = std::basic_regex<WCHAR>;
using StringStream = std::wstringstream;


CPoint CStringToCPoint(const CString& str)
{
    CPoint cp;

    String input = std::regex_replace(str.GetString(), Regex(L"[^-0-9]"), L" ");
    {
        StringStream ss(input);
        ss >> cp.x >> cp.y;
    }

    return cp;
}
#define GetMainFrame()      [this]()->CMainFrame&    { return *dynamic_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd); }        ()
#define GetMainView()       [this]()->CMainView&     { return *dynamic_cast<CMainView*> (GetMainFrame().GetActiveView()); } ()
#define GetMainDocument()   [this]()->CMainDocument& { return *dynamic_cast<CMainDocument*>(GetMainView().GetDocument()); } ()


#ifndef C_MAIN_APP
    BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
        ON_COMMAND(ID_FILE_EXIT, &CMainApp::CWinApp::OnAppExit)
        ON_COMMAND(ID_HELP_BASICS, &CMainApp::OnHelpBasics)
        ON_COMMAND(ID_HELP_ABOUTTESTBENCH, &CMainApp::OnHelpAbouttestbench)
        ON_COMMAND(ID_FILE_SAVEAS, &CMainApp::OnFileSaveAs)
        ON_COMMAND(ID_FILE_NEW, &CMainApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, &CMainApp::OnFileOpen)
    END_MESSAGE_MAP()
    BOOL CMainApp::InitInstance()
    {
        CWinApp::InitInstance();
    
        CSingleDocTemplate* pDocTemplate;
        pDocTemplate = new CSingleDocTemplate(
            IDI_PG2_MAINFRAME,
            RUNTIME_CLASS(CMainDocument),
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

    void CMainApp::OnFileNew()
    {
        auto& frame = GetMainFrame();
        auto& doc = GetMainDocument();
        if (doc.IsModified())
        {
            auto mbResult = MessageBox(NULL, L"Current triangle is not saved. Save as?..", L"Caption", MB_YESNOCANCEL| MB_ICONQUESTION);
            if(mbResult == IDYES)
                doc.DoFileSave();
            if (mbResult == IDCANCEL)
                return;
        }
        doc.OnNewDocument();
        frame.OnEditClear();
    }
    void CMainApp::OnFileOpen()
    {
        auto& frame = GetMainFrame();
        auto& doc = GetMainDocument();

        if (doc.IsModified())
        {
            auto mbResult = MessageBox(NULL, L"Current triangle is not saved. Save as?..", L"Caption", MB_YESNOCANCEL | MB_ICONQUESTION);
            if (mbResult == IDYES)
                doc.DoFileSave();
            if (mbResult == IDCANCEL)
                return;
        }

        doc.DoFileOpen();
        //for (int i = 0; i < doc.m_Points.size(); ++i)
        //{
        //    CPoint p = doc.m_Points[i];
        //    CString pointStr;
        //    {
        //        pointStr.Format(L"( %d,  %d )", p.x, p.y);
        //    }
        //    frame.m_Panel.m_EditPoints[i].SetWindowText(pointStr);
        //}

        frame.m_Canvas.RedrawWindow();
        frame.m_Panel.RedrawWindow();
    }

    void CMainApp::OnFileSaveAs()
    {
        auto& doc = GetMainDocument();
        doc.DoFileSave();
    }


    void CMainApp::OnHelpBasics()
    {
        CString mbText
        {
            "                                   \n"
            " at ControlPanel put point coordinates in (x,y) format \n"
            " or click white area to get points selected automaticaly\n"
            " click OK to get triangle drawn\n"
            " use Edit->Clear to clear the document\n"
            "                                   \n"
        };
        ::MessageBox(NULL,mbText, L"Instructions", MB_OK);
    }
    void CMainApp::OnHelpAbouttestbench()
    {
        CString mbText
        {
            "                                   \n"
            " TestBench, Version 2.0 \n"
            " Copyright(c) 2018\n"
            "                                   \n"
        };
        MessageBox(GetRoutingFrame()->GetSafeHwnd(), mbText, L"About TestBench", MB_OK);
        
    }

#endif // !C_MAIN_APP

#ifndef C_MAIN_FRAME
    IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
    BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
        ON_WM_CREATE()
        ON_WM_LBUTTONDOWN()
        ON_COMMAND(ID_EDIT_CLEAR, &CMainFrame::OnEditClear)
    END_MESSAGE_MAP()
    
    BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    cs.lpszName = _T("TestBench");
    
    cs.style = WS_OVERLAPPEDWINDOW;
    {
        cs.style |= WS_VISIBLE;
        cs.style |= WS_SYSMENU;
        cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    }

    m_Rect = CRect(CPoint(200, 220), CSize(800, 500));
    {
        cs.x  = m_Rect.left;
        cs.y  = m_Rect.top;
        cs.cx = m_Rect.Size().cx;
        cs.cy = m_Rect.Size().cy;
    }

    return TRUE;
}
    int  CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect clientRect;
    CSize clientSize;
    {
        GetClientRect(clientRect);
        clientSize = clientRect.Size();
    }

    m_Canvas;
    {
        m_Canvas.m_Rect = clientRect;
        m_Canvas.m_Rect.DeflateRect(10, 10, 5 + clientSize.cx / 4, 10);
        m_Canvas.Create(this);
        m_Canvas.BringWindowToTop();
    }

    m_Panel;
    {
        m_Panel.m_Rect = clientRect;
        m_Panel.m_Rect.DeflateRect(5 + clientSize.cx * 3 / 4, 10, 10, 10);
        m_Panel.Create(this);
        m_Panel.BringWindowToTop();
    }

    return 0;
}
    void CMainFrame::OnLButtonDown(UINT nFlags, CPoint p)
{
    // superfluous ivent, never procs for now
    // left to be for if i need it one day

    CFrameWnd::OnLButtonDown(nFlags, p);
    MessageBox(L"U clicked");
}    
    void CMainFrame::OnEditClear()
    {
        auto& doc = GetMainDocument();
        while (doc.m_Points.size())
            doc.m_Points.pop_back();

        for (int i = 0; i < 3; ++i)
        {
            m_Panel.m_EditPoints[i].SetWindowTextW(L"");
        }
        
        m_Canvas.RedrawWindow();
        //RedrawWindow();
    }
#ifndef C_MAIN_FRAME_NESTED

#ifndef C_CANVAS
    BEGIN_MESSAGE_MAP(CMainFrame::CCanvas, CStatic)
        ON_WM_LBUTTONDOWN()
    END_MESSAGE_MAP()

    void CMainFrame::CCanvas::OnLButtonDown(UINT nFlags, CPoint mousePt)
    {
        auto& frame = GetMainFrame();
        auto& view = GetMainView();
        auto& doc = GetMainDocument();

        BOOL ptExistsInDoc = FALSE;
        {
            for (int i = 0; i < doc.m_Points.size(); ++i)
            {
                CSize ptsDiff = doc.m_Points[i] - mousePt;
                INT ptsDist = std::abs(std::hypot(ptsDiff.cx, ptsDiff.cy));
                if (ptsDist < 10)
                {
                    ptExistsInDoc = TRUE;
                    doc.m_Points[i] = mousePt;
                    break;
                }
            }
            if (!ptExistsInDoc)
            {
                if (doc.m_Points.size() < 3)
                {
                    doc.m_Points.push_back(mousePt);
                }
                else
                {
                    *std::min_element(
                        doc.m_Points.begin(),
                        doc.m_Points.end(),
                        [&mousePt](const CPoint& cp1, const CPoint& cp2)
                    {
                        CSize sz1 = cp1 - mousePt;
                        CSize sz2 = cp2 - mousePt;

                        return std::abs(std::hypot(sz1.cx, sz1.cy)) < std::abs(std::hypot(sz2.cx, sz2.cy));
                    }) = mousePt;

                }
            }
            view.RedrawWindow();
        }
    }
    BOOL CMainFrame::CCanvas::Create(CWnd* parent)
    {
        BOOL result;
        {
            LONG style = WS_CHILD;
            { 
                style |= WS_VISIBLE;
                style |= SS_WHITERECT;
                style |= WS_BORDER;
                style |= WS_TABSTOP;
            }
            result = CStatic::Create(NULL, style, m_Rect, parent, CtrId::id_canvas);

            m_Pen.CreatePen(PS_SOLID, 1, RGB(0xff, 0, 0));
            m_Brush.CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0xff, 0));
        }
        return result;
    }
    BOOL CMainFrame::CCanvas::RedrawWindow()
    {
        BOOL result = FALSE;
        {
            result = CStatic::RedrawWindow();

            CDC&  dc = *GetDC();
            dc.SelectObject(m_Pen);
            dc.SelectObject(m_Brush);
            
            auto& points = GetMainDocument().m_Points;
            if(points.size()>0)
                dc.Polygon(points.begin()._Ptr, points.size());
        }
        return result;
    }
#endif // !C_CANVAS
#ifndef C_PANEL
    BEGIN_MESSAGE_MAP(CMainFrame::CPanel, CStatic)
        ON_WM_CREATE()
        ON_BN_CLICKED(CtrId::id_button_ok, CMainFrame::CPanel::OnButtonClickOk)
        ON_BN_CLICKED(CtrId::id_check_box, CMainFrame::CPanel::OnButtonClickCp)
        ON_EN_CHANGE(CtrId::id_edit_point, CMainFrame::CPanel::OnEditPointChanged)
        ON_EN_CHANGE(CtrId::id_edit_point+1, CMainFrame::CPanel::OnEditPointChanged)
        ON_EN_CHANGE(CtrId::id_edit_point+2, CMainFrame::CPanel::OnEditPointChanged)
    END_MESSAGE_MAP()

    BOOL CMainFrame::CPanel::Create(CWnd * parent)
    {
        BOOL result;
        {
            LONG style{
                WS_CHILD    |
                WS_VISIBLE  |
                BS_GROUPBOX |
                WS_BORDER   
            };

            result = CButton::Create(_T("Control Panel"), style, m_Rect, parent, CPanel::ID);
        }
        return result;
    }
    BOOL CMainFrame::CPanel::RedrawWindow()
    {
        auto& doc = GetMainDocument();
        BOOL result = FALSE;
        {
            result = CButton::RedrawWindow();

            for (int i = 0; i < doc.m_Points.size(); ++i)
            {
                CPoint p = doc.m_Points[i];
                CString pointStr; 
                    pointStr.Format(L"(%d , %d)", p.x, p.y);
                CString editStr;  
                    m_EditPoints[i].GetWindowText(editStr);

                if (editStr != pointStr)
                    m_EditPoints[i].SetWindowText(pointStr);
            }

            m_CheckPaint.RedrawWindow();

            m_ButtOk.RedrawWindow();
        }
        return result;
    }
    INT  CMainFrame::CPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        INT result;
        {
            result = CButton::OnCreate(lpCreateStruct);

            CRect componentRect;
            {
                GetClientRect(componentRect);
                componentRect.DeflateRect(10, 10, 10, 10 + componentRect.Size().cy * (5.0 / 6.0));
                componentRect.MoveToY(30);
            }

            m_EditPoints;
            {
                for (int i = 0; i < 3; ++i)
                {
                    LONG style{
                        WS_CHILD |
                        WS_VISIBLE |
                        WS_TABSTOP |
                        WS_BORDER |
                        ES_CENTER
                    };

                    m_EditPoints[i].Create(style, componentRect, this, CtrId::id_edit_point + i);

                    componentRect.MoveToY(componentRect.bottom + 20);
                }
            }

            m_CheckPaint;
            {
                componentRect.DeflateRect(55, 0);
                auto hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

                LONG style{
                    WS_CHILD | 
                    WS_VISIBLE | 
                    WS_TABSTOP | 
                    WS_GROUP | 
                    BS_AUTOCHECKBOX | 
                    BS_ICON|
                    TBS_TRANSPARENTBKGND
                };
                m_CheckPaint.Create(L"Paint", style, componentRect, this, CtrId::id_check_box);
                m_CheckPaint.SetIcon(hIcon);
            }

            m_ButtOk;
            {
                componentRect;
                {
                    GetClientRect(componentRect);
                    componentRect.DeflateRect(30, 10 + componentRect.Size().cy * (5.0 / 6.0), 30, 10);
                }

                LONG buttonOkStyle{
                    WS_CHILD |
                    WS_VISIBLE |
                    BS_PUSHBUTTON |
                    WS_BORDER
                };

                m_ButtOk.Create(L"OK", buttonOkStyle, componentRect, this, CtrId::id_button_ok);
            }
        }
        return result;
    }

    void CMainFrame::CPanel::OnButtonClickOk()
    {
        auto& frame = GetMainFrame();
        auto& doc = GetMainDocument();

        doc.UpdateData();

        frame.m_Canvas.RedrawWindow();
    }
    void CMainFrame::CPanel::OnButtonClickCp()
    {
        auto& frame = GetMainFrame();
        auto cbState = frame.m_Panel.m_CheckPaint.GetState();

        //frame.m_Panel.m_ButtOk.EnableWindow(cbState&BST_CHECKED? FALSE:TRUE);

        frame.m_Panel.m_CheckPaint.RedrawWindow();
    }
    void CMainFrame::CPanel::OnEditPointChanged()
    {
        auto& frame = GetMainFrame();
        auto& view = GetMainView();
        auto cbState = frame.m_Panel.m_CheckPaint.GetState();

        if (cbState&BST_CHECKED)
            return;

        auto& doc = GetMainDocument();
        doc.UpdateData();
        //view.RedrawWindow();
        frame.m_Canvas.RedrawWindow();
    }
#endif // !C_PANEL

#endif // !C_MAIN_FRAME_NESTED
#endif // !C_MAIN_FRAME

#ifndef C_MAIN_DOCUMENT
    IMPLEMENT_DYNCREATE(CMainDocument, CDocument)
    BEGIN_MESSAGE_MAP(CMainDocument, CDocument)
    END_MESSAGE_MAP()

    BOOL CMainDocument::OnNewDocument()
    {
        //this->do
        SetPathName(L"%userprofile%\\documents\\");
        m_Points.clear();
        return CDocument::OnNewDocument();
    }
    BOOL CMainDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
    {
        CFileDialog fd(FALSE);

        fd.m_ofn.lpstrFilter = L"Triangle Files(*.tri)\0*.tri\0\0";
        fd.m_ofn.lpstrDefExt = L"tri";
        fd.m_ofn.lpstrTitle = L"Save as Triangle";

        if (fd.DoModal() == IDOK)
        {
            OnSaveDocument(fd.GetPathName());
        }
        
        SetModifiedFlag(FALSE);

        return TRUE;
    }
    BOOL CMainDocument::OnOpenDocument(LPCTSTR lpszPathName)
    {
        CFile f;
        f.Open(lpszPathName, CFile::modeRead);
        m_Points.clear();
        for (int i = 0; i < 3; ++i)
        {
            CPoint p;
            {
                f.Read(&(p.x), sizeof(LONG));
                f.Read(&(p.y), sizeof(LONG));
            }
            m_Points.push_back(p);
        }
        f.Close();
        return TRUE;
    }
    BOOL CMainDocument::OnSaveDocument(LPCTSTR lpszPathName)
    {
        CFile f;
        f.Open(lpszPathName, CFile::modeWrite|CFile::modeCreate);
        for (int i = 0; i < 3; ++i)
        {
            f.Write(&m_Points[i].x, sizeof(LONG));
            f.Write(&m_Points[i].y, sizeof(LONG));
        }
        f.Close();
        return TRUE;
    }
    void CMainDocument::UpdateData()
    {
        auto& frame = GetMainFrame();

        m_Points.clear();
        for (int i = 0; i < 3; ++i)
        {
            CString editPointText;
            frame.m_Panel.m_EditPoints[i].GetWindowText(editPointText);            

            if (!editPointText.IsEmpty())
            {
                m_Points.push_back(CStringToCPoint(editPointText));
            }
        }
        CDocument::SetModifiedFlag(TRUE);
    }
    BOOL CMainDocument::DoFileOpen()
    {
        CFileDialog fd(TRUE);

        fd.m_ofn.lpstrFilter = L"Triangle Files(*.tri)\0*.tri\0\0";
        fd.m_ofn.lpstrDefExt = L"tri";
        fd.m_ofn.lpstrTitle = L"Open Triangle";
        fd.m_ofn.Flags |= OFN_FILEMUSTEXIST;

        if (fd.DoModal() == IDOK)
        {
            OnOpenDocument(fd.GetPathName());
        }

        return TRUE;
    }
#endif // !C_MAIN_DOCUMENT

IMPLEMENT_DYNCREATE (CMainView, CView)
BEGIN_MESSAGE_MAP   (CMainView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


void CMainView::OnDraw(CDC * pDC)
{
    CView::OnDraw(pDC);

    auto& frame = GetMainFrame();
    {
        CBrush bGroundBrush;
        {
            bGroundBrush.CreateSolidBrush(RGB(192, 192, 192));
            pDC->SelectObject(bGroundBrush);
        }
        CRect clientRect;
        {
            frame.GetClientRect(clientRect);
        }
        pDC->Rectangle(clientRect);

        frame.m_Canvas.RedrawWindow();
        frame.m_Panel.RedrawWindow();
    }
}
void CMainView::OnLButtonDown(UINT nFlags, CPoint p)
{
    auto& frame = GetMainFrame();

    auto checkPaintState = frame.m_Panel.m_CheckPaint.GetState();

    if (checkPaintState&BST_CHECKED)
    {
        if (frame.m_Canvas.m_Rect.PtInRect(p))
        {
            CRect clientRect;
            {
                GetClientRect(clientRect);
            }
            p += clientRect.TopLeft() - frame.m_Canvas.m_Rect.TopLeft();

            frame.m_Canvas.OnLButtonDown(nFlags, p);
        }
    }
}

void CMainView::OnMouseMove(UINT nFlags, CPoint p)
{
    CView::OnMouseMove(nFlags, p);

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_HOVER | TME_LEAVE;
    tme.hwndTrack = GetSafeHwnd();

    tme.dwHoverTime = 1;//HOVER_DEFAULT;
    TrackMouseEvent(&tme);
}

void CMainView::OnMouseHover(UINT nFlags, CPoint p)
{
    auto& frame = GetMainFrame();
    auto& doc = GetMainDocument();

    auto checkPaintState = frame.m_Panel.m_CheckPaint.GetState();

    if (checkPaintState&BST_CHECKED)
    {
        if (frame.m_Canvas.m_Rect.PtInRect(p))
        {
            CRect clientRect;
            {
                GetClientRect(clientRect);
            }
            p += clientRect.TopLeft() - frame.m_Canvas.m_Rect.TopLeft();


            for (int i = 0; i < doc.m_Points.size(); ++i)
            {
                CSize diff = doc.m_Points[i] - p;
                INT distance = std::sqrt(std::pow(diff.cx, 2) + std::pow(diff.cy, 2));
                if (distance < 10)
                {
                    CString str;
                    str.Format(L"%d %d", p.x, p.y);
                    SetCursor(::LoadCursor(NULL, IDC_HAND));
                }
            }

            if (nFlags&MK_LBUTTON)
            {
                frame.m_Canvas.OnLButtonDown(nFlags, p);
            }

        }
    }
    CView::OnMouseHover(nFlags, p);
}

void CMainView::OnMouseLeave()
{
    CView::OnMouseLeave();
}

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


#ifndef THE_APP
CMainApp theApp;
#endif

BOOL CMainFrame::CPanel::CCheckBox::RedrawWindow()
{
    CButton::RedrawWindow();
    return 0;
}
