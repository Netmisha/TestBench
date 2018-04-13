#include "stdafx.h"
#include "Triangle.h"

#include <regex>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
using String = std::basic_string<WCHAR>;
using Regex = std::basic_regex<WCHAR>;
using StringStream = std::wstringstream;

Triangle::Triangle():
    isInitialized(false),
    sortVertices(false)
{}
void Triangle::InitTriangle(CWnd* parentWindow, INT idPicture, INT idEditPointA, INT idEditPointB, INT idEditPointC, INT idEditPointD)
{
    pImage = parentWindow->GetDlgItem(idPicture);
    pEditA = parentWindow->GetDlgItem(idEditPointA);
    pEditB = parentWindow->GetDlgItem(idEditPointB);
    pEditC = parentWindow->GetDlgItem(idEditPointC);
    pEditD = parentWindow->GetDlgItem(idEditPointD);

    isInitialized = true;
}

void Triangle::UpdateTriangle()
{
    CString buffer;

    pEditA->GetWindowTextW(buffer);
    a = CStringToCPoint(buffer);

    pEditB->GetWindowTextW(buffer);
    b = CStringToCPoint(buffer);

    pEditC->GetWindowTextW(buffer);
    c = CStringToCPoint(buffer);

    if (pEditD->IsWindowVisible()) 
    {
        pEditD->GetWindowTextW(buffer);
        d = CStringToCPoint(buffer);
    }
    else
    {
        d = c;
    }
}


void Triangle::DrawTriangle()
{
    pImage->RedrawWindow();

    CPoint points[] = { a,b,c,d };
    auto cmpX = [](CPoint&a, CPoint&b) { return a.x < b.x; };
    auto cmpY = [](CPoint&a, CPoint&b) { return a.y < b.y; };

    CRect imageRect;
    pImage->GetWindowRect(imageRect);
    auto imageWidth = imageRect.right - imageRect.left;
    auto imageHeight = imageRect.bottom - imageRect.top;

    auto pMinX = std::min_element(points, points + 4, cmpX);
    auto pMaxX = std::max_element(points, points + 4, cmpX);
    auto pMinY = std::min_element(points, points + 4, cmpY);
    auto pMaxY = std::max_element(points, points + 4, cmpY);

    auto diffX = imageWidth - pMaxX->x;
    {
        if (diffX <= 0)
        {
            int lessener = min(std::abs(diffX) + 1, pMinX->x);
            for (int i = 0; i < 4; ++i)
                points[i].x -= lessener;
        }
    }
    auto diffY = imageHeight - pMaxY->y;
    {
        if (diffY <= 0)
        {
            int lessener = min(std::abs(diffY) + 1, pMinY->y);
            for (int i = 0; i < 4; ++i)
                points[i].y -= lessener;
        }
    }



    CPen pen(PS_SOLID, 1, RGB(0xff, 0, 0));
    CBrush bruh(PS_SOLID, RGB(0, 0xff, 0));

    CDC* dcPic = pImage->GetDC();
    dcPic->SelectObject(&pen);
    dcPic->SelectObject(&bruh);

    CPoint c(std::accumulate(points, points + 4, 0, [](int& val, CPoint& p) {return val+p.x;}) / 4, 
             std::accumulate(points, points + 4, 0, [](int& val, CPoint& p) {return val+p.y;}) / 4);
    if (sortVertices)
    std::sort(points, points + 4, [&](CPoint& a, CPoint&b)
    {
        if (!cmpX(a,c) && cmpX(b,c))
            return true;
        if (cmpX(a,c) && !cmpX(b,c))
            return false;
        if ((!cmpX(a, c) && !cmpX(c, a)) && (!cmpX(b, c) && !cmpX(c, b)))
            if (!cmpY(a, c) || !cmpY(b, c))
                return cmpY(b, a);
            else
                return cmpY(a,b);
        
        return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y) < 0;
        /*if (det < 0)
            return true;
        if (det > 0)
            return false;

        int d1 = (a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y);
        int d2 = (b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y);
        return d1 > d2;*/
    });

    dcPic->Polygon(points, 4);
    //dcPic->SetPixel(c, RGB(~0, 0, 0));
    pImage->ReleaseDC(dcPic);
}

bool Triangle::IsInitialized()
{
    return isInitialized;
}

void Triangle::SortVertices()
{
    sortVertices ^= true;
}

CPoint Triangle::CStringToCPoint(const CString& str)
{
    CPoint cp;

    String input = std::regex_replace(str.GetString(), Regex(L"[^ 0-9]"), L" ");
    if (std::regex_match(input, Regex(L"[ ]*[0-9]+[ ]+[0-9]+[ ]*")))
    {
        StringStream ss(input);
        ss >> cp.x >> cp.y;
    }
    else
    {
        CRect imageRect;
        pImage->GetWindowRect(imageRect);

        cp.x = rand() % (imageRect.right - imageRect.left);
        cp.y = rand() % (imageRect.bottom - imageRect.top);
    }

    return cp;
}
