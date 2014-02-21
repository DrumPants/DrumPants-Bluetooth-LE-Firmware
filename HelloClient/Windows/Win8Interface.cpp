
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

GUID GUID_BLUETOOTHLE_DEVICE_INTERFACE = {0x781aee18, 0x7733, 0x4ce4, {0xad, 0xd0, 0x91, 0xf4, 0x1c, 0x67, 0xb5, 0x92}};

HANDLE OpenBleDevice(BLUETOOTH_ADDRESS *btha)
{
    HDEVINFO                            hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA            deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA    deviceInterfaceDetailData = NULL;
    ULONG                               predictedLength = 0;
    ULONG                               requiredLength = 0, bytes=0;
	WCHAR								szBda[13] = {0};
	HANDLE								hDevice = INVALID_HANDLE_VALUE;
	DWORD								err;

    if ((hardwareDeviceInfo = SetupDiGetClassDevs ((LPGUID)&GUID_BLUETOOTHLE_DEVICE_INTERFACE,
			NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE))) == INVALID_HANDLE_VALUE)
    {
        ods("SetupDiGetClassDevs failed: %x\n", GetLastError());
        return hDevice;
    }

    deviceInterfaceData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

	// Enumerate devices of LE_DEVICE interface class
    for (int i = 0; ; i++) 
	{
        if (!SetupDiEnumDeviceInterfaces (hardwareDeviceInfo, 0, 
				(LPGUID)&GUID_BLUETOOTHLE_DEVICE_INTERFACE, i, &deviceInterfaceData)) 
		{
			if ((err = GetLastError()) == ERROR_NO_MORE_ITEMS) 
				ods ("OpenBleDevice device not found\n");
			else
				ods ("OpenBleDevice:ERROR SetupDiEnumDeviceInterfaces failed:%d\n", err);
			break;
		}
        SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInterfaceData,
				NULL, 0, &requiredLength, NULL);

		if ((err = GetLastError()) != ERROR_INSUFFICIENT_BUFFER) 
		{
            ods("OpenBleDevice:ERROR SetupDiGetDeviceInterfaceDetail failed %d\n", err);
            break;
        }

        predictedLength = requiredLength;

        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) new CHAR[predictedLength + 2];
        if (deviceInterfaceDetailData == NULL) 
		{
            ods("OpenBleDevice:ERROR Couldn't allocate %d bytes for device interface details.\n", predictedLength);
			break;
        }
		RtlZeroMemory (deviceInterfaceDetailData, predictedLength + 2);
        deviceInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
        if (!SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInterfaceData,
			deviceInterfaceDetailData, predictedLength, &requiredLength, NULL)) 
		{
            ods("OpenBleDevice:ERROR SetupDiGetDeviceInterfaceDetail\n");
			delete deviceInterfaceDetailData;
			break;
        }

		_wcsupr_s (deviceInterfaceDetailData->DevicePath, wcslen(deviceInterfaceDetailData->DevicePath) + 1);
		BdaToString (szBda, btha);
		if (wcsstr (deviceInterfaceDetailData->DevicePath, szBda) != NULL)
		{
			ods("Opening interface:%S\n", deviceInterfaceDetailData->DevicePath);

			hDevice = CreateFile ( deviceInterfaceDetailData->DevicePath,
				GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

			delete deviceInterfaceDetailData;
			break;
		}
		delete deviceInterfaceDetailData;
	}
    SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
	return hDevice;
}

