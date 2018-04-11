#pragma once
#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>

#include "resource.h"
class Cpg2App : 
    public CWinApp
{
public:
    BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};
class Cpg2MainFrame : public CFrameWnd
{
public:
    DECLARE_DYNCREATE(Cpg2MainFrame)
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);
    DECLARE_MESSAGE_MAP()
};
class Cpg2Doc : public CDocument
{
public:
    Cpg2Doc() 
    {
        points[0] = CPoint(20, 20); 
        points[1] = CPoint(120, 45);
        points[2] = points[3] = CPoint(350, 350);    
    }

    DECLARE_DYNCREATE(Cpg2Doc)
    DECLARE_MESSAGE_MAP()
public:
    CPoint points[4];
};

class Cpg2View : public CView
{
public:
    DECLARE_DYNCREATE(Cpg2View)
    afx_msg void Cpg2View::OnDraw(_In_ CDC* pDC);
    DECLARE_MESSAGE_MAP()
};

class CChildFrame : public CMDIChildWnd
{
public:
    DECLARE_DYNCREATE(CChildFrame)
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};