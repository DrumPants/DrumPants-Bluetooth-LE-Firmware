
// HelloClientDlg.h : header file
//

#pragma once
#include "Win7Interface.h"
#include "Win8Interface.h"

#define WM_CONNECTED    (WM_USER + 101)
#define WM_NOTIFIED     (WM_USER + 102)


// CHelloClientDlg dialog
class CHelloClientDlg : public CDialogEx
{
// Construction
public:
	CHelloClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CHelloClientDlg();

    void SetParam(BLUETOOTH_ADDRESS *bth, HMODULE hLib);

// Dialog Data
	enum { IDD = IDD_HELLO_CLIENT_DIALOG };

	BOOL m_bWin8;
//    BLUETOOTH_ADDRESS m_bth;
    HMODULE m_hLib;
    CBtInterface *m_btInterface;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
    int    numNotifies;
    HANDLE m_hDevice;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();
    LRESULT OnConnected(WPARAM bConnected, LPARAM lparam);
    LRESULT OnNotified(WPARAM op, LPARAM lparam);
public:
    afx_msg void OnBnClickedReadChar1();
    afx_msg void OnBnClickedReadChar2();
    afx_msg void OnBnClickedWriteChar2();
    afx_msg void OnCbnSelchangeAllowToSendChar1();
};