HANDLE OpenBleService(BLUETOOTH_ADDRESS *btha, GUID *guid)
{
    HDEVINFO                            hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA            deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA    deviceInterfaceDetailData = NULL;
    ULONG                               predictedLength = 0;
    ULONG                               requiredLength = 0, bytes=0;
	WCHAR								szBda[13] = {0};
	HANDLE								hService = INVALID_HANDLE_VALUE;
	DWORD								err;

    if ((hardwareDeviceInfo = SetupDiGetClassDevs ((LPGUID)guid,
			NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE))) == INVALID_HANDLE_VALUE)
    {
        ods("OpenBleServiceSetupDiGetClassDevs failed: %x\n", GetLastError());
        return hService;
    }

    deviceInterfaceData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

	// Enumerate devices of LE_DEVICE interface class
    for (int i = 0; ; i++) 
	{
        if (!SetupDiEnumDeviceInterfaces (hardwareDeviceInfo, 0, guid, i, &deviceInterfaceData)) 
		{
			if ((err = GetLastError()) == ERROR_NO_MORE_ITEMS) 
				ods("OpenBleService device not found\n");
			else
				ods ("OpenBleService:ERROR SetupDiEnumDeviceInterfaces failed:%d\n", err);
			break;
		}
        SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInterfaceData,
				NULL, 0, &requiredLength, NULL);

		if ((err = GetLastError()) != ERROR_INSUFFICIENT_BUFFER) 
		{
            ods("OpenBleService:ERROR SetupDiGetDeviceInterfaceDetail failed %d\n", err);
            break;
        }

        predictedLength = requiredLength;

        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) new CHAR[predictedLength + 2];
        if (deviceInterfaceDetailData == NULL) 
		{
            ods("OpenBleService:ERROR Couldn't allocate %d bytes for device interface details.\n", predictedLength);
			break;
        }
		RtlZeroMemory (deviceInterfaceDetailData, predictedLength + 2);
        deviceInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
        if (!SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInterfaceData,
			deviceInterfaceDetailData, predictedLength, &requiredLength, NULL)) 
		{
            ods("OpenBleService :ERROR SetupDiGetDeviceInterfaceDetail\n");
			delete deviceInterfaceDetailData;
			break;
        }

		_wcsupr_s (deviceInterfaceDetailData->DevicePath, wcslen(deviceInterfaceDetailData->DevicePath) + 1);
		BdaToString (szBda, btha);
		if (wcsstr (deviceInterfaceDetailData->DevicePath, szBda) != NULL)
		{
			ods("OpenBleService Opening interface:%S\n", deviceInterfaceDetailData->DevicePath);

			hService = CreateFile ( deviceInterfaceDetailData->DevicePath,
				GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

			if(hService == INVALID_HANDLE_VALUE)
				ods("OpenBleService (hService == INVALID_HANDLE_VALUE) GetLastError() = %d\n", GetLastError());

			delete deviceInterfaceDetailData;
			break;
		}
		delete deviceInterfaceDetailData;
	}
    SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
	return hService;
}

VOID WINAPI NotificationCallback(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
    BTW_GATT_VALUE *pValue;
    ods ("EventType:%d\n", EventType);
    CHelloClientDlg *pDlg = (CHelloClientDlg *)Context;
    CBtWin8Interface *pBtInterface = (CBtWin8Interface *)(pDlg->m_btInterface);
    if ((pValue = pBtInterface->HandleNotification(EventType, EventOutParameter)) != NULL)
        pDlg->PostMessage(WM_NOTIFIED, (WPARAM)BTW_GATT_OPTYPE_NOTIFICATION, (LPARAM)pValue);
}


CBtWin8Interface::CBtWin8Interface (BLUETOOTH_ADDRESS *bth, HMODULE hLib, LPVOID NotificationContext) 
    : CBtInterface(bth, hLib, NotificationContext) 
    , m_hDevice(INVALID_HANDLE_VALUE)
    , m_pEventHandle(INVALID_HANDLE_VALUE)
{
}

BTW_GATT_VALUE *CBtWin8Interface::HandleNotification(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter)
{
    BLUETOOTH_GATT_VALUE_CHANGED_EVENT *pEvent = (BLUETOOTH_GATT_VALUE_CHANGED_EVENT *)EventOutParameter;
    if (pEvent->ChangedAttributeHandle == m_charHelloNotify.AttributeHandle)
    {
        BTW_GATT_VALUE *p = (BTW_GATT_VALUE *)malloc (sizeof (BTW_GATT_VALUE));
        if (p)
        {
            p->len = (USHORT)pEvent->CharacteristicValue->DataSize;
            memcpy (p->value, pEvent->CharacteristicValue->Data, pEvent->CharacteristicValue->DataSize);
            return p;
        }
    }
    return NULL;
}


