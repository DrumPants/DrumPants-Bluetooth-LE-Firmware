
// HelloClientDlg.h : header file
//

#pragma once
#include "BtInterface.h"

class CBtWin8Interface : public CBtInterface
{
public:
    CBtWin8Interface (BLUETOOTH_ADDRESS *bth, HMODULE hLib, LPVOID NotificationContext);
    virtual ~CBtWin8Interface();

    BOOL Init();
    void GetSystemInfo(UCHAR *szManuName, USHORT cbManuName, UCHAR *szModelNum, USHORT cbModelNum, UCHAR *SystemId, USHORT cbSystemId);
    BOOL GetDescriptorValue(USHORT *DescriptorValue);
    BOOL SetDescriptorValue(USHORT Value);
    BOOL GetBatteryLevel(BYTE *BatteryLevel);
    BOOL GetHelloInput(BTW_GATT_VALUE *pValue);
    BOOL GetHelloConfig(BYTE *pBlinks);
    BOOL SetHelloConfig(BYTE Blinks);

    void RegisterNotification();
    BTW_GATT_VALUE *HandleNotification(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter);

    BOOL m_bConnected;
private:
    HANDLE m_hDevice;
    BTH_LE_GATT_SERVICE m_serviceBattery;
    BTH_LE_GATT_SERVICE m_serviceHello;
    BTH_LE_GATT_SERVICE m_serviceDevInfo;
    BTH_LE_GATT_CHARACTERISTIC m_charBatteryLevel;
    BTH_LE_GATT_CHARACTERISTIC m_charHelloConfig;
    BTH_LE_GATT_CHARACTERISTIC m_charHelloNotify;
    BTH_LE_GATT_DESCRIPTOR     m_descrClientConfig;
    BLUETOOTH_GATT_EVENT_HANDLE m_pEventHandle;
};

