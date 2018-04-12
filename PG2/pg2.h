#pragma once
#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>

#include "resource.h"

#define buttonId 11011

class CMainApp : 
    public CWinApp
{
public:
    BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};
class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame);
    DECLARE_MESSAGE_MAP();
    afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg INT  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnButtonClickedKEK();
public:
    enum CtrId
    {
        id_canvas 
            = 101010,
        id_panel,
        id_edit_point,
        id_button_ok 
            = id_edit_point +3,
        id_edit
    };

    class CCanvas :
        public CStatic
    {
        //DECLARE_MESSAGE_MAP();
        //afx_msg void OnLButtonDown(UINT nFlags, CPoint p);
    public:
        enum { ID = id_canvas };
        BOOL Create(CWnd* parent);
        
        CRect m_Rect;
    };

    class CPanel :
        public CButton
    {
        DECLARE_MESSAGE_MAP();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    public:

        enum { ID = id_panel };
        BOOL Create(CWnd* parent);
        BOOL RedrawWindow();

        CRect m_Rect;

        CEdit editPoints[3];
        CButton buttOk;
    };

    CRect m_Rect;

    CCanvas canvas;
    CPanel  panel;
};
class CTriangleDoc : public CDocument
{
public:
    DECLARE_DYNCREATE(CTriangleDoc);
    DECLARE_MESSAGE_MAP();
    afx_msg BOOL OnNewDocument();
public:
    CPoint points[3];
};

class CMainView : public CView
{
public:
    DECLARE_DYNCREATE(CMainView);
    DECLARE_MESSAGE_MAP();
    afx_msg void CMainView::OnDraw(CDC* pDC);
};

class CChildFrame : public CMDIChildWnd
{
public:
    DECLARE_DYNCREATE(CChildFrame);
    DECLARE_MESSAGE_MAP();
};