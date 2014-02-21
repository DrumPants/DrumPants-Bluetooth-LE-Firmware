
// HelloClientDlg.h : header file
//

#pragma once

class CBtInterface
{
public:
    CBtInterface (BLUETOOTH_ADDRESS *bth, HMODULE hLib, LPVOID NotificationContext) {m_bth = *bth; m_hLib = hLib; m_NotificationContext = NotificationContext;};

    virtual BOOL Init() = NULL;
    virtual void GetSystemInfo(UCHAR *szManuName, USHORT cbManuName, UCHAR *szModelNum, USHORT cbModelNum, UCHAR *SystemId, USHORT cbSystemId) = NULL;
    virtual BOOL GetDescriptorValue(USHORT *DescriptorValue) = NULL;
    virtual BOOL SetDescriptorValue(USHORT Value) = NULL;
    virtual BOOL GetBatteryLevel(BYTE *BatteryLevel) = NULL;
    virtual BOOL GetHelloInput(BTW_GATT_VALUE *pValue) = NULL;
    virtual BOOL GetHelloConfig(BYTE *pBlinks) = NULL;
    virtual BOOL SetHelloConfig(BYTE Blinks) = NULL;

    BLUETOOTH_ADDRESS m_bth;
    HMODULE m_hLib;
    LPVOID m_NotificationContext;
}; 

