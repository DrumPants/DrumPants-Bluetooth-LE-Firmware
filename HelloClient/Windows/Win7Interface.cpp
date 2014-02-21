
// HelloClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include <setupapi.h>
#include "HelloClient.h"
#include "Win7Interface.h"
#include "HelloClientDlg.h"

void GattConnectCallback(LPVOID pRefData, BLUETOOTH_ADDRESS *pDevice, BOOL bConnected)
{
    ods ("Connected:%d\n", bConnected);
    PostMessage(((CHelloClientDlg *)pRefData)->m_hWnd, WM_CONNECTED, (WPARAM)bConnected, NULL); 
}

DWORD GattRequestCallback(
    LPVOID               pRefData,
    BLUETOOTH_ADDRESS   *pAddress, 
    GUID                *pGuidService, 
    DWORD               dwServiceInstance, 
    GUID                *pGuidChar, 
    DWORD               dwCharInstance,
    GUID                *pGuidDescr, 
    DWORD               security, 
    BTW_GATT_OPTYPE     Op, 
    BTW_GATT_VALUE      *pValue
)
{
    ods ("Op:%d\n", Op);
    if ((*pGuidService == guidSvcHello) && (*pGuidChar == guidCharHelloNotify))
    {
        if ((Op == BTW_GATT_OPTYPE_NOTIFICATION) || (Op == BTW_GATT_OPTYPE_INDICATION))
        {
            BTW_GATT_VALUE *p = (BTW_GATT_VALUE *)malloc (sizeof (BTW_GATT_VALUE));
            if (p)
            {
                *p = *pValue;
                PostMessage(((CHelloClientDlg *)pRefData)->m_hWnd, WM_NOTIFIED, (WPARAM)Op, (LPARAM)p);
            }
        }
    }
    return ERROR_SUCCESS;
}

CBtWin7Interface::CBtWin7Interface (BLUETOOTH_ADDRESS *bth, HMODULE hLib, LPVOID NotificationContext) 
    : CBtInterface(bth, hLib, NotificationContext) 
    , m_hReg(INVALID_HANDLE_VALUE)
{
}

CBtWin7Interface::~CBtWin7Interface()
{
    if (m_hReg != INVALID_HANDLE_VALUE)
    {
        FP_BtwGattDeregister pDereg = (FP_BtwGattDeregister)GetProcAddress(m_hLib, "BtwGattDeregister");
        if (pDereg)
            (*pDereg)(m_hReg);
    }
}

BOOL CBtWin7Interface::Init()
{
    if (m_hLib != NULL)
    {
        FP_BtwGattRegister p = (FP_BtwGattRegister)GetProcAddress(m_hLib, "BtwGattRegister");
        if (p)
        {
            DWORD rc = (*p)(&m_bth, &GattConnectCallback, NULL, &GattRequestCallback, m_NotificationContext, &m_hReg);
            return (rc == ERROR_SUCCESS);
        }
    }
    return FALSE;
}

BOOL CBtWin7Interface::GetHelloInput(BTW_GATT_VALUE *pValue)
{
    ods("+%S\n", __FUNCTIONW__);
    FP_BtwGattReadCharacteristic pReadChar = (FP_BtwGattReadCharacteristic)GetProcAddress(m_hLib, "BtwGattReadCharacteristic");
    DWORD dwResult = 0;
    if (pReadChar)
    {
        dwResult = (*pReadChar)(m_hReg, &m_bth, &guidSvcHello, 0, &guidCharHelloNotify, 0, 0, pValue, TRUE, this);
        ods("-%S %d\n", __FUNCTIONW__, dwResult);
        return (dwResult == ERROR_SUCCESS);
    }
    ods("-%S %d\n", __FUNCTIONW__, dwResult);
    return FALSE;
}

BOOL CBtWin7Interface::GetHelloConfig(BYTE *pBlinks)
{
    ods("+%S\n", __FUNCTIONW__);
    BTW_GATT_VALUE value;
    DWORD dwResult = 0;
    FP_BtwGattReadCharacteristic pReadChar = (FP_BtwGattReadCharacteristic)GetProcAddress(m_hLib, "BtwGattReadCharacteristic");
    if (pReadChar)
    {
        dwResult = (*pReadChar)(m_hReg, &m_bth, &guidSvcHello, 0, &guidCharHelloConfig, 0, 0, &value, TRUE, this);
        *pBlinks = value.value[0];
        ods("-%S %d\n", __FUNCTIONW__, dwResult);
        return (dwResult == ERROR_SUCCESS);
    }
    ods("-%S %d\n", __FUNCTIONW__, dwResult);
    return FALSE;
}

