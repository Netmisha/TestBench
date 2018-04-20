#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afximagepaintarea.h>
#include <afxdlgs.h>
#include <afxcview.h>

#include <vector>

enum IssueCtrl
{
    id_list = 14088,
    id_butt_submit,
    id_edit_title,
    id_edit_description,
    id_edit_expected,
    id_edit_actual,
    id_edit_steps
};

class CIssueFrame :
    public CFrameWnd
{
    DECLARE_DYNCREATE(CIssueFrame);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL Create(CWnd* pParentWnd);
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnDestroy();

    CPoint  m_SpawnPt;
    CSize   m_Size;
    CRect   m_Rect;
};

class CIssueView :
    public CView
{
    DECLARE_DYNCREATE(CIssueView);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnDraw(CDC* pDC) override;
    afx_msg virtual VOID OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    class CIssueList :
        public CListCtrl
    {
        DECLARE_MESSAGE_MAP();
    public:
        afx_msg virtual VOID OnLButtonDblClk(UINT nFlags, CPoint p);
        afx_msg virtual VOID UpdateWindow();
        class CSingleIssueFrame* m_CurrentTitle = nullptr;
    } 
    m_List;  
};

class CSingleIssueFrame :
    public CFrameWnd
{
    DECLARE_DYNCREATE(CSingleIssueFrame);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL Create(CWnd* pParentWnd);
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnClose();

    CSingleIssueFrame() = default;
    CSingleIssueFrame(CString const& issueName);

    CWnd*   m_Parent;
    CString m_Title;
    CPoint  m_SpawnPt;
    CSize   m_Size;
    CRect   m_Rect;
    BOOL    m_IsNewTitle = FALSE;
};

class CSingleIssueView :
    public CView
{
    DECLARE_DYNCREATE(CSingleIssueView);
    DECLARE_MESSAGE_MAP();
public:
    afx_msg virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg virtual VOID OnDraw(CDC* pDC) override;
    afx_msg virtual VOID OnButtonClickSubmit();
    afx_msg virtual VOID OnEditChanged();
    void RetreiveDataFromDoc();

    CSingleIssueFrame& GetParentFrame();

    CPoint  m_SpawnPoint     = { 10, 35 };
    INT     m_CtrlWidth      = { (INT)(900 * .95) };
    INT     m_LineHeight     = { 35 };
    INT     m_LineGap        = { 25 };
    INT     m_MultiLineCount = { 6 };

    CEdit   m_EditTitle;
    CEdit   m_EditDescription;
    CEdit   m_EditExpectedResult;
    CEdit   m_EditActualResult;
    CEdit   m_EditSteps;
    CButton m_ButtSubmit;
    BOOL    m_StateChanged = FALSE;
};