BOOL CBtWin8Interface::Init()
{
    if ((m_hDevice = OpenBleDevice(&m_bth)) == INVALID_HANDLE_VALUE)
        return FALSE;

	USHORT num = 0;
	HRESULT hr;

    FP_BluetoothGATTGetServices pBluetoothGATTGetServices = (FP_BluetoothGATTGetServices)GetProcAddress(m_hLib, "BluetoothGATTGetServices");
	hr = pBluetoothGATTGetServices(m_hDevice, 0, NULL, &num, BLUETOOTH_GATT_FLAG_NONE);
    if (num == 0)
        return FALSE;

    PBTH_LE_GATT_SERVICE pServices = (PBTH_LE_GATT_SERVICE) malloc(sizeof(BTH_LE_GATT_SERVICE) * num);
    if (pServices == NULL) 
        return FALSE;

    RtlZeroMemory(pServices, sizeof(BTH_LE_GATT_SERVICE) * num);
    
    hr = pBluetoothGATTGetServices(m_hDevice, num, pServices, &num, BLUETOOTH_GATT_FLAG_NONE);
    if (S_OK != hr) 
    {
        ods("BluetoothGATTGetServices - Actual Data %x", hr);
        return FALSE;
    }

    // search for Hello and Battery services.
    _BTH_LE_UUID uuidBattery, uuidHello, uuidDevInfo;
    uuidBattery.IsShortUuid = TRUE;
    uuidBattery.Value.ShortUuid = BTW_GATT_UUID_SERVCLASS_BATTERY;
    uuidHello.IsShortUuid = FALSE;
    uuidHello.Value.LongUuid = UUID_HELLO_SERVICE;
    uuidDevInfo.IsShortUuid = TRUE;
    uuidDevInfo.Value.ShortUuid = BTW_GATT_UUID_SERVCLASS_DEVICE_INFO;
    for (int i = 0; i < num; i++)
    {
        if (IsBthLEUuidMatch(pServices[i].ServiceUuid, uuidBattery))
            m_serviceBattery = pServices[i];
        else if (IsBthLEUuidMatch(pServices[i].ServiceUuid, uuidHello))
            m_serviceHello = pServices[i];
        else if (IsBthLEUuidMatch(pServices[i].ServiceUuid, uuidDevInfo))
            m_serviceDevInfo = pServices[i];
    }

    PBTH_LE_GATT_CHARACTERISTIC pChars;

    FP_BluetoothGATTGetCharacteristics pBluetoothGATTGetCharacteristics = (FP_BluetoothGATTGetCharacteristics)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristics");
	pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceBattery, 0, NULL, &num, BLUETOOTH_GATT_FLAG_NONE);
	if (num != 0)
	{
	    if ((pChars = new BTH_LE_GATT_CHARACTERISTIC[num]) == NULL)
            return FALSE;

        if ((pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceBattery, num, pChars, &num, BLUETOOTH_GATT_FLAG_NONE)) != S_OK)
	    {
            ods("LoadCharacteristics hr:0x%x\n", hr);
		    delete [] pChars;
            return FALSE;
	    }
    }
    // search for battery level characteristic.
    _BTH_LE_UUID uuidBatteryLevel;
    uuidBatteryLevel.IsShortUuid = TRUE;
    uuidBatteryLevel.Value.ShortUuid = BTW_GATT_UUID_CHAR_BATTERY_LEVEL;
    for (int i = 0; i < num; i++)
    {
        if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, uuidBatteryLevel))
            m_charBatteryLevel = pChars[i];
    }
    delete [] pChars;

    // Go through all characteristics of the Hello services to find Config and Notify
    num = 0;
	pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceHello, 0, NULL, &num, BLUETOOTH_GATT_FLAG_NONE);
	if (num != 0)
	{
	    if ((pChars = new BTH_LE_GATT_CHARACTERISTIC[num]) == NULL)
		    return FALSE;

        if ((pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceHello, num, pChars, &num, BLUETOOTH_GATT_FLAG_NONE)) != S_OK)
	    {
            ods("LoadCharacteristics hr:0x%x\n", hr);
		    delete [] pChars;
		    return FALSE;
	    }
    }
    // search for Hello and Battery services.
    m_charHelloNotify.AttributeHandle = 0;
    m_charHelloConfig.AttributeHandle = 0;

    _BTH_LE_UUID charHelloConfig, charHelloNotify;
    charHelloConfig.IsShortUuid = FALSE;
    charHelloConfig.Value.LongUuid = UUID_HELLO_CHARACTERISTIC_CONFIG;
    charHelloNotify.IsShortUuid = FALSE;
    charHelloNotify.Value.LongUuid = UUID_HELLO_CHARACTERISTIC_NOTIFY;
    for (int i = 0; i < num; i++)
    {
        if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, charHelloConfig))
            m_charHelloConfig = pChars[i];
        else if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, charHelloNotify))
            m_charHelloNotify = pChars[i];

        PBTH_LE_GATT_DESCRIPTOR pDescrs = NULL;
        USHORT numDescr = 0;

        FP_BluetoothGATTGetDescriptors pBluetoothGATTGetDescriptors = (FP_BluetoothGATTGetDescriptors)GetProcAddress(m_hLib, "BluetoothGATTGetDescriptors");
	    pBluetoothGATTGetDescriptors(m_hDevice, &pChars[i], 0, NULL, &numDescr, BLUETOOTH_GATT_FLAG_NONE);
	    if (numDescr != 0)
	    {
	        if ((pDescrs = new BTH_LE_GATT_DESCRIPTOR[numDescr]) == NULL)
		        break;

            if ((pBluetoothGATTGetDescriptors(m_hDevice, &pChars[i], numDescr, pDescrs, &numDescr, BLUETOOTH_GATT_FLAG_NONE)) != S_OK)
	        {
                ods("LoadCharacteristics hr:0x%x\n", hr);
		        delete [] pDescrs;
		        break;
	        }
        }

        // search for client configuration descriptor.
        _BTH_LE_UUID uuidClientConfig;
        uuidClientConfig.IsShortUuid = TRUE;
        uuidClientConfig.Value.ShortUuid = BTW_GATT_UUID_DESCRIPTOR_CLIENT_CONFIG;

        for (int j = 0; j < numDescr; j++)
        {
            if (IsBthLEUuidMatch(pDescrs[j].DescriptorUuid, uuidClientConfig))
            {
                m_descrClientConfig = pDescrs[j];
                break;
            }
        }
        delete []pDescrs;
    }
    free (pServices);
    return TRUE;
}

