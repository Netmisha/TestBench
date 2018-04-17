#include "xmlHelpIssues.h"
#include "tinyxml.h"

IMPLEMENT_DYNCREATE(CIssueFrame, CFrameWnd);
BEGIN_MESSAGE_MAP(CIssueFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CIssueFrame::Create(CWnd* pParentWnd)
{
    m_Rect = CRect(CPoint(200, 200), CSize(500, 800));

    LONG style = WS_OVERLAPPEDWINDOW;
    {
        style |= WS_VISIBLE;
        style |= WS_SYSMENU;
        style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        //style ^= WS_THICKFRAME;
    }

    return CFrameWnd::Create(NULL, L"Issues", style, m_Rect);
}

BOOL CIssueFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CCreateContext ccx;
    ccx.m_pNewViewClass = RUNTIME_CLASS(CIssueView);
    ccx.m_pCurrentDoc = NULL;

    CView* pView = DYNAMIC_DOWNCAST(CIssueView, this->CreateView(&ccx));

    pView->ShowWindow(SW_SHOW);
    pView->OnInitialUpdate();

    SetActiveView(pView);
    RecalcLayout();

    return 0;
}

IMPLEMENT_DYNCREATE(CIssueView, CView);
BEGIN_MESSAGE_MAP  (CIssueView, CView)
END_MESSAGE_MAP()

BOOL CIssueView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect clientRect;
    {
        GetClientRect(clientRect);
    }

    LONG style = WS_CHILD;
    {
        style |= WS_VISIBLE;
        style |= SS_WHITERECT;
        style |= WS_BORDER;
        style |= WS_TABSTOP;
    }
    m_Sheet.Create(NULL, style, clientRect, this, 14088);

    return 0;
}

CString GetContent()
{
    CString content;
    {
        TiXmlDocument xmlDoc("issues.xml");
        if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
        {
            ::MessageBox(NULL, L"issues.xml load failed", L"", MB_OK|MB_ICONEXCLAMATION);
        }

        auto root = xmlDoc.RootElement();
        for (auto iter = root->FirstChild(); iter; iter = iter->NextSibling())
        {
            CString nodeContent;
            CString csName(iter->ToElement()->Attribute("name"));
            CString csText;
            for (auto iterChild = iter->FirstChild(); iterChild; iterChild = iterChild->NextSibling())
            {
                CString childName(iterChild->ToElement()->Value());
                CString childText(iterChild->ToElement()->GetText());

                csText.Format(L"%s\n\t%s\n",
                              childName,
                              childText);
            }
            nodeContent.Format(L"%s\n\t%s\n",
                               csName,
                               csText);
            content += nodeContent;
        }     
    }
    return content;
}

void CIssueView::OnDraw(CDC * pDC)
{
    CBrush bGroundBrush;
    {
        bGroundBrush.CreateSolidBrush(RGB(200, 200, 200));
        pDC->SelectObject(bGroundBrush);
    }

    auto& frame = *static_cast<CIssueFrame*>(this->GetParentFrame());
    CRect clientRect; frame.GetClientRect(clientRect);
    clientRect.DeflateRect(1, 1,3,3);
    pDC->Rectangle(clientRect);

    static CString xmlDocContent = GetContent();
    clientRect.DeflateRect(15, 15);
    pDC->DrawText(xmlDocContent, clientRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    //m_Sheet.RedrawWindow();
}
