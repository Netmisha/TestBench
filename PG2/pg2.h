#pragma once
#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>

#include "resource.h"
#include <vector>

class CMainApp : 
    public CWinApp
{
    DECLARE_MESSAGE_MAP()
public:
    virtual afx_msg void OnHelpBasics();
    virtual afx_msg void OnHelpAboutTestBench();
    virtual afx_msg void OnFileSaveAs();
    virtual afx_msg void OnFileNew();
    virtual afx_msg void OnFileOpen();
    virtual afx_msg void OnAppExit();
    BOOL InitInstance();
    CWinThread* m_RunningPointsThreadPtr;
};

enum CtrId
{
    id_canvas = 101010,
    id_panel,
    id_edit_point,
    id_button_ok
    = id_edit_point + 3,
    id_check_box_pnt,
    id_check_box_run,
};

class CMainFrame : 
    public CFrameWnd
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
        CPen   m_PenRed;
        CPen   m_PenBlack;
        CBrush m_Brush;
    };
    class CPanel :
        public CButton
    {
        DECLARE_MESSAGE_MAP();
        afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnButtonClickOk();
        afx_msg void OnButtonClickPaint();
        afx_msg void OnButtonClickRun();
        afx_msg void OnEditPointChanged();
    public:
        enum { ID = id_panel };
        BOOL Create(CWnd* parent);
        BOOL RedrawWindow();

        CRect       m_Rect;
        CEdit       m_EditPoints[3];
        CButton     m_CheckBoxPaint;
        CButton     m_CheckBoxRun;
        CButton     m_ButtOk;
    };

    afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg INT  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint p);
    afx_msg void OnEditClear();

    CRect   m_Rect;
    CCanvas m_Canvas;
    CPanel  m_Panel;

    BOOL m_OnEditClear;
    afx_msg void OnHelpSomeIssues();
};

class DPoint :
    public CPoint
{
public:
    long double x;
    long double y;
    operator CPoint() const
    {
        return CPoint((LONG)x, (LONG)y);
    }
    
    DPoint(long double _x, long double _y) :
        CPoint((LONG)_x, (LONG)_y),
        x(_x), y(_y)
    {}
    DPoint(const CPoint& obj) :
        CPoint(obj),
        x(obj.x),
        y(obj.y)
    {}
    DPoint(const DPoint& obj):
        CPoint(obj.operator CPoint()),
        x(obj.x),
        y(obj.y)
    {}
};

class CMainDocument: 
    public CDocument
{
    DECLARE_DYNCREATE(CMainDocument);
    DECLARE_MESSAGE_MAP();
public:
    virtual afx_msg BOOL OnNewDocument();
    virtual afx_msg BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
    virtual afx_msg BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual afx_msg BOOL OnOpenDocument(LPCTSTR lpszPathName);
    void UpdateData();
    BOOL DoFileOpen();

    std::vector<CPoint> m_Points;
    std::vector<DPoint> m_RunningPoints;
};
class CMainView: 
    public CView
{
    DECLARE_DYNCREATE(CMainView);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg void OnDraw(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint p);
    afx_msg void OnMouseMove(UINT nFlags, CPoint p);
    afx_msg void OnMouseHover(UINT nFlags, CPoint p);
    afx_msg void OnMouseLeave();

};
class CChildFrame : public CMDIChildWnd
{
public:
    DECLARE_DYNCREATE(CChildFrame);
    DECLARE_MESSAGE_MAP();
};