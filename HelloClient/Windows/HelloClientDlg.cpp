
// HelloClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include <setupapi.h>
#include "HelloClient.h"
#include "HelloClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelloClientDlg dialog



CHelloClientDlg::CHelloClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelloClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_btInterface = NULL;
}

CHelloClientDlg::~CHelloClientDlg()
{
	delete m_btInterface;
}

void CHelloClientDlg::SetParam(BLUETOOTH_ADDRESS *bth, HMODULE hLib)
{
    if (m_bWin8) 
        m_btInterface = new CBtWin8Interface(bth, hLib, this);
    else
        m_btInterface = new CBtWin7Interface(bth, hLib, this);
}

void CHelloClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHelloClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_CONNECTED, OnConnected)
    ON_MESSAGE(WM_NOTIFIED, OnNotified)
    ON_BN_CLICKED(IDC_READ_CHAR_1, &CHelloClientDlg::OnBnClickedReadChar1)
    ON_BN_CLICKED(IDC_READ_CHAR_2, &CHelloClientDlg::OnBnClickedReadChar2)
    ON_BN_CLICKED(IDC_WRITE_CHAR_2, &CHelloClientDlg::OnBnClickedWriteChar2)
    ON_CBN_SELCHANGE(IDC_ALLOW_TO_SEND_CHAR_1, &CHelloClientDlg::OnCbnSelchangeAllowToSendChar1)
END_MESSAGE_MAP()


// CHelloClientDlg message handlers

