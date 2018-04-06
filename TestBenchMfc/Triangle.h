#pragma once
class Triangle
{
public:
    Triangle();

    void InitTriangle(CWnd* parentWindow, INT idPicture, INT idEdintPointA, INT idEditPointB, INT idEditPointC, INT idEditPointD);
    void UpdateTriangle();
    void DrawTriangle();
    bool IsInitialized();
protected:
    CPoint CStringToCPoint(const CString&);

    bool isInitialized;
    CWnd* pImage;
    CWnd* pEditA, 
        * pEditB, 
        * pEditC, 
        * pEditD;
    CPoint a, b, c, d;

    
};

