#pragma once
#include "resource.h"
class CErrorCh:
    public CDialogEx
{
    int instances;
public:
    enum{ IDD = IDD_ERROR};
    CErrorCh();
    DECLARE_MESSAGE_MAP();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedButtonImok();
};

