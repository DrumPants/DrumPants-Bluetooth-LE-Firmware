
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <bthdef.h>
#include <BluetoothAPIs.h>
#include "BTWLeApis.h"

// === Following should be from <BluetoothLeAPIs.h>

extern "C" {
static const GUID bt_guid = {0, 0, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

#define ERROR_MORE_DATA_PRESENT 0x800700ea

////////////////////////////////////////////////////////////////////////////////
// GATT Security and Other Flag-related Facilities
////////////////////////////////////////////////////////////////////////////////
#define BLUETOOTH_GATT_FLAG_NONE                        0x00000000
#define BLUETOOTH_GATT_FLAG_CONNECTION_ENCRYPTED        0x00000001
#define BLUETOOTH_GATT_FLAG_CONNECTION_AUTHENTICATED    0x00000002
#define BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_DEVICE      0x00000004
#define BLUETOOTH_GATT_FLAG_FORCE_READ_FROM_CACHE       0x00000008
#define BLUETOOTH_GATT_FLAG_SIGNED_WRITE                0x00000010
#define BLUETOOTH_GATT_FLAG_WRITE_WITHOUT_RESPONSE      0x00000020
#define BLUETOOTH_GATT_FLAG_RETURN_ALL                  0x00000040

////////////////////////////////////////////////////////////////////////////////
// GATT Structures
////////////////////////////////////////////////////////////////////////////////
typedef struct _BTH_LE_UUID {
    BOOLEAN IsShortUuid;
    
#ifdef MIDL_PASS
    [switch_type(BOOLEAN), switch_is((BOOLEAN)IsShortUuid)]
#endif
    union {
    
#ifdef MIDL_PASS
        [case(TRUE)]
#endif
        USHORT ShortUuid;
        
#ifdef MIDL_PASS
        [case(FALSE)]
#endif
        GUID LongUuid;
    } Value;
} BTH_LE_UUID, *PBTH_LE_UUID;

typedef struct _BTH_LE_GATT_SERVICE {
    BTH_LE_UUID ServiceUuid;
    USHORT AttributeHandle;
} BTH_LE_GATT_SERVICE, *PBTH_LE_GATT_SERVICE;

typedef enum _BTH_LE_GATT_DESCRIPTOR_TYPE {
    CharacteristicExtendedProperties,
    CharacteristicUserDescription,
    ClientCharacteristicConfiguration,
    ServerCharacteristicConfiguration,
    CharacteristicFormat,
    CharacteristicAggregateFormat,
    CustomDescriptor
} BTH_LE_GATT_DESCRIPTOR_TYPE, *PBTH_LE_GATT_DESCRIPTOR_TYPE;

typedef struct _BTH_LE_GATT_CHARACTERISTIC {
    USHORT ServiceHandle;
    BTH_LE_UUID CharacteristicUuid;
    USHORT AttributeHandle;
    USHORT CharacteristicValueHandle;
    BOOLEAN IsBroadcastable;
    BOOLEAN IsReadable;
    BOOLEAN IsWritable;
    BOOLEAN IsWritableWithoutResponse;
    BOOLEAN IsSignedWritable;
    BOOLEAN IsNotifiable;
    BOOLEAN IsIndicatable;
    BOOLEAN HasExtendedProperties;
} BTH_LE_GATT_CHARACTERISTIC, *PBTH_LE_GATT_CHARACTERISTIC;

typedef struct _BTH_LE_GATT_CHARACTERISTIC_VALUE {
    ULONG DataSize;

#ifdef MIDL_PASS
    [size_is(DataSize)] UCHAR Data[*];
#else
   UCHAR Data[1];
#endif
} BTH_LE_GATT_CHARACTERISTIC_VALUE, *PBTH_LE_GATT_CHARACTERISTIC_VALUE;

typedef struct _BTH_LE_GATT_DESCRIPTOR {
    USHORT ServiceHandle;
    USHORT CharacteristicHandle;
    BTH_LE_GATT_DESCRIPTOR_TYPE DescriptorType;
    BTH_LE_UUID DescriptorUuid;
    USHORT AttributeHandle;
} BTH_LE_GATT_DESCRIPTOR, *PBTH_LE_GATT_DESCRIPTOR;

typedef struct _BTH_LE_GATT_DESCRIPTOR_VALUE {
    BTH_LE_GATT_DESCRIPTOR_TYPE DescriptorType;
    BTH_LE_UUID DescriptorUuid;

#ifdef MIDL_PASS
    [switch_type(BTH_LE_GATT_DESCRIPTOR_TYPE), 
            switch_is((BTH_LE_GATT_DESCRIPTOR_TYPE)DescriptorType)]
#endif
    union {
    
#ifdef MIDL_PASS
        [case(CharacteristicExtendedProperties)]
#endif
        struct {
            BOOLEAN IsReliableWriteEnabled;
            BOOLEAN IsAuxiliariesWritable;
        } CharacteristicExtendedProperties;

#ifdef MIDL_PASS
        [case(ClientCharacteristicConfiguration)]
#endif
        struct {
            BOOLEAN IsSubscribeToNotification;
            BOOLEAN IsSubscribeToIndication;
        } ClientCharacteristicConfiguration;

#ifdef MIDL_PASS
        [case(ServerCharacteristicConfiguration)]
#endif
        struct {
            BOOLEAN IsBroadcast;
        } ServerCharacteristicConfiguration;

#ifdef MIDL_PASS
        [case(CharacteristicFormat)]
#endif
        struct {
            UCHAR Format; 
            UCHAR Exponent;
            BTH_LE_UUID Unit;
            UCHAR NameSpace;
            BTH_LE_UUID Description;
        } CharacteristicFormat;
        
#ifdef MIDL_PASS
        [default]
            ;
#endif
    };
    
    ULONG DataSize;

#ifdef MIDL_PASS
    [size_is(DataSize)] UCHAR Data[*];
#else
   UCHAR Data[1];
#endif
} BTH_LE_GATT_DESCRIPTOR_VALUE, *PBTH_LE_GATT_DESCRIPTOR_VALUE;

typedef enum _BTH_LE_GATT_EVENT_TYPE {
    CharacteristicValueChangedEvent,

} BTH_LE_GATT_EVENT_TYPE;


typedef VOID (CALLBACK *PFNBLUETOOTH_GATT_EVENT_CALLBACK)(
    _In_ BTH_LE_GATT_EVENT_TYPE EventType,
    _In_ PVOID EventOutParameter,
    _In_opt_ PVOID Context
);

typedef struct _BLUETOOTH_GATT_VALUE_CHANGED_EVENT {
    USHORT ChangedAttributeHandle;
    size_t CharacteristicValueDataSize;
    PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue;
} BLUETOOTH_GATT_VALUE_CHANGED_EVENT, *PBLUETOOTH_GATT_VALUE_CHANGED_EVENT;


typedef ULONG64 BTH_LE_GATT_RELIABLE_WRITE_CONTEXT, *PBTH_LE_GATT_RELIABLE_WRITE_CONTEXT;

HRESULT WINAPI
BluetoothGATTGetServices(
    _In_ HANDLE hDevice,
    _In_ USHORT ServicesBufferCount,
    _Out_ PBTH_LE_GATT_SERVICE ServicesBuffer,
    _Out_ USHORT* ServicesBufferActual,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTGetServices)(
    _In_ HANDLE hDevice,
    _In_ USHORT ServicesBufferCount,
    _Out_ PBTH_LE_GATT_SERVICE ServicesBuffer,
    _Out_ USHORT* ServicesBufferActual,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTGetCharacteristicValue(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ ULONG CharacteristicValueDataSize,
    _Out_opt_ PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue,
    _Out_opt_ USHORT* CharacteristicValueSizeRequired,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTGetDescriptorValue(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_DESCRIPTOR Descriptor,
    _In_ ULONG DescriptorValueDataSize,
    _Out_opt_ PBTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue,
    _Out_opt_ USHORT* DescriptorValueSizeRequired,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTGetDescriptorValue)(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_DESCRIPTOR Descriptor,
    _In_ ULONG DescriptorValueDataSize,
    _Out_opt_ PBTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue,
    _Out_opt_ USHORT* DescriptorValueSizeRequired,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTGetCharacteristicValue(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ ULONG CharacteristicValueDataSize,
    _Out_opt_ PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue,
    _Out_opt_ USHORT* CharacteristicValueSizeRequired,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTGetCharacteristicValue)(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ ULONG CharacteristicValueDataSize,
    _Out_opt_ PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue,
    _Out_opt_ USHORT* CharacteristicValueSizeRequired,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTSetCharacteristicValue(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue,
    _In_opt_ BTH_LE_GATT_RELIABLE_WRITE_CONTEXT ReliableWriteContext,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTSetCharacteristicValue)(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ PBTH_LE_GATT_CHARACTERISTIC_VALUE CharacteristicValue,
    _In_opt_ BTH_LE_GATT_RELIABLE_WRITE_CONTEXT ReliableWriteContext,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTGetCharacteristics(
    _In_ HANDLE hDevice,
    _In_opt_ PBTH_LE_GATT_SERVICE Service,
    _In_ USHORT CharacteristicsBufferCount,
    _Out_ PBTH_LE_GATT_CHARACTERISTIC CharacteristicsBuffer,
    _Out_ USHORT* CharacteristicsBufferActual,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTGetCharacteristics)(
    _In_ HANDLE hDevice,
    _In_opt_ PBTH_LE_GATT_SERVICE Service,
    _In_ USHORT CharacteristicsBufferCount,
    _Out_ PBTH_LE_GATT_CHARACTERISTIC CharacteristicsBuffer,
    _Out_ USHORT* CharacteristicsBufferActual,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTGetDescriptors(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ USHORT DescriptorsBufferCount,
    _Out_ PBTH_LE_GATT_DESCRIPTOR DescriptorsBuffer,
    _Out_ USHORT* DescriptorsBufferActual,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTGetDescriptors)(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_CHARACTERISTIC Characteristic,
    _In_ USHORT DescriptorsBufferCount,
    _Out_ PBTH_LE_GATT_DESCRIPTOR DescriptorsBuffer,
    _Out_ USHORT* DescriptorsBufferActual,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTSetDescriptorValue(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_DESCRIPTOR Descriptor,
    _In_ PBTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTSetDescriptorValue)(
    _In_ HANDLE hDevice,
    _In_ PBTH_LE_GATT_DESCRIPTOR Descriptor,
    _In_ PBTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue,
    _In_ ULONG Flags
    );

BOOLEAN
IsBthLEUuidMatch(
    _In_ BTH_LE_UUID uuid1,
    _In_ BTH_LE_UUID uuid2
    );

//
// Structure associated with a CharacteristicValueChanged Event
//
typedef struct _BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION {
    USHORT NumCharacteristics;
    BTH_LE_GATT_CHARACTERISTIC Characteristics[1];
} BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION, *PBLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION;

//
// Callback function signature for Bluetooth GATT events.
//
typedef VOID (CALLBACK *PFNBLUETOOTH_GATT_EVENT_CALLBACK)(
    _In_ BTH_LE_GATT_EVENT_TYPE EventType,
    _In_ PVOID EventOutParameter,
    _In_opt_ PVOID Context
);

typedef HANDLE BLUETOOTH_GATT_EVENT_HANDLE;

HRESULT WINAPI
BluetoothGATTRegisterEvent(
    _In_ HANDLE hService,
    _In_ BTH_LE_GATT_EVENT_TYPE EventType,
    _In_ PVOID EventParameterIn,
    _In_ PFNBLUETOOTH_GATT_EVENT_CALLBACK Callback,
    _In_opt_ PVOID CallbackContext,
    _Out_ BLUETOOTH_GATT_EVENT_HANDLE * pEventHandle,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTRegisterEvent)(
    _In_ HANDLE hService,
    _In_ BTH_LE_GATT_EVENT_TYPE EventType,
    _In_ PVOID EventParameterIn,
    _In_ PFNBLUETOOTH_GATT_EVENT_CALLBACK Callback,
    _In_opt_ PVOID CallbackContext,
    _Out_ BLUETOOTH_GATT_EVENT_HANDLE * pEventHandle,
    _In_ ULONG Flags
    );

HRESULT WINAPI
BluetoothGATTUnregisterEvent(
    _In_ BLUETOOTH_GATT_EVENT_HANDLE EventHandle,
    _In_ ULONG Flags
    );
typedef HRESULT (WINAPI *FP_BluetoothGATTUnregisterEvent)(
    _In_ BLUETOOTH_GATT_EVENT_HANDLE EventHandle,
    _In_ ULONG Flags
    );

HRESULT WINAPI BluetoothGATTCloseSession(
  _In_  HANDLE hDevice,
  _In_  ULONG Flags
);

typedef HRESULT (WINAPI *FP_BluetoothGATTCloseSession)(
  _In_  HANDLE hDevice,
  _In_  ULONG Flags
);


FORCEINLINE
BOOLEAN
IsBthLEUuidMatch(
    _In_ BTH_LE_UUID uuid1,
    _In_ BTH_LE_UUID uuid2
    )
{
    BTH_LE_UUID tempLongUuid = {0};
    
    tempLongUuid.IsShortUuid = FALSE;
    tempLongUuid.Value.LongUuid = bt_guid;

    if (uuid1.IsShortUuid && uuid2.IsShortUuid) {
        return (uuid1.Value.ShortUuid == uuid2.Value.ShortUuid);
    } else if (!uuid1.IsShortUuid && !uuid2.IsShortUuid) {
        return (0 == memcmp(&uuid1.Value.LongUuid, &uuid2.Value.LongUuid, sizeof(GUID)));
    } else if (uuid1.IsShortUuid) {
        tempLongUuid.Value.LongUuid.Data1 += uuid1.Value.ShortUuid;
        return (0 == memcmp(&tempLongUuid, &uuid2.Value.LongUuid, sizeof(GUID)));
    } else if (uuid2.IsShortUuid) {
        tempLongUuid.Value.LongUuid.Data1 += uuid2.Value.ShortUuid;
        return (0 == memcmp(&uuid1.Value.LongUuid, &tempLongUuid.Value.LongUuid, sizeof(GUID)));
    }
    
    return FALSE;
}
}


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


