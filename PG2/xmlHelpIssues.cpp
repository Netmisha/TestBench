#include "xmlHelpIssues.h"
#include "tinyxml.h"

#include <tuple>
#include <vector>
#include <cstdlib>
#include <initializer_list>

TiXmlDocument xmlDoc("issues.xml");


static auto basicWidth = 900;
static auto basicHeight = 500;
static auto issueBgrndColor = RGB(192, 192, 192);
static constexpr auto buffSize = 128u;

IMPLEMENT_DYNCREATE(CIssueFrame, CFrameWnd);
BEGIN_MESSAGE_MAP(CIssueFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CIssueFrame::Create(CWnd* pParentWnd)
{
    m_SpawnPt = { 225,225 };
    m_Size = { basicWidth, basicHeight };
    m_Rect = { m_SpawnPt, m_Size };

    LONG style = WS_OVERLAPPEDWINDOW;
    {
        style |= WS_VISIBLE;
        style |= WS_SYSMENU;
        style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
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
    CFrameWnd::OnDestroy();
}

IMPLEMENT_DYNCREATE(CIssueView, CView);
BEGIN_MESSAGE_MAP(CIssueView, CView)
    ON_WM_CREATE() 
    ON_WM_ACTIVATE()
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
    clientRect.DeflateRect(10, 5);

    LONG listStyle{
        WS_CHILD
        | WS_VISIBLE
        | WS_BORDER
        | LVS_REPORT
    };
    m_List.Create(listStyle, clientRect, this, IssueCtrl::id_list);
    {
        m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
        m_List.InsertColumn(0, _T("#"), LVCFMT_RIGHT, clientRect.Size().cx * .1);
        m_List.InsertColumn(1, _T("Title"), LVCFMT_LEFT, clientRect.Size().cx * .3);
        m_List.InsertColumn(2, _T("Description"), LVCFMT_LEFT, clientRect.Size().cx * .6);
    }
    m_List.BringWindowToTop();

    return 0;
}
void CIssueView::OnDraw(CDC * pDC)
{
    CView::OnDraw(pDC);

    CBrush bGroundBrush;
    {
        bGroundBrush.CreateSolidBrush(issueBgrndColor);
        pDC->SelectObject(bGroundBrush);
    }
    CRect clientRect;
    {
        GetClientRect(clientRect);
    }
    pDC->Rectangle(clientRect);

    m_List.UpdateWindow();
}

VOID CIssueView::OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized)
{
    CView::OnActivate(nState, pWndOther, bMinimized);

    UpdateWindow();
    RedrawWindow();
    GetParent()->ShowWindow(SW_MINIMIZE);
    GetParent()->ShowWindow(SW_NORMAL);
}

BEGIN_MESSAGE_MAP(CIssueView::CIssueList, CListCtrl)
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP();

VOID CIssueView::CIssueList::OnLButtonDblClk(UINT nFlags, CPoint p)
{
    auto nIndex = GetSelectionMark();
    if (nIndex == -1)
        return;

    CString issueName;
    issueName = GetItemText(nIndex, 1);

    m_CurrentTitle = new CSingleIssueFrame(issueName);

    m_CurrentTitle->Create(this);   
    WaitForSingleObject(m_CurrentTitle->GetSafeHwnd(), INFINITE);
    UpdateWindow();
}
VOID CIssueView::CIssueList::UpdateWindow()
{
    DeleteAllItems();

    INT i = 0;
    auto root = xmlDoc.RootElement();
    for (TiXmlNode* iter = root->FirstChild(); iter; iter = iter->NextSibling())
    {
        CString issueNumber; issueNumber.Format(L"%d", i);
        CString issueName(iter->ToElement()->Attribute("name"));
        CString issueDescr(iter->FirstChild()->ToElement()->GetText());
        INT nIndex;
        {
            nIndex = InsertItem(i, issueNumber);
            SetItemText(nIndex, 1, issueName);
            SetItemText(nIndex, 2, issueDescr);
        }
        ++i;
    }

    CListCtrl::UpdateWindow();
}