void CBtWin8Interface::RegisterNotification()
{
    // register to receive notification
    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return;

    BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION reg;
    reg.NumCharacteristics = 1;
    reg.Characteristics[0] = m_charHelloNotify;

    if (m_pEventHandle == INVALID_HANDLE_VALUE)
    {
        FP_BluetoothGATTRegisterEvent pReg = (FP_BluetoothGATTRegisterEvent)GetProcAddress(m_hLib, "BluetoothGATTRegisterEvent");
        if (pReg)
        {
            HRESULT hr = pReg(hService, CharacteristicValueChangedEvent, &reg, &NotificationCallback, m_NotificationContext, &m_pEventHandle, BLUETOOTH_GATT_FLAG_NONE);
            ods ("BluetoothGATTRegisterEvent hr:0x%x\n", hr);
        }
    }
    CloseHandle(hService);
}

BOOL CBtWin8Interface::GetDescriptorValue(USHORT *Value)
{
    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    FP_BluetoothGATTGetDescriptorValue pGetDescr = (FP_BluetoothGATTGetDescriptorValue)GetProcAddress(m_hLib, "BluetoothGATTGetDescriptorValue");
    if (pGetDescr == NULL)
    {
        CloseHandle(hService);
        return FALSE;
    }

    BTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue;
    RtlZeroMemory(&DescriptorValue, sizeof(DescriptorValue));

    USHORT descValueDataSize;
    PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer;
    HRESULT hr = pGetDescr(hService, &m_descrClientConfig, 0, NULL, &descValueDataSize, BLUETOOTH_GATT_FLAG_NONE);
    ods ("BluetoothGATTGetDescriptorValue hr:0x%x\n", hr);

    if (descValueDataSize)
    {
        pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);
        if (pDescValueBuffer == NULL) 
        {
            ods("pDescValueBuffer out of memory\r\n");
            CloseHandle(hService);
            return FALSE;
        } 
        RtlZeroMemory(pDescValueBuffer, descValueDataSize);

        // Force reading from the device.  If failed device is not connected
        hr = pGetDescr(hService, &m_descrClientConfig, (ULONG)descValueDataSize, pDescValueBuffer, NULL, BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_DEVICE);
        if (hr == HRESULT_FROM_WIN32(ERROR_SEM_TIMEOUT))
            m_bConnected = FALSE;
        else
            *Value = pDescValueBuffer->ClientCharacteristicConfiguration.IsSubscribeToNotification +
                    (pDescValueBuffer->ClientCharacteristicConfiguration.IsSubscribeToIndication << 1);

        free (pDescValueBuffer);
    }
    CloseHandle(hService);
    return TRUE;
}

