#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>
#include <afxdlgs.h>


class CIssueFrame :
    public CFrameWnd
{
    DECLARE_DYNCREATE(CIssueFrame);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL Create(CWnd* pParentWnd);
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    CRect m_Rect;
};

class CIssueView :
    public CView
{
    DECLARE_DYNCREATE(CIssueView);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual void OnDraw(CDC* pDC) override;

    CStatic m_Sheet;
};