IMPLEMENT_DYNCREATE(CSingleIssueFrame, CFrameWnd);
BEGIN_MESSAGE_MAP  (CSingleIssueFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CSingleIssueFrame::Create(CWnd * pParentWnd)
{
    m_Parent = pParentWnd;

    LONG style = WS_OVERLAPPEDWINDOW;
    {
        style |= WS_VISIBLE;
        style |= WS_SYSMENU;
        style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        style |= WS_POPUP;
    }

    BOOL creationResult = CFrameWnd::Create(NULL, m_Title, style, m_Rect, m_Parent);

    UpdateWindow();
    ShowWindow(SW_SHOW);

    return creationResult;
}
BOOL CSingleIssueFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        MessageBox(L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    CCreateContext ccx;
    ccx.m_pNewViewClass = RUNTIME_CLASS(CSingleIssueView);
    ccx.m_pCurrentDoc = NULL;

    CView* pView = DYNAMIC_DOWNCAST(CSingleIssueView, this->CreateView(&ccx));

    pView->ShowWindow(SW_SHOW);
    pView->OnInitialUpdate();

    SetActiveView(pView);
    RecalcLayout();

    m_Parent->EnableWindow(FALSE);

    return 0;
}
VOID CSingleIssueFrame::OnClose()
{
    m_Parent->EnableWindow(TRUE);

    CFrameWnd::OnClose();
}

CSingleIssueFrame::CSingleIssueFrame(CString const & issueName):
    m_Title(issueName),
    m_SpawnPt(250, 250),
    m_Size(900, 620),
    m_Rect(m_SpawnPt, m_Size)
{}


IMPLEMENT_DYNCREATE(CSingleIssueView, CView);
BEGIN_MESSAGE_MAP(CSingleIssueView, CView)
    ON_WM_CREATE()
    ON_BN_CLICKED(IssueCtrl::id_butt_submit, CSingleIssueView::OnButtonClickSubmit)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 0, CSingleIssueView::OnEditChanged)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 1, CSingleIssueView::OnEditChanged)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 2, CSingleIssueView::OnEditChanged)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 3, CSingleIssueView::OnEditChanged)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 4, CSingleIssueView::OnEditChanged)
    ON_EN_CHANGE(IssueCtrl::id_edit_title + 5, CSingleIssueView::OnEditChanged)
END_MESSAGE_MAP();

