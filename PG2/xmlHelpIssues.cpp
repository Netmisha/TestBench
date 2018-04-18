#include "xmlHelpIssues.h"
#include "tinyxml.h"

TiXmlDocument xmlDoc("issues.xml");


static constexpr INT basicWidth = 500;

static constexpr INT buttonHeight = 30;
static constexpr INT buttonWidth = basicWidth - 40;
static constexpr INT buttonGap = 10;

IMPLEMENT_DYNCREATE(CIssueFrame, CFrameWnd);
BEGIN_MESSAGE_MAP(CIssueFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CIssueFrame::Create(CWnd* pParentWnd)
{
    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        ::MessageBox(NULL, L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
    }

    
    INT basicHeight = 50;

    auto root = xmlDoc.RootElement();
    for (auto iter = root->FirstChild(); iter; iter = iter->NextSibling())
    {
        basicHeight += buttonHeight;
        basicHeight += buttonGap;
    }
    /*for buttonSubmit*/
    {
        basicHeight += buttonHeight;
        basicHeight += buttonGap;
    }

    m_Size = { basicWidth, basicHeight };

    m_Rect = CRect(m_SpawnPoint, m_Size);

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
VOID CIssueFrame::OnDestroy()
{
    xmlDoc.SaveFile();
}

IMPLEMENT_DYNCREATE(CIssueView, CView);

BEGIN_MESSAGE_MAP(CIssueView, CView)
    ON_WM_CREATE() 
    ON_CONTROL_RANGE(BN_CLICKED, ID_BUTTON(0), ID_BUTTON(500), CIssueView::OnClickedButton)
    //ON_BN_CLICKED(ID_BUTTON(0), CIssueView::OnButtonClick<ID_BUTTON(0)>)
END_MESSAGE_MAP();

BOOL CIssueView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        MessageBox(L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    CRect clientRect; 
        this->GetParentFrame()->GetClientRect(clientRect);
        clientRect.DeflateRect(5,5);
    m_List.Create(
        WS_CHILD 
        | WS_VISIBLE 
        | WS_BORDER
        | WS_VSCROLL 
        | LVS_REPORT 
       // | LVS_EDITLABELS
        ,clientRect, this, 14077);
    m_List.SetExtendedStyle(
        LVS_EX_FULLROWSELECT 
        | LVS_EX_SUBITEMIMAGES 
        //| LVS_EX_GRIDLINES
    );

    m_List.InsertColumn(0, _T("#"), LVCFMT_RIGHT, clientRect.Size().cx * .1);
    m_List.InsertColumn(1, _T("Issue"), LVCFMT_LEFT, clientRect.Size().cx * .3);
    m_List.InsertColumn(2, _T("Short Description"), LVCFMT_LEFT, clientRect.Size().cx * .6);

    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        ::MessageBox(NULL, L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        INT i = 0;
        auto root = xmlDoc.RootElement();
        for (TiXmlNode* iter = root->FirstChild(); iter; iter = iter->NextSibling())
        {
            CString issueNumber; issueNumber.Format(L"%d", i);
            CString issueName(iter->ToElement()->Attribute("name"));
            CString issueDescr(iter->FirstChild()->ToElement()->GetText());
            INT nIndex;
            {
                nIndex = 
                m_List.InsertItem(i, issueNumber);
                m_List.SetItemText(nIndex, 1, issueName);
                m_List.SetItemText(nIndex, 2, issueDescr);
            }

            ++i;
        }

        m_List.BringWindowToTop();
    }


    /*LONG buttonStyle{
        WS_CHILD |
        WS_VISIBLE |
        BS_PUSHBUTTON |
        BS_FLAT|
        BS_TEXT|
        BS_LEFT|
        BS_MULTILINE|
        WS_BORDER
    };*/

   /* CPoint  buttPosition{ 10, 10 };
   
    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        ::MessageBox(NULL, L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        int i = 0;
        auto root = xmlDoc.RootElement();
        for (auto iter = root->FirstChild(); iter; iter = iter->NextSibling())
        {
            CIssueButton* ib = new CIssueButton{};
            {
                ib->m_Name = CString(iter->ToElement()->Attribute("name"));
            }
            ib->Create(ib->m_Name, buttonStyle, CRect(buttPosition, ib->m_Size), this, ID_BUTTON(i++));
            m_Buttons.push_back(ib);
            ib->BringWindowToTop();

            buttPosition.Offset(0, ib->m_Size.cy + buttonGap / 2);
        }

        m_ButtSubmit.Create(L"Submit New Issue", buttonStyle^BS_LEFT, CRect(buttPosition, CSize(buttonWidth, buttonHeight)), this, ID_BUTTON(-1));
    }*/

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
    CView::OnDraw(pDC);

    CBrush bGroundBrush;
    {
        bGroundBrush.CreateSolidBrush(RGB(192, 192, 192));
        pDC->SelectObject(bGroundBrush);
    }
    CRect clientRect;
    {
        GetClientRect(clientRect);
    }
    pDC->Rectangle(clientRect);

    m_List.UpdateWindow();
    /*for (auto it = m_Buttons.begin(); it != m_Buttons.end(); ++it)
    {
        (*it)->UpdateWindow();
    }*/
}

VOID CIssueView::OnClickedButton(UINT nId)
{
    INT pos = nId - ID_BUTTON(0);
    CString issueName = m_Buttons[pos]->m_Name;
    {
        auto root = xmlDoc.RootElement();
        for (auto iter = root->FirstChild(); iter; iter = iter->NextSibling())
        {
            CString nodeName(iter->ToElement()->Attribute("name"));
            if (issueName == nodeName)
            {
                CString csText;
                for (auto iterChild = iter->FirstChild(); iterChild; iterChild = iterChild->NextSibling())
                {
                    CString childName(iterChild->ToElement()->Value());
                    CString childText(iterChild->ToElement()->GetText());
                    CString childNode;
                    childNode.Format(L"%s\n\t%s\n",
                                  childName,
                                  childText);
                    csText += childNode;
                }
                MessageBox(csText, issueName, MB_OK);
            }

        }
    }
}

CIssueView::CIssueButton::CIssueButton():
    m_Size(buttonWidth, buttonHeight)
{}