BOOL CBtWin8Interface::SetDescriptorValue(USHORT Value)
{
    HRESULT hr = E_FAIL;
    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    FP_BluetoothGATTSetDescriptorValue pSetDescr = (FP_BluetoothGATTSetDescriptorValue)GetProcAddress(m_hLib, "BluetoothGATTSetDescriptorValue");
    if (pSetDescr)
    {
        BTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue;

        RtlZeroMemory(&DescriptorValue, sizeof(DescriptorValue));

        DescriptorValue.DescriptorType = ClientCharacteristicConfiguration;
        DescriptorValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = (Value == 1);
        DescriptorValue.ClientCharacteristicConfiguration.IsSubscribeToIndication = (Value == 2);

        HRESULT hr = pSetDescr(hService, &m_descrClientConfig, &DescriptorValue, BLUETOOTH_GATT_FLAG_NONE);
        ods ("BluetoothGATTSetDescriptorValue hr:0x%x\n", hr);
    }
    CloseHandle(hService);
    return hr == S_OK;
}

BOOL CBtWin8Interface::GetHelloInput(BTW_GATT_VALUE *pValue)
{
    if (m_charHelloNotify.AttributeHandle == 0)
        return FALSE;

    FP_BluetoothGATTGetCharacteristicValue pReadChar = (FP_BluetoothGATTGetCharacteristicValue)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristicValue");
    if (pReadChar == NULL)
        return FALSE;

    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    USHORT charValueDataSize;
    HRESULT hr = pReadChar(hService, &m_charHelloNotify, 0, NULL, &charValueDataSize, BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);

    if (charValueDataSize)
    {
        PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);
        if (pCharValueBuffer)
        {
            hr = pReadChar(hService, &m_charHelloNotify, (ULONG)charValueDataSize, pCharValueBuffer, &charValueDataSize, BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_DEVICE | BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);
            memcpy (pValue->value, pCharValueBuffer->Data, pCharValueBuffer->DataSize);
            pValue->len = (USHORT)pCharValueBuffer->DataSize;
            free (pCharValueBuffer);
            CloseHandle (hService);
            return (hr == S_OK);
        }
    }
    CloseHandle (hService);
    return FALSE;
}