BOOL CSingleIssueView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    auto& frame = GetParentFrame();

    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!xmlDoc.LoadFile(TiXmlEncoding::TIXML_ENCODING_LEGACY))
    {
        MessageBox(L"issues.xml load failed", L"", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    LONG style{
        WS_CHILD |
        WS_VISIBLE |
        WS_TABSTOP |
        WS_BORDER |
        ES_LEFT |
        ES_AUTOHSCROLL
    };
    CPoint spawnPoint = m_SpawnPoint;

    m_EditTitle;
    {
        m_EditTitle.Create(style, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), this, IssueCtrl::id_edit_title);
        m_EditTitle.BringWindowToTop();

        spawnPoint.Offset(0, m_LineHeight + m_LineGap);
    }

    m_EditDescription;
    {
        m_EditDescription.Create(style, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), this, IssueCtrl::id_edit_description);
        m_EditDescription.BringWindowToTop();

        spawnPoint.Offset(0, m_LineHeight + m_LineGap);
    }

    m_EditSteps;
    {
        auto mlStyle = style | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL | ES_AUTOVSCROLL;
        m_EditSteps.Create(mlStyle, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight * m_MultiLineCount)), this, IssueCtrl::id_edit_steps);
        m_EditSteps.BringWindowToTop();

        spawnPoint.Offset(0, m_LineHeight * m_MultiLineCount + m_LineGap);
    }

    m_EditExpectedResult;
    {
        m_EditExpectedResult.Create(style, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), this, IssueCtrl::id_edit_expected);
        m_EditExpectedResult.BringWindowToTop();

        spawnPoint.Offset(0, m_LineHeight + m_LineGap);
    }

    m_EditActualResult;
    {
        m_EditActualResult.Create(style, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), this, IssueCtrl::id_edit_actual);
        m_EditActualResult.BringWindowToTop();

        spawnPoint.Offset(0, m_LineHeight + m_LineGap);
    }

    m_ButtSubmit;
    {
        LONG style{
            WS_CHILD |
            WS_VISIBLE |
            BS_PUSHBUTTON |
            WS_BORDER
        };

        m_ButtSubmit.Create(L"Submit", style, CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), this, IssueCtrl::id_butt_submit);
        m_ButtSubmit.BringWindowToTop();
    }

    m_SpawnPoint = { 10, 10 };

    if (!frame.m_IsNewTitle)
        RetreiveDataFromDoc();

    return 0;
}
VOID CSingleIssueView::OnDraw(CDC * pDC)
{
    CView::OnDraw(pDC);

    CBrush bGroundBrush;
    {
        bGroundBrush.CreateSolidBrush(issueBgrndColor);
        pDC->SelectObject(bGroundBrush);
    }
    CRect clientRect;
    {
        GetClientRect(clientRect);
    }
    pDC->Rectangle(clientRect);

    auto& frame = GetParentFrame();

    CPoint spawnPoint = m_SpawnPoint;

    auto originlaMode = pDC->SetBkMode(TRANSPARENT);
    {
        pDC->DrawText("Title:", CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), DT_LEFT);
        spawnPoint.Offset(0, m_LineHeight + m_LineGap);

        pDC->DrawText("Description:", CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), DT_LEFT);
        spawnPoint.Offset(0, m_LineHeight + m_LineGap);

        pDC->DrawText("Steps to reproduce:", CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), DT_LEFT);
        spawnPoint.Offset(0, m_LineHeight * m_MultiLineCount + m_LineGap);

        pDC->DrawText("Expected result:", CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), DT_LEFT);
        spawnPoint.Offset(0, m_LineHeight + m_LineGap);

        pDC->DrawText("Actual result:", CRect(spawnPoint, CSize(m_CtrlWidth, m_LineHeight)), DT_LEFT);
        spawnPoint.Offset(0, m_LineHeight + m_LineGap);
    }
    pDC->SetBkMode(originlaMode);

    m_EditTitle             .UpdateWindow();
    m_EditDescription       .UpdateWindow();
    m_EditExpectedResult    .UpdateWindow();
    m_EditActualResult      .UpdateWindow();
    m_EditSteps             .UpdateWindow();
    m_ButtSubmit            .UpdateWindow();
}
VOID CSingleIssueView::OnButtonClickSubmit()
{
    if (!m_StateChanged)
        return;

    auto& frame = GetParentFrame();
    auto root = xmlDoc.RootElement();

    TiXmlElement* newIssueNode = new TiXmlElement("issue");
    {       
        CHAR newIssueTitleBuff[buffSize];
        {
            CString newIssueTitle;
            {
                m_EditTitle.GetWindowText(newIssueTitle);
            }
            wcstombs(newIssueTitleBuff, newIssueTitle.GetBuffer(), buffSize);
        }
        newIssueNode->SetAttribute("name", newIssueTitleBuff);

        TiXmlElement* singleLineElements[]{
            new TiXmlElement("description"),
            new TiXmlElement("expected_result"),
            new TiXmlElement("actual_result")
        };
        CEdit* singleLineEdits[]{
            &m_EditDescription,
            &m_EditExpectedResult,
            &m_EditActualResult
        };
        for (int i = 0; i < 3; ++i)
        {
            CHAR nodeTextBuff[buffSize];
            {
                CString editText;
                {
                    singleLineEdits[i]->GetWindowText(editText);
                }
                wcstombs(nodeTextBuff, editText.GetBuffer(), buffSize);
            }
            auto elementText = new TiXmlText(nodeTextBuff);
            singleLineElements[i]->LinkEndChild(elementText);
            newIssueNode->LinkEndChild(singleLineElements[i]);
        }

        TiXmlElement* reprStepsNode = new TiXmlElement("steps_to_reproduce");
        {
            newIssueNode->LinkEndChild(reprStepsNode);

            CString allSteps;
            {
                m_EditSteps.GetWindowText(allSteps);
            }

            INT i = 0;
            for(CString line; AfxExtractSubString(line, allSteps, i, _T('\n')); ++i)
            {
                if (line.IsEmpty())
                    continue;

                auto stepNode = new TiXmlElement("step");
                reprStepsNode->LinkEndChild(stepNode);

                CHAR stepTextBuff[buffSize]{};
                {
                    auto cPtr = stepTextBuff;
                    auto wPtr = line.GetBuffer() + line.Find(_T(":")) + 2;

                    while (*wPtr != _T('\r'))
                    {
                        *cPtr++ = static_cast<CHAR>(*wPtr++);
                    }
                    *cPtr = 0;
                }            
                auto stepNodeText = new TiXmlText(stepTextBuff);
                stepNode->LinkEndChild(stepNodeText);
            }
        }
    }

    if (frame.m_IsNewTitle)
    {
        root->LinkEndChild(newIssueNode);
    }
    else
    {
        CString m_Title = frame.m_Title;
        for (auto it = root->FirstChild(); it; it = it->NextSibling())
        {
            CString issueTitle(it->ToElement()->Attribute("name"));
            if (issueTitle == m_Title)
            {
                root->ReplaceChild(it, *newIssueNode);
                break;
            }
        }        
    }

    xmlDoc.SaveFile();
    m_StateChanged = FALSE;
    frame.m_IsNewTitle = FALSE;

    frame.m_Parent->Invalidate();
    frame.m_Parent->UpdateWindow();
    frame.m_Parent->GetParent()->Invalidate();
    frame.m_Parent->GetParent()->UpdateWindow();
    frame.m_Parent->GetParent()->GetParent()->Invalidate();
    frame.m_Parent->GetParent()->GetParent()->UpdateWindow();

    //if (frame.m_IsNewTitle)
    //{
    //    TiXmlElement* newIssueNode = new TiXmlElement("issue");
    //    {
    //        CString issueTitle;
    //        {
    //            m_EditTitle.GetWindowText(issueTitle);
    //        }
    //        CHAR titleBuff[buffSize];
    //        {
    //            wcstombs(titleBuff, issueTitle.GetBuffer(), buffSize);
    //        }
    //        newIssueNode->SetAttribute("name", titleBuff);
    //    }
    //    root->LinkEndChild(newIssueNode);
    //    TiXmlElement* singleLineElements[] =
    //    {
    //        new TiXmlElement("description"),
    //        new TiXmlElement("expected_result"),
    //        new TiXmlElement("actual_result")
    //    };
    //    CEdit* singleLineEdits[] =
    //    {
    //        &m_EditDescription,
    //        &m_EditExpectedResult,
    //        &m_EditActualResult
    //    };
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        CString editText;
    //        {
    //            singleLineEdits[i]->GetWindowText(editText);
    //        }
    //        CHAR nodeBuff[buffSize];
    //        {
    //            wcstombs(nodeBuff, editText.GetBuffer(), buffSize);
    //        }
    //        singleLineElements[i]->LinkEndChild(new TiXmlText(nodeBuff));
    //        newIssueNode->LinkEndChild(singleLineElements[i]);
    //    }
    //    TiXmlElement* reprStepsNode = new TiXmlElement("steps_to_reproduce");
    //    {
    //        INT lineCount = m_EditSteps.GetLineCount();
    //        for (int i = 0; i < lineCount; ++i)
    //        {
    //            WCHAR wbuff[buffSize];
    //            {
    //                m_EditSteps.GetLine(i, wbuff);
    //            }
    //            CHAR buff[buffSize];
    //            {
    //                wcstombs(buff, wbuff, buffSize);
    //            }
    //            auto stepNd = new TiXmlElement("step");
    //            stepNd->LinkEndChild(new TiXmlText(buff));
    //            reprStepsNode->LinkEndChild(stepNd);
    //        }
    //        newIssueNode->LinkEndChild(reprStepsNode);
    //    }
    //}
    //else
    //{
    //    if (m_StateChanged)
    //    {
    //        CString m_Title = frame.m_Title;
    //        for (auto it = root->FirstChild(); it; it = it->NextSibling())
    //        {
    //            CString issueTitle(it->ToElement()->Attribute("name"));
    //            if (issueTitle == m_Title)
    //            {
    //                auto newIssueNode = it;
    //                {
    //                    CString newTitle; 
    //                    {
    //                        m_EditTitle.GetWindowText(newTitle);
    //                        if (issueTitle != newTitle)
    //                        {
    //                            CHAR newTitleBuff[buffSize];
    //                            {
    //                                wcstombs(newTitleBuff, newTitle.GetBuffer(), buffSize);
    //                            }
    //                            newIssueNode->ToElement()->SetAttribute("name", newTitleBuff);
    //                        }
    //                    }
    //                    for (auto childNode = newIssueNode->FirstChild(); childNode; childNode = childNode->NextSibling())
    //                    {
    //                        CString nodeName(childNode->ToElement()->Value());
    //                        
    //                        if (nodeName == CString("description"))
    //                        {
    //                            CString oldDescription(childNode->ToElement()->GetText());
    //                            CString newDescription;
    //                            {
    //                                m_EditDescription.GetWindowText(newDescription);
    //                            }
    //                            if (oldDescription != newDescription)
    //                            {
    //                                CHAR newDescriptionBuff[buffSize];
    //                                {
    //                                    wcstombs(newDescriptionBuff, newDescription.GetBuffer(), buffSize);
    //                                }
    //                                childNode->FirstChild()->SetValue(newDescriptionBuff);
    //                            }
    //                            break;
    //                        }
    //                        if (nodeName == CString("expected_result"))
    //                        {
    //                            CString oldExpectedResult(childNode->ToElement()->GetText());
    //                            CString newExpectedResult;
    //                            {
    //                                m_EditExpectedResult.GetWindowText(newExpectedResult);
    //                            }
    //                            if (oldExpectedResult != newExpectedResult)
    //                            {
    //                                CHAR newExpectedResultBuff[buffSize];
    //                                {
    //                                    wcstombs(newExpectedResultBuff, newExpectedResult.GetBuffer(), buffSize);
    //                                }
    //                                childNode->FirstChild()->SetValue(newExpectedResultBuff);
    //                            }
    //                            break;
    //                        }
    //                        if (nodeName == CString("actual_result"))
    //                        {
    //                            CString oldActualResult(childNode->ToElement()->GetText());
    //                            CString newActualResult;
    //                            {
    //                                m_EditActualResult.GetWindowText(newActualResult);
    //                            }
    //                            if (oldActualResult != newActualResult)
    //                            {
    //                                CHAR newActualResultBuff[buffSize];
    //                                {
    //                                    wcstombs(newActualResultBuff, newActualResult.GetBuffer(), buffSize);
    //                                }
    //                                childNode->FirstChild()->SetValue(newActualResultBuff);
    //                            }
    //                            break;
    //                        }
    //                        if (nodeName == CString("steps_to_reproduce"))
    //                        {
    //                            newIssueNode->RemoveChild(childNode);
    //                            TiXmlElement* reprStepsNode = new TiXmlElement("steps_to_reproduce");
    //                            {
    //                                INT lineCount = m_EditSteps.GetLineCount();
    //                                for (int i = 0; i < lineCount; ++i)
    //                                {
    //                                    WCHAR wbuff[buffSize];
    //                                    {
    //                                        m_EditSteps.GetLine(i, wbuff);
    //                                    }
    //                                    CHAR buff[buffSize];
    //                                    {
    //                                        wcstombs(buff, wbuff, buffSize);
    //                                    }
    //                                    auto stepNd = new TiXmlElement("step");
    //                                    stepNd->LinkEndChild(new TiXmlText(buff));
    //                                    reprStepsNode->LinkEndChild(stepNd);
    //                                }
    //                                newIssueNode->LinkEndChild(reprStepsNode);
    //                            }
    //                        }
    //                    }
    //                }
    //                break;
    //            }
    //        }
    //    }
    //    else // no changes -> no reason to save doc
    //    {
    //        return;
    //    }
    //}
}
VOID CSingleIssueView::OnEditChanged()
{
    m_StateChanged = TRUE;
}
VOID CSingleIssueView::RetreiveDataFromDoc()
{
    auto& frame = GetParentFrame();
    CString m_Title = frame.m_Title;

    m_EditTitle.SetWindowText(m_Title);
    
    auto root = xmlDoc.RootElement();
    for (auto it = root->FirstChild(); it; it = it->NextSibling())
    {
        CString issueName(it->ToElement()->Attribute("name"));
        if (issueName == m_Title)
        {
            for (auto itCh = it->FirstChild(); itCh; itCh = itCh->NextSibling())
            {
                CString nodeName(itCh->ToElement()->Value());
                CString nodeText(itCh->ToElement()->GetText());

                if (nodeName == CString("description"))
                    m_EditDescription.SetWindowText(nodeText);

                if (nodeName == CString("expected_result"))
                    m_EditExpectedResult.SetWindowText(nodeText);

                if (nodeName == CString("actual_result"))
                    m_EditActualResult.SetWindowText(nodeText);

                if (nodeName == CString("steps_to_reproduce"))
                {
                    CString combinedNodeText;
                    int i = 0;
                    for (auto itSt = itCh->FirstChild(); itSt; itSt = itSt->NextSibling())
                    {
                        CString nodeText;
                        nodeText.Format(L"%d: %s\r\n", i++, CString(itSt->ToElement()->GetText()));

                        combinedNodeText += nodeText;
                    }
                    m_EditSteps.SetWindowText(combinedNodeText);
                }
            }
            return;
        }
    }
}
CSingleIssueFrame& CSingleIssueView::GetParentFrame()
{
    return *dynamic_cast<CSingleIssueFrame*>(CView::GetParentFrame());
}