BOOL CBtWin7Interface::SetHelloConfig(BYTE Blinks)
{
    ods("+%S\n", __FUNCTIONW__);
    BTW_GATT_VALUE value = {1, Blinks};
    DWORD dwResult = 0;
    FP_BtwGattWriteCharacteristic pWriteChar = (FP_BtwGattWriteCharacteristic)GetProcAddress(m_hLib, "BtwGattWriteCharacteristic");
    if (pWriteChar)
    {
        dwResult = (*pWriteChar)(m_hReg, &m_bth, &guidSvcHello, 0, &guidCharHelloConfig, 0, 0, &value, TRUE, this);
        ods("-%S %d\n", __FUNCTIONW__, dwResult);
        return (dwResult == ERROR_SUCCESS);
    }
    ods("-%S %d\n", __FUNCTIONW__, dwResult);
    return FALSE;
}

BOOL CBtWin7Interface::GetBatteryLevel(BYTE *BatteryLevel)
{
    ods("+%S\n", __FUNCTIONW__);
    BTW_GATT_VALUE value = {0};
    DWORD dwResult = 0;
    FP_BtwGattReadCharacteristic pReadChar = (FP_BtwGattReadCharacteristic)GetProcAddress(m_hLib, "BtwGattReadCharacteristic");
    if (pReadChar)
    {
        dwResult = (*pReadChar)(m_hReg, &m_bth, &guidSvcBattery, 0, &guidCharBatLevel, 0, 0, &value, TRUE, this);
        *BatteryLevel = value.value[0];
        ods("-%S %d\n", __FUNCTIONW__, dwResult);
        return TRUE;
    }
    ods("-%S %d\n", __FUNCTIONW__, dwResult);
    return FALSE;
}

BOOL CBtWin7Interface::GetDescriptorValue(USHORT *Value)
{
    ods("+%S\n", __FUNCTIONW__);
    DWORD dwResult;
    BTW_GATT_VALUE value = {0};
    FP_BtwGattReadDescriptor pReadDescr = (FP_BtwGattReadDescriptor)GetProcAddress(m_hLib, "BtwGattReadDescriptor");
    if (pReadDescr)
    {
        value.value[0] = 0;
        value.value[1] = 0;
        value.len = 2;

        dwResult = (*pReadDescr)(m_hReg, &m_bth, &guidSvcHello, 0, &guidCharHelloNotify, 0, &guidClntConfigDesc, 0, &value, TRUE, NULL);
        if (dwResult == ERROR_SUCCESS)
        {
            *Value = value.value[0] + (value.value[1] << 8);
            ods("-%S %d\n", __FUNCTIONW__, dwResult);
            return TRUE;
        }
    }
    ods("-%S %d\n", __FUNCTIONW__, dwResult);
    return FALSE;
}

BOOL CBtWin7Interface::SetDescriptorValue(USHORT Value)
{
    ods("+%S\n", __FUNCTIONW__);
    BTW_GATT_VALUE value = {0};
    value.len = 2;
    value.value[0] = (BYTE)(Value & 0xff);
    value.value[1] = (BYTE)((Value >> 8) &0xff);

    FP_BtwGattWriteDescriptor pWriteDescr = (FP_BtwGattWriteDescriptor)GetProcAddress(m_hLib, "BtwGattWriteDescriptor");
    if (pWriteDescr)
    {
        DWORD dwResult = (*pWriteDescr)(m_hReg, &m_bth, &guidSvcHello, 0, &guidCharHelloNotify, 0, &guidClntConfigDesc, 0, &value, TRUE, NULL);
        ods("-%S %d\n", __FUNCTIONW__, dwResult);
        return (dwResult == ERROR_SUCCESS);
    }
    ods("-%S\n", __FUNCTIONW__);
    return FALSE;
}

void CBtWin7Interface::GetSystemInfo(UCHAR *szManuName, USHORT cbManuName, UCHAR *szModelNum, USHORT cbModelNum, UCHAR *SystemId, USHORT cbSystemId)
{
    ods("+%S\n", __FUNCTIONW__);
    BTW_GATT_VALUE value = {0};
    FP_BtwGattReadCharacteristic pReadChar = (FP_BtwGattReadCharacteristic)GetProcAddress(m_hLib, "BtwGattReadCharacteristic");
    if (pReadChar)
    {
        (*pReadChar)(m_hReg, &m_bth, &guidSvcDeviceInfo, 0, &guidCharManufacturer, 0, 0, &value, TRUE, this);
        memcpy (szManuName, value.value, value.len < cbManuName ? value.len : cbManuName);
        (*pReadChar)(m_hReg, &m_bth, &guidSvcDeviceInfo, 0, &guidCharModelNumber, 0, 0, &value, TRUE, this);
        memcpy (szModelNum, value.value, value.len < cbModelNum ? value.len : cbModelNum);
        (*pReadChar)(m_hReg, &m_bth, &guidSvcDeviceInfo, 0, &guidCharSystemId, 0, 0, &value, TRUE, this);
        memcpy (SystemId, value.value, value.len < cbSystemId ? value.len : cbSystemId);
    }
    ods("-%S\n", __FUNCTIONW__);
}