BOOL CBtWin8Interface::GetHelloConfig(BYTE *pBlinks)
{
    FP_BluetoothGATTGetCharacteristicValue pReadChar = (FP_BluetoothGATTGetCharacteristicValue)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristicValue");
    if (pReadChar == NULL)
        return FALSE;

    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    USHORT charValueDataSize;
    HRESULT hr = pReadChar(hService, &m_charHelloConfig, 0, NULL, &charValueDataSize, BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);

    if (charValueDataSize)
    {
        PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);
        if (pCharValueBuffer)
        {
            hr = pReadChar(hService, &m_charHelloConfig, (ULONG)charValueDataSize, pCharValueBuffer, &charValueDataSize, BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_DEVICE | BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);
            *pBlinks = pCharValueBuffer->Data[0];
            free (pCharValueBuffer);
            CloseHandle (hService);
            return (hr == S_OK);
        }
    }
    CloseHandle (hService);
    return FALSE;
}

BOOL CBtWin8Interface::SetHelloConfig(BYTE Blinks)
{
    FP_BluetoothGATTSetCharacteristicValue pWriteChar = (FP_BluetoothGATTSetCharacteristicValue)GetProcAddress(m_hLib, "BluetoothGATTSetCharacteristicValue");
    if (pWriteChar == NULL)
        return FALSE;

    GUID guid = UUID_HELLO_SERVICE;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    BTH_LE_GATT_CHARACTERISTIC_VALUE Value;
    RtlZeroMemory(&Value, sizeof(Value));

    Value.DataSize = 1;
    Value.Data[0] = (BYTE)Blinks;

    pWriteChar(hService, &m_charHelloConfig, &Value, NULL, BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);

    CloseHandle (hService);
    return FALSE;
}

BOOL CBtWin8Interface::GetBatteryLevel(BYTE *BatteryLevel)
{
    HANDLE hService = OpenBleService(&m_bth, (GUID *)&guidSvcBattery);
    if (hService == INVALID_HANDLE_VALUE)
        return FALSE;

    FP_BluetoothGATTGetCharacteristicValue pGetChar = (FP_BluetoothGATTGetCharacteristicValue)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristicValue");
    if (pGetChar == NULL)
    {
        CloseHandle(hService);
        return FALSE;
    }
    USHORT sizeValue = 0;
    PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue;

    HRESULT hr = pGetChar(hService, &m_charBatteryLevel, 0, NULL, &sizeValue, BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);
    ods ("BluetoothGATTGetCharacteristicValue hr:0x%x\n", hr);
	if (sizeValue == 0)
    {
        CloseHandle(hService);
        return FALSE;
    }
	if ((CharacteristicValue = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)new BYTE[sizeValue]) == NULL)
    {
        CloseHandle(hService);
        return FALSE;
    }
    hr = pGetChar(hService, &m_charBatteryLevel, sizeValue, CharacteristicValue, &sizeValue, BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_DEVICE | BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED);
    ods ("BluetoothGATTGetCharacteristicValue hr:0x%x\n", hr);
                    
        if (hr == HRESULT_FROM_WIN32(ERROR_SEM_TIMEOUT))
            m_bConnected = FALSE;
        else
            *BatteryLevel = CharacteristicValue->Data[0];

    delete[] CharacteristicValue;
    CloseHandle(hService);
    return TRUE;
}