BOOL CHelloClientDlg::OnInitDialog()
{
    BOOL bConnected = TRUE;  // assume that device is connected which should generally be the case for hello sensor

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    USHORT ClientConfDescrNotify = 0;

    CString s;
    WCHAR buf[200];
    GetDlgItemText(IDC_DESCRIPTION_CHAR_1, buf, 200);
    UuidToString (&buf[wcslen(buf)], 100, (GUID *)&UUID_HELLO_CHARACTERISTIC_NOTIFY);
    SetDlgItemText(IDC_DESCRIPTION_CHAR_1, buf);

    GetDlgItemText(IDC_DESCRIPTION_CHAR_2, buf, 200);
    UuidToString (&buf[wcslen(buf)], 100, (GUID *)&UUID_HELLO_CHARACTERISTIC_CONFIG);
    SetDlgItemText(IDC_DESCRIPTION_CHAR_2, buf);

    m_btInterface->Init();

    // on Win7 we will receive notification when device is connected and will intialize dialog there
    if (!m_bWin8)
        return TRUE;

    UCHAR szManuName[9] = {0};
    UCHAR szModelNum[9] = {0};
    UCHAR SystemId[8] = {0};
    char bufs[80];
    m_btInterface->GetSystemInfo(szManuName, 8, szModelNum, 8, SystemId, 8);
    SetDlgItemTextA(m_hWnd, IDC_MANU_NAME, (LPCSTR)szManuName);
    SetDlgItemTextA(m_hWnd, IDC_MODEL_NUM, (LPCSTR)szModelNum);
    sprintf_s (bufs, "%02x %02x %02x %02x %02x %02x %02x %02x", SystemId[0], SystemId[1], SystemId[2], SystemId[3], SystemId[4], SystemId[5], SystemId[6], SystemId[7]);
    SetDlgItemTextA(m_hWnd, IDC_SYSTEM_ID, bufs);

    CBtWin8Interface *pWin8BtInterface = dynamic_cast<CBtWin8Interface *>(m_btInterface);

    // Assume that we are connected.  Failed attempt to read battery will change that to FALSE.
    pWin8BtInterface->m_bConnected = TRUE;

    BYTE BatteryLevel = 0;
    m_btInterface->GetBatteryLevel(&BatteryLevel);
    SetDlgItemInt(IDC_BATTERY_LEVEL, BatteryLevel, FALSE);

    if (pWin8BtInterface->m_bConnected)
    {
        SetDlgItemText(IDC_DEVICE_STATE, L"Connected");

        USHORT ClientConfDescrNotify;
        m_btInterface->GetDescriptorValue(&ClientConfDescrNotify);
        ((CComboBox *)GetDlgItem(IDC_ALLOW_TO_SEND_CHAR_1))->SetCurSel(ClientConfDescrNotify);
    }

    pWin8BtInterface->RegisterNotification();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHelloClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHelloClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHelloClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHelloClientDlg::PostNcDestroy()
{
    CDialogEx::PostNcDestroy();
}


LRESULT CHelloClientDlg::OnConnected(WPARAM bConnected, LPARAM lparam)
{
    SetDlgItemText(IDC_DEVICE_STATE, bConnected ? L"Connected" : L"Disconnected");

    if (!bConnected)
        return S_OK;

    BYTE mBatteryLevel = 0;

    if (!m_bWin8)
    {
        UCHAR szManuName[9] = {0};
        UCHAR szModelNum[9] = {0};
        UCHAR SystemId[8] = {0};
        char buf[80];
        m_btInterface->GetSystemInfo(szManuName, 8, szModelNum, 8, SystemId, 8);
        SetDlgItemTextA(m_hWnd, IDC_MANU_NAME, (LPCSTR)szManuName);
        SetDlgItemTextA(m_hWnd, IDC_MODEL_NUM, (LPCSTR)szModelNum);
        sprintf_s (buf, "%02x %02x %02x %02x %02x %02x %02x %02x", SystemId[0], SystemId[1], SystemId[2], SystemId[3], SystemId[4], SystemId[5], SystemId[6], SystemId[7]);
        SetDlgItemTextA(m_hWnd, IDC_SYSTEM_ID, buf);

        m_btInterface->GetBatteryLevel(&mBatteryLevel);

        USHORT ClientConfDescrNotify = 0;
        m_btInterface->GetDescriptorValue(&ClientConfDescrNotify);

        ((CComboBox *)GetDlgItem(IDC_ALLOW_TO_SEND_CHAR_1))->SetCurSel(ClientConfDescrNotify);
    }
    SetDlgItemInt(IDC_BATTERY_LEVEL, mBatteryLevel, FALSE);
    return S_OK;
}

LRESULT CHelloClientDlg::OnNotified(WPARAM bConnected, LPARAM lparam)
{
    BTW_GATT_VALUE *pValue = (BTW_GATT_VALUE *)lparam;
    char buffer[30];
    memcpy (buffer, pValue->value, pValue->len);
    buffer[pValue->len] = 0;
    SetDlgItemTextA(m_hWnd, IDC_VALUE_CHAR_1, buffer);
    free (pValue);

    return S_OK;
}

void CHelloClientDlg::OnBnClickedReadChar1()
{
    BTW_GATT_VALUE value = {0};
    if (m_btInterface->GetHelloInput(&value))
    {
        value.value[value.len] = 0;
        SetDlgItemTextA(m_hWnd, IDC_VALUE_CHAR_1, (LPCSTR)value.value);
    }
}

void CHelloClientDlg::OnBnClickedReadChar2()
{
    BYTE Blinks = 0;
    m_btInterface->GetHelloConfig(&Blinks);
    SetDlgItemInt(IDC_VALUE_CHAR_2, Blinks);
}


void CHelloClientDlg::OnBnClickedWriteChar2()
{
    int Blinks = GetDlgItemInt(IDC_VALUE_CHAR_2);
    m_btInterface->SetHelloConfig((BYTE)Blinks);
}


void CHelloClientDlg::OnCbnSelchangeAllowToSendChar1()
{
    int Sel = ((CComboBox *)GetDlgItem(IDC_ALLOW_TO_SEND_CHAR_1))->GetCurSel();
    m_btInterface->SetDescriptorValue(Sel);
}

