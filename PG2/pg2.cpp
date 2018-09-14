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

#include "xmlHelpIssues.h"


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
#define GetMainApp()        [this]()->CMainApp&      { return *dynamic_cast<CMainApp*>(AfxGetApp());}                       ()
#define GetMainFrame()      [this]()->CMainFrame&    { return *dynamic_cast<CMainFrame*>(GetMainApp().m_pMainWnd); }        ()
#define GetMainView()       [this]()->CMainView&     { return *dynamic_cast<CMainView*> (GetMainFrame().GetActiveView()); } ()
#define GetMainDocument()   [this]()->CMainDocument& { return *dynamic_cast<CMainDocument*>(GetMainView().GetDocument()); } ()


#ifndef C_MAIN_APP
    BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
        ON_COMMAND(ID_FILE_EXIT,   &CMainApp::CWinApp::OnAppExit)
        ON_COMMAND(ID_HELP_BASICS, &CMainApp::OnHelpBasics)
        ON_COMMAND(ID_HELP_ABOUTTESTBENCH, &CMainApp::OnHelpAboutTestBench)
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

        frame.m_Canvas.RedrawWindow();
        frame.m_Panel.RedrawWindow();
    }
    void CMainApp::OnAppExit()
    {
        if (m_RunningPointsThreadPtr)
        {
            m_RunningPointsThreadPtr->SuspendThread();
            delete m_RunningPointsThreadPtr;
        }
        CWinApp::OnAppExit();
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
            "at ControlPanel put point coordinates in (x,y) format \n"
            "click OK to get triangle drawn\n"
            "or select a fancy button over ok \n"
            "and click white area to get points selected automaticaly\n"            
            "use Edit->Clear to clear the document\n"
            "u can create/save/open triangle documents in File menu"
            "have fun"
            "                                   \n"
        };
        ::MessageBox(NULL,mbText, L"Instructions", MB_OK | MB_ICONINFORMATION);
    }
    void CMainApp::OnHelpAboutTestBench()
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
    void CMainFrame::OnHelpSomeIssues()
    {
        CIssueFrame* issFrmPtr = new CIssueFrame;
        issFrmPtr->Create(static_cast<CWnd*>(this));
        issFrmPtr->ShowWindow(SW_SHOW);
    }

#endif // !C_MAIN_APP

