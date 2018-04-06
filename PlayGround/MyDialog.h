#pragma once
#include <afxwin.h>
#include <afxdlgs.h>
#include "resource.h"

class CMyDialog :
    public CDialog
{
public:
    enum {IDD = IDD_MADIALOGBOX};
    CMyDialog();
   ~CMyDialog();
};

