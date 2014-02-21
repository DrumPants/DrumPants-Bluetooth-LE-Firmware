
// HelloClientDlg.h : header file
//

#pragma once
#include "BtInterface.h"

// Windows 7 definitions
typedef DWORD (WINAPI *FP_BtwGattRegister)
(
    const BLUETOOTH_ADDRESS    *pAddress, 
    PFN_GATT_CONNECT_CALLBACK  pfnConnectCallback,
    PFN_GATT_COMPLETE_CALLBACK pfnCompleteCallback,
    PFN_GATT_REQUEST_CALLBACK  pfnRequestCallback,
    PVOID                      pRefData,
    HANDLE                     *phReg
);

typedef void (WINAPI *FP_BtwGattDeregister)(HANDLE hReg);

typedef DWORD (WINAPI *FP_BtwGattWriteDescriptor)
(
    HANDLE                      hReg,               // handle of the application registration
    BLUETOOTH_ADDRESS           *pAddress,          // address of the device to read
    const GUID                  *pGuidService,      // guid of the service
    DWORD                       dwServiceInstance,  // instance of the service (most likely 0)
    const GUID                  *pGuidChar,         // guid of the char
    DWORD                       dwCharInstance,     // instance of the char (most likely 0)
    const GUID                  *pGuidDescr,        // guid of the char descriptor
    DWORD                       security,           // required security
    BTW_GATT_VALUE              *pValue,            // pointer to the result, can be NULL if completion is used
    BOOL                        bWait,              // if False returns right away
    LPVOID                      pRefData            // pointer returned in the completion
);

typedef DWORD (WINAPI *FP_BtwGattReadCharacteristic)
(
    HANDLE                      hReg,               // handle of the application registration
    BLUETOOTH_ADDRESS           *pAddress,          // address of the device to read
    const GUID                  *pGuidService,      // guid of the service
    DWORD                       dwServiceInstance,  // instance of the service (most likely 0)
    const GUID                  *pGuidChar,         // guid of the char
    DWORD                       dwCharInstance,     // instance of the char (most likely 0)
    DWORD                       security,           // required security
    BTW_GATT_VALUE              *pValue,            // pointer to the result, can be NULL if completion is used
    BOOL                        bWait,              // if False returns right away
    LPVOID                      pRefData            // pointer returned in the completion
);

typedef DWORD (WINAPI *FP_BtwGattWriteCharacteristic)
(
    HANDLE                      hReg,               // handle of the application registration
    BLUETOOTH_ADDRESS           *pAddress,          // address of the device to read
    const GUID                  *pGuidService,      // guid of the service
    DWORD                       dwServiceInstance,  // instance of the service (most likely 0)
    const GUID                  *pGuidChar,         // guid of the char
    DWORD                       dwCharInstance,     // instance of the char (most likely 0)
    DWORD                       security,           // required security
    BTW_GATT_VALUE              *pValue,            // pointer to the result, can be NULL if completion is used
    BOOL                        bWait,              // if False returns right away
    LPVOID                      pRefData            // pointer returned in the completion
);


class CBtWin7Interface : public CBtInterface
{
public:
    CBtWin7Interface (BLUETOOTH_ADDRESS *bth, HMODULE hLib, LPVOID NotificationContext);
    virtual ~CBtWin7Interface();

    BOOL Init();
    void GetSystemInfo(UCHAR *szManuName, USHORT cbManuName, UCHAR *szModelNum, USHORT cbModelNum, UCHAR *SystemId, USHORT cbSystemId);
    BOOL GetDescriptorValue(USHORT *DescriptorValue);
    BOOL SetDescriptorValue(USHORT Value);
    BOOL GetBatteryLevel(BYTE *BatteryLevel);
    BOOL GetHelloInput(BTW_GATT_VALUE *pValue);
    BOOL GetHelloConfig(BYTE *pBlinks);
    BOOL SetHelloConfig(BYTE Blinks);

private:
    HANDLE m_hReg;
};

