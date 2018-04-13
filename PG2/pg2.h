#pragma once
#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>

#include "resource.h"
#include <vector>

class CMainApp : 
    public CWinApp
{
public:
    BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnFileExit();
    afx_msg void OnHelpBasics();
    afx_msg void OnHelpAbouttestbench();
};

enum CtrId
{
    id_canvas
    = 101010,
    id_panel,
    id_edit_point,
    id_button_ok
    = id_edit_point + 3,
    id_edit
};

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame);
    DECLARE_MESSAGE_MAP();
public:
    class CCanvas :
        public CStatic
    {
        DECLARE_MESSAGE_MAP();
    public:
        afx_msg void OnLButtonDown(UINT nFlags, CPoint p);
        BOOL Create(CWnd* parent);
        BOOL RedrawWindow();

        CRect  m_Rect;
        CPen   m_Pen;
        CBrush m_Brush;
    };
    class CPanel :
        public CButton
    {
        DECLARE_MESSAGE_MAP();
        afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnButtonClickOk();
    public:
        enum { ID = id_panel };
        BOOL Create(CWnd* parent);
        BOOL RedrawWindow();

        CRect m_Rect;
        CEdit editPoints[3];
        CButton buttOk;
    };

    afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg INT  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint p);

    CRect   m_Rect;
    CCanvas m_Canvas;
    CPanel  m_Panel;
    afx_msg void OnEditClear();
};
class CMainDocument: 
    public CDocument
{
    DECLARE_DYNCREATE(CMainDocument);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg BOOL OnNewDocument();
    std::vector<CPoint> m_Points;
    //CPoint m_Points[3];
};
class CMainView: 
    public CView
{
    DECLARE_DYNCREATE(CMainView);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg void OnDraw(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint p);
};
class CChildFrame : public CMDIChildWnd
{
public:
    DECLARE_DYNCREATE(CChildFrame);
    DECLARE_MESSAGE_MAP();
};