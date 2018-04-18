#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>
#include <afxdlgs.h>
#include <afxcview.h>

#include <vector>
class CIssueFrame :
    public CFrameWnd
{
    DECLARE_DYNCREATE(CIssueFrame);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL Create(CWnd* pParentWnd);
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnDestroy();

    CPoint  m_SpawnPoint{ 200,200 };
    CSize   m_Size;
    CRect   m_Rect;
};

class CIssueView :
    public CView
{
    DECLARE_DYNCREATE(CIssueView);
    DECLARE_MESSAGE_MAP();
public:
    class CIssueButton :
        public CButton
    {
    public:
        CIssueButton();
        CSize   m_Size;
        CString m_Name;
        CString m_Description;
        CString m_OtherContent;
    };

    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnDraw(CDC* pDC) override;
    afx_msg virtual VOID OnClickedButton(UINT nId);
    #define ID_BUTTON(x) (14088 + (x))
    
    std::vector<CIssueButton*> m_Buttons;
    CButton m_ButtSubmit;

    CListCtrl m_List;
};

class CIssueDialog :
    CDialog
{

};