#pragma once
#include "afxwin.h"

#include <bthdef.h>

// CDeviceSelect dialog

class CDeviceSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSelect)

public:
	CDeviceSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSelect();

// Dialog Data
	enum { IDD = IDD_SELECT_DEVICE };

    BLUETOOTH_ADDRESS m_bth;
    BOOL m_bWin8;

private:
    int m_numDevices;

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDblclkDeviceList();
    virtual BOOL OnInitDialog();
    CListBox m_lbDevices;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