#ifndef C_MAIN_FRAME
    IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
    BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
        ON_WM_CREATE()
        ON_WM_LBUTTONDOWN()
        ON_COMMAND(ID_EDIT_CLEAR, &CMainFrame::OnEditClear)
        ON_COMMAND(ID_HELP_SOMEISSUES, &CMainFrame::OnHelpSomeIssues)
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

    m_OnEditClear = FALSE;
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
        m_OnEditClear = TRUE;
        {
            auto& frame = GetMainFrame();
            auto& doc = GetMainDocument();
            auto& view = GetMainView();

            doc.m_Points.clear();

            view.RedrawWindow();
            frame.m_Canvas.RedrawWindow();
            frame.m_Panel.RedrawWindow();
        }
        m_OnEditClear = FALSE;
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
            for (UINT i = 0; i < doc.m_Points.size(); ++i)
            {
                CSize ptsDiff = doc.m_Points[i] - mousePt;
                INT ptsDist = std::abs(std::hypot(ptsDiff.cx, ptsDiff.cy));
                if (ptsDist < 5)
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
            frame.m_Panel.RedrawWindow();
            //view.RedrawWindow();
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

            m_PenRed.CreatePen(PS_SOLID, 1, RGB(0xff, 0, 0));
            m_PenBlack.CreatePen(PS_SOLID, 1, ~0);
            m_Brush.CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0xff, 0));
        }
        return result;
    }
    BOOL CMainFrame::CCanvas::RedrawWindow()
    {
        auto& frame = GetMainFrame();
        auto& doc = GetMainDocument();
        CDC&  dc = *GetDC();

        BOOL result = FALSE;
        {
            result = CStatic::RedrawWindow();

            if (!doc.m_Points.empty())
            {
                dc.SelectObject(m_PenRed);
                dc.SelectObject(m_Brush);

                dc.Polygon(doc.m_Points.begin()._Ptr, doc.m_Points.size());

                dc.SelectObject(m_PenBlack);
                for (auto i = 0u; i < doc.m_Points.size(); ++i)
                {
                    CPoint p = doc.m_Points[i];
                    p.Offset(-std::sqrt(5), -std::sqrt(5));
                    dc.Ellipse(CRect(p, CSize(5, 5)));
                }
            }

            if (!doc.m_RunningPoints.empty())
            {
                dc.SelectObject(m_PenRed);
                dc.SelectObject(GetStockObject(NULL_BRUSH));

                std::vector<CPoint> cp;
                {
                    std::for_each(
                        doc.m_RunningPoints.begin(),
                        doc.m_RunningPoints.end(),
                        [&cp](const DPoint& dp)
                    {
                        cp.push_back(dp.operator CPoint());
                    });
                }
                dc.Polygon(cp.begin()._Ptr, doc.m_RunningPoints.size());
                //dc.Rectangle(CRect
                //{
                //    CPoint
                //{
                //    std::min_element(doc.m_RunningPoints.begin(), doc.m_RunningPoints.end(), 
                //    [](CPoint& p1, CPoint& p2) {return p1.x < p2.x;})->operator CPoint().x,
                //    std::min_element(doc.m_RunningPoints.begin(), doc.m_RunningPoints.end(), 
                //    [](CPoint& p1, CPoint& p2) {return p1.y < p2.y;})->operator CPoint().y
                //},
                //    CPoint
                //{
                //    std::max_element(doc.m_RunningPoints.begin(), doc.m_RunningPoints.end(), 
                //    [](CPoint& p1, CPoint& p2) {return p1.x < p2.x;})->operator CPoint().x,
                //    std::max_element(doc.m_RunningPoints.begin(), doc.m_RunningPoints.end(), 
                //    [](CPoint& p1, CPoint& p2) {return p1.y < p2.y;})->operator CPoint().y
                //}
                //});
            }

        }

        dc.ReleaseOutputDC();
        return result;
    }
