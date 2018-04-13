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

#ifndef C_MAIN_APP
    BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
        ON_COMMAND(ID_FILE_EXIT, &CMainApp::OnFileExit)
        ON_COMMAND(ID_HELP_BASICS, &CMainApp::OnHelpBasics)
        ON_COMMAND(ID_HELP_ABOUTTESTBENCH, &CMainApp::OnHelpAbouttestbench)
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

    void CMainApp::OnFileExit()
    {
        exit(CWinApp::ExitInstance());
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
        ((CMainDocument*)GetActiveView()->GetDocument())->m_Points.clear();

        for (int i = 0; i < 3; ++i)
        {
            m_Panel.editPoints[i].SetWindowTextW(L"");
        }
        
        m_Canvas.RedrawWindow();
        //RedrawWindow();
    }
#ifndef C_MAIN_FRAME_NESTED
    #define GetMainFrame()      [this]()->CMainFrame&    { return *dynamic_cast<CMainFrame*>(GetParentFrame()); }               ()
    #define GetMainView()       [this]()->CMainView&     { return *dynamic_cast<CMainView*> (GetMainFrame().GetActiveView()); } ()
    #define GetMainDocument()   [this]()->CMainDocument& { return *dynamic_cast<CMainDocument*>(GetMainView().GetDocument()); } ()

#ifndef C_CANVAS
    BEGIN_MESSAGE_MAP(CMainFrame::CCanvas, CStatic)
        ON_WM_LBUTTONDOWN()
    END_MESSAGE_MAP()

    void CMainFrame::CCanvas::OnLButtonDown(UINT nFlags, CPoint p)
    {
        static INT i = 0;
        {
            auto& frame = GetMainFrame();
            auto& view = GetMainView();
            auto& doc = GetMainDocument();

            CString str;
            str.Format(L"(%d, %d)", p.x, p.y);

            for (int j = 0; j < 3; ++j)
            {
                CString strOld;
                if (frame.m_Panel.editPoints[j].GetWindowTextLengthW() == 0)
                {
                    frame.m_Panel.editPoints[j].SetWindowTextW(str);
                    return;
                }
            }

            frame.m_Panel.editPoints[i].SetWindowTextW(str);

            //doc.m_Points[i] = p;
            doc.m_Points.push_back(p);
        }
        ++i %= 3;
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
        BOOL result;
        {
            result = CStatic::RedrawWindow();

            CDC&  dc = *GetDC();
            dc.SelectObject(m_Pen);
            dc.SelectObject(m_Brush);
            
            auto& points = GetMainDocument().m_Points;
            dc.Polygon(points.begin()._Ptr, points.size());
        }
        return result;
    }
#endif // !C_CANVAS
#ifndef C_PANEL
    BEGIN_MESSAGE_MAP(CMainFrame::CPanel, CStatic)
        ON_WM_CREATE()
        ON_BN_CLICKED(CtrId::id_button_ok, CMainFrame::CPanel::OnButtonClickOk)
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
        BOOL result;
        {
            result = CButton::RedrawWindow();

            for (int i = 0; i < 3; ++i)
                editPoints[i].RedrawWindow();

            buttOk.RedrawWindow();
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

            editPoints;
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

                    editPoints[i].Create(style, componentRect, this, CtrId::id_edit_point + i);

                    componentRect.MoveToY(componentRect.bottom + 20);
                }
            }

            buttOk;
            {
                componentRect;
                {
                    GetClientRect(componentRect);
                    componentRect.DeflateRect(10, 10 + componentRect.Size().cy * (5.0 / 6.0), 10, 10);
                }

                LONG buttonOkStyle{
                    WS_CHILD |
                    WS_VISIBLE |
                    BS_PUSHBUTTON |
                    WS_BORDER
                };

                buttOk.Create(L"OK", buttonOkStyle, componentRect, this, CtrId::id_button_ok);
            }
        }
        return result;
    }

    CPoint CStringToCPoint(const CString& str)
    {
        CPoint cp;

        String input = std::regex_replace(str.GetString(), Regex(L"[^ 0-9]"), L" ");
        if (std::regex_match(input, Regex(L"[ ]*[0-9]+[ ]+[0-9]+[ ]*")))
        {
            StringStream ss(input);
            ss >> cp.x >> cp.y;
        }
        else cp = { 0, 0 };

        return cp;
    }

    void CMainFrame::CPanel::OnButtonClickOk()
    {
        auto& frame = GetMainFrame();
        auto& view  = GetMainView();
        auto& doc   = GetMainDocument();

        CString strPoints[3];
        for (int i = 0; i < 3; ++i)
        {
            editPoints[i].GetWindowTextW(strPoints[i]);
            if (strPoints[i].IsEmpty())
                return;
        }

        doc.m_Points.clear();
        for (int i = 0; i < 3; ++i)
        {
            doc.m_Points.push_back(CStringToCPoint(strPoints[i]));
        }

        //{
        //    if (doc.m_Points.size() == 3)
        //        doc.m_Points.clear();
        //    for (int i = 0; i < 3; ++i)
        //    {
        //        CPoint p = CStringToCPoint(strPoints[i]);
        //        doc.m_Points.insert(doc.m_Points.begin(), p);
        //    }

        //    auto cmpX = [](CPoint&a, CPoint&b) { return a.x < b.x; };
        //    auto cmpY = [](CPoint&a, CPoint&b) { return a.y < b.y; };

        //    CPoint c(std::accumulate(doc.m_Points.begin(), doc.m_Points.end(), 0, [](int& val, CPoint& p) {return val + p.x;}) / 4,
        //             std::accumulate(doc.m_Points.begin(), doc.m_Points.end(), 0, [](int& val, CPoint& p) {return val + p.y;}) / 4);

        //    std::sort(doc.m_Points.begin(), doc.m_Points.end(), [&](CPoint& a, CPoint&b)
        //    {
        //        if (!cmpX(a, c) && cmpX(b, c))
        //            return true;
        //        if (cmpX(a, c) && !cmpX(b, c))
        //            return false;
        //        if ((!cmpX(a, c) && !cmpX(c, a)) && (!cmpX(b, c) && !cmpX(c, b)))
        //            if (!cmpY(a, c) || !cmpY(b, c))
        //                return cmpY(b, a);
        //            else
        //                return cmpY(a, b);

        //        //auto det = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
        //        //if (det < 0)
        //        //    return true;
        //        //if (det > 0)
        //        //    return false;

        //        int d1 = (a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y);
        //        int d2 = (b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y);
        //        return d1 > d2;
        //    });
        //}

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
        CDocument::OnNewDocument();
        //for (int i = 0; i < 3; m_Points[i++] = CPoint(0, 0));
        return TRUE;
    }
#endif // !C_MAIN_DOCUMENT



IMPLEMENT_DYNCREATE (CMainView, CView)
BEGIN_MESSAGE_MAP   (CMainView, CView)
    ON_WM_LBUTTONDOWN()
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
    CMainFrame& frame = *dynamic_cast<CMainFrame*>(GetParentFrame());
    {
        // bug: 
        // when (x->max, y) or (x, y->max) 
        // triangle is drawn outside of frame::canvas

        if (frame.m_Canvas.m_Rect.PtInRect(p))
        {
            //! solution:
            //CRect clientRect;
            //{
            //    GetClientRect(clientRect);
            //}
            //p += clientRect.TopLeft() - frame.m_Canvas.m_Rect.TopLeft();

            frame.m_Canvas.OnLButtonDown(nFlags, p);
        }
    }
}

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


#ifndef THE_APP
CMainApp theApp;
#endif