void CBtWin8Interface::GetSystemInfo(UCHAR *szManuName, USHORT cbManuName, UCHAR *szModelNum, USHORT cbModelNum, UCHAR *SystemId, USHORT cbSystemId)
{
    GUID guid = bt_guid;
    guid.Data1 = BTW_GATT_UUID_SERVCLASS_DEVICE_INFO;
    HANDLE hService = OpenBleService(&m_bth, &guid);
    if (!hService)
    {
        ods("No Device Info service\n");
        return;
    }
    USHORT num = 0;
    FP_BluetoothGATTGetCharacteristics pBluetoothGATTGetCharacteristics = (FP_BluetoothGATTGetCharacteristics)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristics");
	HRESULT hr = pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceDevInfo, 0, NULL, &num, BLUETOOTH_GATT_FLAG_NONE);
	if (num == 0)
    {
        ods("No Device Info service hr:0x%x\n", hr);
        CloseHandle(hService);
        return;
    }
    PBTH_LE_GATT_CHARACTERISTIC pChars;
	if ((pChars = new BTH_LE_GATT_CHARACTERISTIC[num]) == NULL)
    {
        CloseHandle(hService);
		return;
    }
    if ((hr = pBluetoothGATTGetCharacteristics(m_hDevice, &m_serviceDevInfo, num, pChars, &num, BLUETOOTH_GATT_FLAG_NONE)) != S_OK)
	{
        ods("***GetSystemInfo:LoadCharacteristics hr:0x%x\n", hr);
		delete [] pChars;
        CloseHandle(hService);
		return;
	}
    _BTH_LE_UUID uuidManufacturer, uuidModel, uuidSystem;
    uuidManufacturer.IsShortUuid = uuidModel.IsShortUuid = uuidSystem.IsShortUuid = TRUE;
    uuidSystem.Value.ShortUuid = BTW_GATT_UUID_CHAR_DIS_SYSTEM_ID;
    uuidModel.Value.ShortUuid = BTW_GATT_UUID_CHAR_DIS_MODEL_NUMBER;
    uuidManufacturer.Value.ShortUuid = BTW_GATT_UUID_CHAR_DIS_MANUFACTURER_NAME;
    for (int i = 0; i < num; i++)
    {
        FP_BluetoothGATTGetCharacteristicValue pGetChar = (FP_BluetoothGATTGetCharacteristicValue)GetProcAddress(m_hLib, "BluetoothGATTGetCharacteristicValue");
        if (pGetChar)
        {
            USHORT sizeValue = 0;
            PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue;

            HRESULT hr = pGetChar(hService, &pChars[i], 0, NULL, &sizeValue, BLUETOOTH_GATT_FLAG_NONE);
            ods ("BluetoothGATTGetCharacteristicValue hr:0x%x\n", hr);
	        if (sizeValue != 0)
	        {
	            if ((CharacteristicValue = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)new BYTE[sizeValue]) != NULL)
                {
                    HRESULT hr = pGetChar(hService, &pChars[i], sizeValue, CharacteristicValue, &sizeValue, BLUETOOTH_GATT_FLAG_NONE);
                    ods ("BluetoothGATTGetCharacteristicValue hr:0x%x\n", hr);
                    
                    if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, uuidManufacturer))
                        memcpy (szManuName, CharacteristicValue->Data, CharacteristicValue->DataSize < cbManuName ? CharacteristicValue->DataSize : cbManuName);
                    else if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, uuidModel))
                        memcpy (szModelNum, CharacteristicValue->Data, CharacteristicValue->DataSize < cbModelNum ? CharacteristicValue->DataSize : cbModelNum);
                    else if (IsBthLEUuidMatch(pChars[i].CharacteristicUuid, uuidSystem))
                        memcpy (SystemId, CharacteristicValue->Data, CharacteristicValue->DataSize < cbSystemId ? CharacteristicValue->DataSize : cbSystemId);

                    delete[] CharacteristicValue;
                }
            }
        }
    }
    delete [] pChars;
    CloseHandle(hService);
}

CBtWin8Interface::~CBtWin8Interface()
{
    if (m_pEventHandle != INVALID_HANDLE_VALUE)
    {
        FP_BluetoothGATTUnregisterEvent pDereg = (FP_BluetoothGATTUnregisterEvent)GetProcAddress(m_hLib, "BluetoothGATTUnregisterEvent");
        if (pDereg)
            pDereg(m_pEventHandle, BLUETOOTH_GATT_FLAG_NONE);
    }
    if (m_hDevice != INVALID_HANDLE_VALUE)
        CloseHandle (m_hDevice);
}