#endif // !C_CANVAS
#ifndef C_PANEL
    BEGIN_MESSAGE_MAP(CMainFrame::CPanel, CStatic)
        ON_WM_CREATE()
        ON_BN_CLICKED(CtrId::id_button_ok, CMainFrame::CPanel::OnButtonClickOk)
        ON_BN_CLICKED(CtrId::id_check_box_pnt, CMainFrame::CPanel::OnButtonClickPaint)
        ON_BN_CLICKED(CtrId::id_check_box_run, CMainFrame::CPanel::OnButtonClickRun)
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

            for (auto i = 0u; i < 3; ++i)
            {
                CString editStr;
                    m_EditPoints[i].GetWindowText(editStr);

                if (i < doc.m_Points.size())
                {
                    CPoint p = doc.m_Points[i];
                    CString pointStr;
                        pointStr.Format(L"(%d , %d)", p.x, p.y);

                    if (editStr != pointStr)
                        m_EditPoints[i].SetWindowText(pointStr);
                }
                else
                {
                    if (editStr != L"")
                        m_EditPoints[i].SetWindowText(L"");
                }
            }

            m_CheckBoxRun.RedrawWindow();
            m_CheckBoxPaint.RedrawWindow();

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

            m_CheckBoxPaint;
            {
                componentRect.MoveToXY(componentRect.left+13, componentRect.top + 25);
                componentRect.DeflateRect(10, 0, 100, 0);
                auto hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

                LONG style{
                    WS_CHILD 
                    | WS_VISIBLE 
                    | WS_TABSTOP 
                    | BS_AUTOCHECKBOX 
                    | BS_ICON
                    | TBS_TRANSPARENTBKGND
                };
                m_CheckBoxPaint.Create(L"Paint", style, componentRect, this, CtrId::id_check_box_pnt);
                m_CheckBoxPaint.SetIcon(hIcon);
            }

            m_CheckBoxRun;
            {
                componentRect.MoveToX(componentRect.right + 20);
                auto hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
                LONG style{
                    WS_CHILD |
                    WS_VISIBLE |
                    WS_TABSTOP |
                    BS_CHECKBOX|
                    //BS_AUTOCHECKBOX |
                    BS_ICON |
                    TBS_TRANSPARENTBKGND
                };
                m_CheckBoxRun.Create(L"Run", style, componentRect, this, CtrId::id_check_box_run);
                m_CheckBoxRun.SetIcon(hIcon);
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

    
    DPoint Centroid(std::vector<DPoint>& pts)
    {
        return 
            std::accumulate(
            pts.begin(),pts.end(),
            DPoint({ 0,0 }),
            [&pts](auto& acc, auto& val)
        {
            acc.x += val.x / pts.size();
            acc.y += val.y / pts.size();
            return acc;
        });
    }
    void SimpleRotation(std::vector<DPoint>& pts, DPoint o, DOUBLE alfa)
    {
        static const auto pi = 3.141592653589793238462643383279502884L;
        auto fi = (pi / 180) * alfa;
        for (auto& d : pts)
        {
            auto p = d;

            p.x = std::cos(fi) * (d.x - o.x) - std::sin(fi) * (d.y - o.y) + o.x;
            p.y = std::sin(fi) * (d.x - o.x) + std::cos(fi) * (d.y - o.y) + o.y;

            d = p;
        }
    }
    void SimleMovement(std::vector<DPoint>& pts, DPoint ms)
    {
        for (auto& d : pts)
        {
            d.x += ms.x;
            d.y += ms.y;
        }
    }
    BOOL cmpX(DPoint& p1, DPoint& p2)
    {
        return p1.x < p2.x;
    }
    BOOL cmpY(DPoint& p1, DPoint& p2)
    {
        return p1.y < p2.y;
    }
    std::vector<DPoint> DRect(std::vector<DPoint>& pts)
    {
        return  std::vector<DPoint>{
            DPoint
        {
            std::min_element(pts.begin(), pts.end(), cmpX)->x,
            std::min_element(pts.begin(), pts.end(), cmpY)->y
        },
            DPoint
        {
            std::max_element(pts.begin(), pts.end(), cmpX)->x,
            std::min_element(pts.begin(), pts.end(), cmpY)->y

        },
            DPoint
        {
            std::min_element(pts.begin(), pts.end(), cmpX)->x,
            std::max_element(pts.begin(), pts.end(), cmpY)->y
        },
            DPoint
        {
            std::max_element(pts.begin(), pts.end(), cmpX)->x,
            std::max_element(pts.begin(), pts.end(), cmpY)->y
        }
        };
    }
    DPoint OutOfBoundCorrection(std::vector<DPoint>& pts, CRect bound)
    {
        DPoint fix{ 0,0 };

        auto dRect = DRect(pts);

        auto diffXL = dRect[0].x - bound.left;
        if ( diffXL < 0)
            fix.x = diffXL;

        auto diffYT = dRect[0].y - bound.top;
        if (diffYT < 0)
            fix.y = diffYT;
        
        auto diffXR = dRect[1].x - bound.right;

        if (diffXR > 0)
            fix.x = diffXR;

        auto diffYB = dRect[2].y - bound.bottom;
        if (diffYB > 0)
            fix.y = diffYB;

        return fix;

    }
    void MoveAndRotateVector(std::vector<DPoint>& pts, CRect border)
    {
        if (pts.empty())
            return;

        static DPoint ms { 15,15 };
        static DOUBLE fi = 10;

        BOOL onRotatingCollision = FALSE;
        {
            DOUBLE fi_fixed;

            int i = 1;
            for (; i < 10; ++i)
            {
                auto ptsCopy = pts;
                SimpleRotation(ptsCopy, Centroid(ptsCopy), fi);
                BOOL alright = true;
                for (auto& p : ptsCopy)
                {
                    if (!border.PtInRect(p))
                    {
                        alright = false;
                        break;
                    }
                }

                fi_fixed = fi / i;

                if (alright)
                    break;
            }

            SimpleRotation(pts, Centroid(pts), fi_fixed);
        }

        BOOL onStraightMoveCollision = FALSE;
        {
            DPoint ms_fixed = ms;

            auto dRect = DRect(pts);
            auto c = Centroid(pts);
            if (ms.x > 0)
            {
                auto dist = std::abs(border.right - dRect[1].x);
                if (dist < ms.x)
                {
                    ms_fixed.x = dist -1;
                    ms.x *= -1;

                    DPoint pc = *std::max_element(pts.begin(), pts.end(), cmpX);
                    if ((pc.y < c.y) == (fi > 0))
                    {
                        fi *= -1;
                    }
                }
            }
            else
            {
                auto dist = std::abs(dRect[0].x - border.left);
                if (dist < std::abs(ms.x))
                {
                    ms_fixed.x = -dist + 1;
                    ms.x *= -1;

                    DPoint pc = *std::min_element(pts.begin(), pts.end(), cmpX);
                    if ((pc.y < c.y) == (fi < 0))
                    {
                        fi *= -1;
                    }
                }
            }

            if (ms.y > 0)
            {
                auto dist = std::abs(border.bottom - dRect[2].y);
                if (dist < ms.y)
                {
                    ms_fixed.y = dist - 1;
                    ms.y *= -1;

                    DPoint pc = *std::max_element(pts.begin(), pts.end(), cmpY);
                    if ((pc.x < c.x) == (fi < 0))
                    {
                        fi *= -1;
                    }
                }
            }
            else
            {
                auto dist = std::abs(border.top - dRect[0].y);
                if (dist < std::abs(ms.y))
                {
                    ms_fixed.y = -dist + 1;
                    ms.y *= -1;

                    DPoint pc = *std::min_element(pts.begin(), pts.end(), cmpY);
                    if ((pc.x < c.x) == (fi > 0))
                    {
                        fi *= -1;
                    }
                }
            }
            SimleMovement(pts, ms_fixed);
        }

    }


    void RotateTriangleVector(std::vector<DPoint>& vec, CRect border)
    {
        if (vec.empty())        
            return;

        border.OffsetRect(-10, -10);

        DPoint c = Centroid(vec);

        static auto fi = 2;

        SimpleRotation(vec, c, fi);

        //for (int i = 0; i < 3; ++i)
        //{
        //    if (!border.PtInRect(vec[i])) 
        //    {
        //        fi *= 1;
        //        //fi += .1;
        //    }
        //}
    }
    void MoveTriangleVector(std::vector<DPoint>& vec, CRect border)
    {
        if (vec.empty())
            return;

        border.OffsetRect(-10, -10);
        static CSize movementVector(4, 4);

        CRect triRect
        {
            CPoint
            {
            std::min_element(vec.begin(), vec.end(), [](CPoint& p1, CPoint& p2) {return p1.x < p2.x;})->operator CPoint().x,
            std::min_element(vec.begin(), vec.end(), [](CPoint& p1, CPoint& p2) {return p1.y < p2.y;})->operator CPoint().y
            },
            CPoint
            {
            std::max_element(vec.begin(), vec.end(), [](CPoint& p1, CPoint& p2) {return p1.x < p2.x;})->operator CPoint().x,
            std::max_element(vec.begin(), vec.end(), [](CPoint& p1, CPoint& p2) {return p1.y < p2.y;})->operator CPoint().y
            }
        };

        CSize fix(0, 0);

        if (LONG diff = triRect.top - border.top < 0/*-movementVector.cy*/)
        {
           // movementVector.cy += movementVector.cy / std::abs(movementVector.cy);
            movementVector.cy *= -1;
            fix.cy = diff;
        }
        if (LONG diff = triRect.bottom - border.bottom > 0/*-movementVector.cy*/)
        {
           // movementVector.cy += movementVector.cy / std::abs(movementVector.cy);
            movementVector.cy *= -1;
            fix.cy = diff;

        }
        if (LONG diff = triRect.left - border.left < 0/*-movementVector.cx*/)
        {
            //movementVector.cx += movementVector.cx / std::abs(movementVector.cx);
            movementVector.cx *= -1;
            fix.cx = diff;

        }
        if (LONG diff = triRect.right - border.right > 0 /*-movementVector.cx*/)
        {
            //movementVector.cx += movementVector.cx / std::abs(movementVector.cx);
            movementVector.cx *= -1;
            fix.cx = diff;
        }

        CPoint ms(movementVector.cx + fix.cx, movementVector.cy + fix.cy);
        SimleMovement(vec, ms);

        //for (auto i = 0u; i < vec.size(); ++i)
        //{
        //    vec[i].x += movementVector.cx + fix.cx;
        //    vec[i].y += movementVector.cy + fix.cy;
        //}

    }

    UINT __cdecl RunningPointsThread(LPVOID param)
    {
        CMainFrame& frame   = *reinterpret_cast<CMainFrame*>(param);
        CMainView& view     = *dynamic_cast<CMainView*>(frame.GetActiveView());
        CMainDocument& doc  = *dynamic_cast<CMainDocument*>(view.GetDocument());

        while (true)
        {
            auto border = frame.m_Canvas.m_Rect;
            border.OffsetRect(-11, -11);
            MoveAndRotateVector(doc.m_RunningPoints, border);
            frame.m_Canvas.RedrawWindow();
            Sleep(50);
        }
    }

    void CMainFrame::CPanel::OnButtonClickPaint()
    {
        auto& frame = GetMainFrame();

        //auto cbState = frame.m_Panel.m_CheckBoxPaint.GetState();
        //frame.m_Panel.m_ButtOk.EnableWindow(cbState&BST_CHECKED? FALSE:TRUE);

        frame.m_Panel.m_CheckBoxPaint.RedrawWindow();
    }
    void CMainFrame::CPanel::OnButtonClickRun()
    {
        auto& doc   = GetMainDocument();

        if (std::empty(doc.m_Points) && std::empty(doc.m_RunningPoints))
            return;
        else
        {
            auto& app = GetMainApp();
            auto& frame = GetMainFrame();

            if (!std::empty(doc.m_RunningPoints))
            {
                doc.m_RunningPoints.clear();
                delete app.m_RunningPointsThreadPtr;
                app.m_RunningPointsThreadPtr = NULL;
            }

            if (!std::empty(doc.m_Points))
            {
                for (auto i = 0u; i < doc.m_Points.size(); ++i)
                {
                    doc.m_RunningPoints.push_back(DPoint{ doc.m_Points[i] });
                }
                frame.OnEditClear();
                app.m_RunningPointsThreadPtr = AfxBeginThread(RunningPointsThread, &frame, THREAD_PRIORITY_NORMAL);
            }
        }
    }
    void CMainFrame::CPanel::OnEditPointChanged()
    {
        auto& frame = GetMainFrame();

        if (frame.m_OnEditClear)
            return;

        auto cbState = frame.m_Panel.m_CheckBoxPaint.GetState();

        if (!cbState&BST_CHECKED)
            return;

        GetMainDocument().UpdateData();
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

    auto checkPaintState = frame.m_Panel.m_CheckBoxPaint.GetState();

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

void CMainView::OnMouseHover(UINT nFlags, CPoint mousePt)
{
    auto& frame = GetMainFrame();
    auto& doc = GetMainDocument();

    auto checkPaintState 
        = frame.m_Panel.m_CheckBoxPaint.GetState();

    if (checkPaintState&BST_CHECKED)    
        if (frame.m_Canvas.m_Rect.PtInRect(mousePt))
        {
            CRect clientRect;            
                GetClientRect(clientRect);
            
            // offsetting mouse pt in relation to canvas
            mousePt += clientRect.TopLeft() - frame.m_Canvas.m_Rect.TopLeft();

            for (auto i = 0u; i < doc.m_Points.size(); ++i)
            {
                CSize ptsDiff = doc.m_Points[i] - mousePt;
                INT ptsDist = std::hypot(ptsDiff.cx, ptsDiff.cy);
                if (std::abs(ptsDist) < 5)                
                    SetCursor(::LoadCursor(NULL, IDC_HAND));                
            }

            if (nFlags&MK_LBUTTON)
            {
                frame.m_Canvas.OnLButtonDown(nFlags, mousePt);
            }
        }
    
    CView::OnMouseHover(nFlags, mousePt);
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