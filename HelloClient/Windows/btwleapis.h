/****************************************************************************
**
**  Name:       BtwLeApis.h
**  Function    this file contains the definitions for the BTW GATT APIs
**
**
**  Copyright (c) 2010-2011, Broadcom Corp., All Rights Reserved.
**  Proprietary and confidential.
**
******************************************************************************/
#ifndef _BTWLEAPIS_H
#define _BTWLEAPIS_H

#include <winnt.h>
#include <BluetoothApis.h>

#ifdef BTWLEAPI_EXPORTS
#define BTWLEAPI_API __declspec(dllexport)
#else
#define BTWLEAPI_API __declspec(dllimport)
#endif

/* Services */
#define BTW_GATT_UUID_SERVCLASS_GAP_SERVER               0x1800  
#define BTW_GATT_UUID_SERVCLASS_GATT_SERVER              0x1801  

#define BTW_GATT_UUID_SERVCLASS_LINK_LOSS                0x1803    /* Link Loss Service  */
#define BTW_GATT_UUID_SERVCLASS_IMMEDIATE_ALERT          0x1802    /* Immediate Alert Service  */
#define BTW_GATT_UUID_SERVCLASS_TX_POWER                 0x1804    /* Transmission Power Service  */

#define BTW_GATT_UUID_SERVCLASS_CURRENT_TIME             0x1805    /* Current Time Service  */
#define BTW_GATT_UUID_SERVCLASS_REFERENCE_TIME           0x1806    /* Reference Time Update Service  */
#define BTW_GATT_UUID_SERVCLASS_DST_CHANGE               0x1807    /* DST Change Service  */
#define BTW_GATT_UUID_SERVCLASS_BATTERY                  0x180F    /* Battery Service  */
#define BTW_GATT_UUID_SERVCLASS_NWA                      0x180B    /* Network Availability Service  */

#define BTW_GATT_UUID_SERVCLASS_HEALTH_THERMOMETER       0x1809    /* Health Thermometer Service  */
#define BTW_GATT_UUID_SERVCLASS_DEVICE_INFO              0x180A    /* Device Information Service  */
#define BTW_GATT_UUID_SERVCLASS_BLOOD_PRESSURE           0x1810    /* Blood Pressure Monitor Service  */
#define BTW_GATT_UUID_SERVCLASS_HEART_RATE               0x180D    /* Heart Rate Monitor Service  */
#define BTW_GATT_UUID_SERVCLASS_WEIGHT_SCALE             0xB000    /* Weight Scale Monitor Service  */  // TODO Change to spec value.
#define BTW_GATT_UUID_SERVCLASS_BLOOD_PRESSURE           0x1810	/* Blood Pressure Monitor Service  */

#define BTW_GATT_UUID_SERVCLASS_HID                      0x1812	/* LE HID Service  */                
#define BTW_GATT_UUID_SERVCLASS_SCAN_PARAMETERS          0x1813	/* LE Scan Parameters  */                


/* Characteristics  */

#define BTW_GATT_UUID_CHAR_ALERT_LEVEL                  0x2A06  /* Alert Level */
#define BTW_GATT_UUID_CHAR_TX_POWER_LEVEL               0x2A07  /* TX power level */

#define BTW_GATT_UUID_CHAR_CT_TIME                      0x2A2B    /* CT Time  */
#define BTW_GATT_UUID_CHAR_LOCAL_TIME_INFO              0x2A0F    /* Local Time Information  */
#define BTW_GATT_UUID_CHAR_REFERENCE_TIME_INFO          0x2A14    /* Reference Time Information*/

#define BTW_GATT_UUID_CHAR_TIME_WITH_DST                0x2A11    /* Time with DST  */

#define BTW_GATT_UUID_CHAR_TIME_UPDATE_CTRL_PT          0x2A16    /* Ref. Time Update Ctrl Point */
#define BTW_GATT_UUID_CHAR_TIME_UPDATE_STATE            0x2A17    /* Time Update State  */

#define BTW_GATT_UUID_CHAR_NEWORK_AVAILABILIY           0x2A18    /* Network Availability  */

#define BTW_GATT_UUID_CHAR_DIS_SYSTEM_ID                0x2A23
#define BTW_GATT_UUID_CHAR_DIS_MODEL_NUMBER             0x2A24
#define BTW_GATT_UUID_CHAR_DIS_SERIAL_NUMBER            0x2A25
#define BTW_GATT_UUID_CHAR_DIS_FIRMWARE_REVISION        0x2A26
#define BTW_GATT_UUID_CHAR_DIS_HARDWARE_REVISION        0x2A27
#define BTW_GATT_UUID_CHAR_DIS_SOFTWARE_REVISION        0x2A28
#define BTW_GATT_UUID_CHAR_DIS_MANUFACTURER_NAME        0x2A29

#define BTW_GATT_UUID_CHAR_HTS_TEMP_MEASUREMENT             0x2A1C
#define BTW_GATT_UUID_CHAR_HTS_TEMPERATURE_TYPE             0x2A1D
#define BTW_GATT_UUID_CHAR_HTS_INTERMEDIATE_TEMPERATURE     0x2A1E
#define BTW_GATT_UUID_CHAR_HTS_MEASUREMENT_INTERVAL         0x2A21
#define BTW_GATT_UUID_CHAR_HTS_VALID_RANGE                  0x2906    // Do we need this here?
#define BTW_GATT_UUID_CHAR_HTS_DATE_TIME                    0x2A08

#define BTW_GATT_UUID_CHAR_BPM_MEASUREMENT                  0x2A35
#define BTW_GATT_UUID_CHAR_BPM_INTER_BLOOD_PRESSURE         0x2A36

#define BTW_GATT_UUID_CHAR_HRM_MEASUREMENT                  0x2A37
#define BTW_GATT_UUID_CHAR_HRM_SENSOR_LOCATION              0x2A38
#define BTW_GATT_UUID_CHAR_HRM_COMMAND                      0x2A39

#define BTW_GATT_UUID_CHAR_BATTERY_LEVEL                    0x2A19
#define BTW_GATT_UUID_CHAR_BATTERY_STATE                    0x2A1A
#define BTW_GATT_UUID_CHAR_BATTERY_STATELEVEL               0x2A1B

#define BTW_GATT_UUID_CHAR_WEIGHT_MEASUREMENT               0xB001    // TODO Change to spec value.
#define BTW_GATT_UUID_CHAR_WEIGHT_INTERMEDIATE_MEASUREMENT  0xB002    // TODO Change to spec value.
#define BTW_GATT_UUID_CHAR_DIS_PNP_ID           		0x2A50
// LE HID
#define BTW_GATT_UUID_CHAR_BOOT_KEYBOARD_INPUT_REPORT   0x2A22
#define BTW_GATT_UUID_CHAR_BOOT_KEYBOARD_OUTPUT_REPORT  0x2A32
#define BTW_GATT_UUID_CHAR_BOOT_MOUSE_INPUT_REPORT      0x2A33

#define BTW_GATT_UUID_CHAR_HID_INFORMATION              0x2A4A
#define BTW_GATT_UUID_CHAR_HID_REPORT_MAP               0x2A4B
#define BTW_GATT_UUID_CHAR_HID_CTRL_PT                  0x2A4C
#define BTW_GATT_UUID_CHAR_HID_PROTOCOL_MODE            0x2A4E
#define BTW_GATT_UUID_CHAR_HID_REPORT                   0x2A4D

//LE Weight Scale


/* Attribute permissions
*/
#ifndef BTW_GATT_PERM_READ
#define BTW_GATT_PERM_READ              (1 << 0) /* bit 0 */   
#define BTW_GATT_PERM_READ_ENCRYPTED    (1 << 1) /* bit 1 */
#define BTW_GATT_PERM_READ_ENC_MITM     (1 << 2) /* bit 2 */
#define BTW_GATT_PERM_WRITE             (1 << 4) /* bit 4 */
#define BTW_GATT_PERM_WRITE_ENCRYPTED   (1 << 5) /* bit 5 */
#define BTW_GATT_PERM_WRITE_ENC_MITM    (1 << 6) /* bit 6 */
#define BTW_GATT_PERM_WRITE_SIGNED      (1 << 7) /* bit 7 */
#define BTW_GATT_PERM_WRITE_SIGNED_MITM (1 << 8) /* bit 8 */
#endif

/* Characteristic properties
*/
#ifndef BTW_GATT_CHAR_PROP_BIT_BROADCAST
#define BTW_GATT_CHAR_PROP_BIT_BROADCAST    (1 << 0)
#define BTW_GATT_CHAR_PROP_BIT_READ         (1 << 1)
#define BTW_GATT_CHAR_PROP_BIT_WRITE_NR     (1 << 2)
#define BTW_GATT_CHAR_PROP_BIT_WRITE        (1 << 3)
#define BTW_GATT_CHAR_PROP_BIT_NOTIFY       (1 << 4)
#define BTW_GATT_CHAR_PROP_BIT_INDICATE     (1 << 5)
#define BTW_GATT_CHAR_PROP_BIT_AUTH         (1 << 6)
#define BTW_GATT_CHAR_PROP_BIT_EXT_PROP     (1 << 7)
typedef BYTE tGATT_CHAR_PROP;
#endif

#ifndef BTW_GATT_AUTH_REQ_NONE
#define BTW_GATT_AUTH_REQ_NONE              0
#define BTW_GATT_AUTH_REQ_NO_MITM           1   /* unauthenticated encryption */
#define BTW_GATT_AUTH_REQ_MITM              2   /* authenticated encryption */
#define BTW_GATT_AUTH_REQ_SIGNED_NO_MITM    3
#define BTW_GATT_AUTH_REQ_SIGNED_MITM       4      
#endif

#define BTW_GATT_TRANSPORT_LE               0
#define BTW_GATT_TRANSPORT_BR_EDR           1
#define BTW_GATT_TRANSPORT_LE_BR_EDR        2

#define BTW_GATT_CLT_CONFIG_NONE                    0x0000
#define BTW_GATT_CLT_CONFIG_NOTIFICATION            0x0001
#define BTW_GATT_CLT_CONFIG_INDICATION              0x0002
typedef UINT16 tBTW_GATT_CLT_CHAR_CONFIG;

#define BTW_GATT_UUID_CHAR_CLIENT_CONFIG            0x2902      /*  Client Characteristic Configuration */
#define BTW_GATT_UUID_CHAR_EXTERNAL_REPORT_REF  	0x2907      /*  External Report Reference */
#define BTW_GATT_UUID_CHAR_REPORT_REFERENCE			0x2908      /*  Report Reference */

#ifdef __cplusplus
extern "C" {
#endif
typedef USHORT BTW_GATT_HANDLE;

typedef UCHAR BTW_GATT_STATUS;

typedef enum _BTW_GATT_OPTYPE 
{
    BTW_GATT_OPTYPE_NONE = 0x0,
    BTW_GATT_OPTYPE_READ,
    BTW_GATT_OPTYPE_WRITE,
    BTW_GATT_OPTYPE_NOTIFICATION,
    BTW_GATT_OPTYPE_INDICATION
} BTW_GATT_OPTYPE, * PBTW_GATT_OPTYPE;

//
// Define callback used to indicate connection up/down events
//
typedef void (*PFN_GATT_CONNECT_CALLBACK)(LPVOID pRefData, BLUETOOTH_ADDRESS *pDevice, BOOL bConnected);

/* Attribute Value structure 
*/
#ifndef GATT_MAX_ATTR_LEN 
#define GATT_MAX_ATTR_LEN     600 
#endif

typedef struct
{
    USHORT          len;        /* length of attribute value */
    BYTE            value[GATT_MAX_ATTR_LEN];  /* the actual attribute value */
} BTW_GATT_VALUE;

//
// Define callback used to indicate completion of read or write operations
//
typedef void (*PFN_GATT_COMPLETE_CALLBACK)(LPVOID pRefData, DWORD status, BTW_GATT_VALUE *p_data);

//
// Define callback used to indicate indication or notification received from the peer
// Callback function can return GATT_PENDING. In such case BtwGattRequestComplete should be called later.
// Callback function should return GATT_SUCCESS if request has been handled successfully.
// On error callback function should return proper error code GATT_....
//
typedef DWORD (*PFN_GATT_REQUEST_CALLBACK)
(
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
);


/*******************************************************************************
**
** Function         BtwGattRegister
**
** Description      This function is called to register application callback
**                  functions with GATT
**
** Parameter        
** pAddress: A pointer to a BLUETOOTH_DEVICE_INFO structure. GATT will indicate 
**     callbacks for this device.  Application can register to receive 
**     notification for all devices by passing NULL in the registration.
**
** pfnConnectCallback: callback when connection to device goes up or down.
** 
** pfnCompleteCallback: indicates completion of read/write operation
**
** pfnRequestCallback: indicates request from the client
**
** pRefData:        A void pointer which will be returned to the application
**      in the Connection Callback, if bWait is requested.  Bluetooth core will
**      not interpret or modify this value in any way.
**
** hReg: Output of the operation, application should call Deregister passing
**  this handle not to receive callbacks anymore.
**
** Returns.  ERROR_SUCCESS if operation succeeds
**
*******************************************************************************/
DWORD WINAPI BtwGattRegister 
(
    const BLUETOOTH_ADDRESS     *pAddress, 
    PFN_GATT_CONNECT_CALLBACK   pfnConnectCallback,
    PFN_GATT_COMPLETE_CALLBACK  pfnCompleteCallback,
    PFN_GATT_REQUEST_CALLBACK   pfnRequestCallback,
    PVOID                       pRefData,
    HANDLE                      *phReg
);
typedef DWORD (WINAPI *FP_BtwGattRegister)
(
    const BLUETOOTH_ADDRESS    *pAddress, 
    PFN_GATT_CONNECT_CALLBACK  pfnConnectCallback,
    PFN_GATT_COMPLETE_CALLBACK pfnCompleteCallback,
    PFN_GATT_REQUEST_CALLBACK  pfnRequestCallback,
    PVOID                      pRefData,
    HANDLE                     *phReg
);

/*******************************************************************************
**
** Function:        BtwGattDeregister
**
** Description:     This function is called to deregister with GATT
**
** Parameters:
** hReg:            Handle returned in BtwGattRegister
**
** Returns:         None.
**
*******************************************************************************/
void WINAPI BtwGattDeregister 
(
   HANDLE hReg
);
typedef void (WINAPI *FP_BtwGattDeregister)(HANDLE hReg);

/*******************************************************************************
**
** Function:        BtwGattConnect
**
** Description:     This function initiate a connecttion to a GATT server.  
**  Application does not need to call this function in normal case.  
**  Connections to all paired devices are established automatically.  Meanwhile 
**  calling function will increase amount of time Bluetooth radio spends 
**  scanning for advartisements.  If connection does not exist and if peer 
**  device is sending advertisments connection may be created faster.  Also 
**  this is the only way to connect to a device which does not send directed
**  but sends only undirected advertisments.
**
** Parameters:
** hReg:   Handle returned in BtwGattRegister
**
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. GATT will 
**      attempt to initiate connection for this device.  
**
** bWait            If True function will not return until connection is
**      established successfully or fails.
**
** Returns          ERROR_SUCCESS if connection established. WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattConnect
(
    HANDLE hReg,
    BLUETOOTH_ADDRESS *pAddress,
    BOOL bWait 
);
typedef DWORD (WINAPI *FP_BtwGattConnect)
(
    HANDLE hReg,
    BLUETOOTH_ADDRESS *pAddress,
    BOOL bWait 
);


/*******************************************************************************
**
** Function:        BtwGattReadCharacteristic
**
** Description:     This function can be used to read value of the characteristic
**      of the specific service of the specific Bluetooth device.  
**
** Parameters:
** hReg:            Handle returned in BtwGattRegister
**
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. Connection 
**      to the peer device should be established before application can attemt
**      a read
**      
** pGuidService:    GUID of the service which contains characteristic to read.
**
** dwServiceInstance: Some servers may sport multiple services of the same GUID.
**      To access different service instance can be used.
**
** pGuidChar:       GUID of the characteristic to read.
**
** dwCharInstance:    Some services may sport multiple characteristics of the 
**      same GUID. To access different characteristics instance can be used.
**
** security:        Required security for the operation.
**
** pValue:          Memory location where results will be stored.
**
** bWait:           If True call does not return until operation is completed
**      or timeout occurs
**
** pRefData:        A void pointer which will be returned to the application
**      in the Completion Callback, if bWait is requested.  Bluetooth core will
**      not interpret or modify this value in any way.
**
** Returns          ERROR_SUCCESS on success; WinError if read fails.
**
*******************************************************************************/
DWORD WINAPI BtwGattReadCharacteristic
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
/*******************************************************************************
**
** Function:        BtwGattReadDescriptor
**
** Description:     This function can be used to read value of the descriptor
**  of the characteristic of the specific service of the specific Bluetooth device.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. Connection 
**      to the peer device should be established before application can attemt
**      a read
**      
** pGuidService:    GUID of the service which contains descriptor of the 
**      characteristic to read.
**
** dwServiceInstance: Some servers may sport multiple services of the same GUID.
**      To access different service instance can be used.
**
** pGuidChar:       GUID of the characteristic to which descriptor belongs.
**
** dwCharInstance:    Some services may sport multiple characteristics of the 
**      same GUID. To access different characteristics instance can be used.
**
** pGuidDescr:      GUID of the descriptor to read.
**
** security:        Required security for the operation.
**
** pValue:          Memory location where results will be stored.
**
** bWait:           If True call does not return until operation is completed
**      or timeout occurs
**
** pRefData:        A void pointer which will be returned to the application
**      in the Completion Callback, if bWait is requested.  Bluetooth core will
**      not interpret or modify this value in any way.
**
** Returns          ERROR_SUCCESS on success; WinError if read fails.
**
*******************************************************************************/
DWORD WINAPI BtwGattReadDescriptor
(
    HANDLE                      hReg,               // handle of the application registration
    BLUETOOTH_ADDRESS           *pAddress,          // address of the device to read
    const GUID                  *pGuidService,      // guid of the service
    DWORD                       dwServiceInstance,  // instance of the service (most likely 0)
    const GUID                  *pGuidChar,         // guid of the char
    DWORD                       dwCharInstance,     // instance of the char (most likely 0)
    const GUID                  *pGuidDescr,        // guid of the descriptor 
    DWORD                       security,           // required security
    BTW_GATT_VALUE              *pValue,            // pointer to the result, can be NULL if completion is used
    BOOL                        bWait,              // if False returns right away
    LPVOID                      pRefData            // pointer returned in the completion
);

typedef DWORD (WINAPI *FP_BtwGattReadDescriptor)
(
    HANDLE                      hReg,               // handle of the application registration
    BLUETOOTH_ADDRESS           *pAddress,          // address of the device to read
    const GUID                  *pGuidService,      // guid of the service
    DWORD                       dwServiceInstance,  // instance of the service (most likely 0)
    const GUID                  *pGuidChar,         // guid of the char
    DWORD                       dwCharInstance,     // instance of the char (most likely 0)
    const GUID                  *pGuidDescr,        // guid of the descriptor 
    DWORD                       security,           // required security
    BTW_GATT_VALUE              *pValue,            // pointer to the result, can be NULL if completion is used
    BOOL                        bWait,              // if False returns right away
    LPVOID                      pRefData            // pointer returned in the completion
);

/*******************************************************************************
**
** Function:        BtwGattWriteCharacteristic
**
** Description:     This function can be used to write value of the characteristic
**  of the specific service of the specific Bluetooth device.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. Connection 
**  to the peer device should be established before application can attemt
**  a write the value.
**      
** pGuidService:    GUID of the service which contains descriptor of the 
**  characteristic to write.
**
** dwServiceInstance: Some servers may sport multiple services of the same GUID.
**  To access different service instance can be used.
**
** pGuidChar:       GUID of the characteristic to which descriptor belongs.
**
** dwCharInstance:    Some services may sport multiple characteristics of the 
**  same GUID. To access different characteristics instance can be used.
**
** security:        Required security for the operation.
**
** pValue:          Value of the characteristic to write.
**
** bWait:           If True call does not return until operation is completed
**  or timeout occurs
**
** pRefData:        A void pointer which will be returned to the application
**      in the Completion Callback, if bWait is requested.  Bluetooth core will
**      not interpret or modify this value in any way.
**
** Returns          ERROR_SUCCESS on succes; WinError if write fails.
**
*******************************************************************************/
DWORD WINAPI BtwGattWriteCharacteristic
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


/*******************************************************************************
**
** Function:        BtwGattWriteDescriptor
**
** Description:     This function can be used to write value to the 
**  descriptor of the characteristic of the specific service of the specific 
**  Bluetooth device.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. Connection 
**  to the peer device should be established before application can attemt
**  a write the value.
**      
** pGuidService:    GUID of the service which contains characteristic to write.
**
** dwServiceInstance: Some servers may sport multiple services of the same GUID.
**                  To access different service instance can be used.
**
** pGuidChar:       GUID of the characteristic to write.
**
** dwCharInstance:    Some services may sport multiple characteristics of the 
**  same GUID. To access different characteristics instance can be used.
**
** pGuidDescr:      GUID of the descriptor to write.
**
** security:        Required security for the operation.
**
** pValue:          Value of the characteristic to write.
**
** Returns          ERROR_SUCCESS on succes; WinError if write fails.
**
*******************************************************************************/
DWORD WINAPI BtwGattWriteDescriptor
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

/*******************************************************************************
**
** Function:        BtwGattEnumService
**
** Description:     This function returns parameters of the service.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure with address
**  of the peer device. 
**
** dwIndex          Index of the service to return.
**
** pGuidService     Pointer to the GUID where information will be returned
**
** Returns          Handle in hService that can be used to enumerate includes services,
**  characteristics, or descriptors of the characteristics.
**                  ERROR_SUCCESS on succes; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattEnumService
(
    BLUETOOTH_ADDRESS *pAddress,
    DWORD   dwIndex,
    GUID    *pGuidService,
    BTW_GATT_HANDLE   *hService
);

typedef DWORD (WINAPI *FP_BtwGattEnumService)
(
    BLUETOOTH_ADDRESS *pAddress,
    DWORD   dwIndex,
    GUID    *pGuidService,
    BTW_GATT_HANDLE   *hService
);

/*******************************************************************************
**
** Function:        BtwGattEnumIncludeService
*******************************************************************************/
DWORD WINAPI BtwGattEnumIncludeService
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE  hService, 
    DWORD   dwIndex, 
    GUID    *pGuidIncludeService,
    BTW_GATT_HANDLE *phIncludeService
);

typedef DWORD (WINAPI *FP_BtwGattEnumIncludeService)
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE  hService, 
    DWORD   dwIndex, 
    GUID    *pGuidIncludeService,
    BTW_GATT_HANDLE *phIncludeService
);

/*******************************************************************************
**
** Function:        BtwGattEnumCharacteristic
**
** Description:     This function returns parameters of the characteristic.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure with address
**  of the peer device. 
**
** hService         Handle of the service which contain this characteristic.
**
** dwIndex          Index of the characteristic to return.
**
** pGuidChar        Pointer to the GUID where information will be returned
**
** Returns          Handle in hChar that can be used to enumerate descriptors of 
**  the characteristics.
**                  ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattEnumCharacteristic
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hService, 
    DWORD   dwIndex, 
    GUID    *pGuidChar,
    BTW_GATT_HANDLE     *hChar
);

typedef DWORD (WINAPI *FP_BtwGattEnumCharacteristic)
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hService, 
    DWORD   dwIndex, 
    GUID    *pGuidChar,
    BTW_GATT_HANDLE     *hChar
);

/*******************************************************************************
**
** Function:        BtwGattEnumDescriptor
**
** Description:     This function returns parameters of the descriptor.  
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure with address
**  of the peer device. 
**
** hService         Handle of the service which contain the characteristic of 
**  the descriptor.
**
** hChar            Handle of the characteristic to which the descriptor belong
**
** dwIndex          Index of the characteristic to return.
**
** pGuidDescr       Pointer to the GUID where information will be returned
**
** Returns          Handle in hDescr that can be used to enumerate included 
**  characteristics, or descriptors of the characteristics.
**                  ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattEnumDescriptor
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hService, 
    BTW_GATT_HANDLE     hChar, 
    DWORD   dwIndex, 
    GUID    *pGuidDescr,
    BTW_GATT_HANDLE     *hDescr
);

typedef DWORD (WINAPI *FP_BtwGattEnumDescriptor)
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hService, 
    BTW_GATT_HANDLE     hChar, 
    DWORD   dwIndex, 
    GUID    *pGuidDescr,
    BTW_GATT_HANDLE     *hDescr
);

/*******************************************************************************
**
** Function:        BtwGattCreateService
**
** Description:     This function creates service in GATT database.  The 
**  procedure for creating service should include BtwGattCreateService 
**  followed by sequence of BtwGattAddIncludeService for each service to be 
**  included in the service definitions.  If service contains one or more
**  characteristics BtwGattAddCharacteristic will follow.  If added 
**  characteristics need descriptors BtwGattAddDescriptor should be called
**  before the calle to next BtwGattAddCharacteristic.  The service will not
**  be published for external devices to discover until application calls
**  BtwGattStartService.  The full procedure should look
**  like
**      BtwGattCreateService
**      BtwGattAddIncludeService
**      ...
**      BtwGattAddIncludeService
**      BtwGattAddCharacteristic
**      BtwGattAddDescriptor
**      ...
**      BtwGattAddDescriptor
**      ...
**      BtwGattAddCharacteristic
**      BtwGattAddDescriptor
**      ...
**      BtwGattAddDescriptor
**      BtwGattStartService
**
** Parameters:
** hReg:            Handle returned in BtwGattRegister
**
** pGuidService:    Pointer to service GUID.  GUID + Instance should be unique.
**
** dwServiceInstance: Service instance. GUID + Instance should be unique.  This 
**  should be used if more than one service with the same GUID has to be 
**  instantiated, otherwise 0.
**
** numHandles       Number of handles needed to reserve for the service.  
**  The good estimation is number of included services + (2 * number of 
**  characteristics) + number of descriptors + 10;
**
** bPrimary         True is primary service is being created
**
** Returns          Service handle in hService that can be used to add characteristics 
**  and descriptors.  0 indicates failure of the operation.
**                  ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattCreateService
(
    HANDLE hReg, 
    GUID   *pGuidService,
    DWORD  dwNumIncludedServices,
    DWORD  dwNumChar,
    DWORD  dwNumDesc,
    BOOL   bPrimary,
    BTW_GATT_HANDLE *hService
);

typedef DWORD (WINAPI *FP_BtwGattCreateService)
(
    HANDLE hReg, 
    GUID   *pGuidService,
    DWORD  dwNumIncludedServices,
    DWORD  dwNumChar,
    DWORD  dwNumDesc,
    BOOL   bPrimary,
    BTW_GATT_HANDLE *hService
);

/*******************************************************************************
**
** Function:        BtwGattAddIncludeService
**
** Description:     This function is called to add an included service.
**
** Parameters:
** hService:        Handle of the service to which this included service is 
**  added to.
**
** hIncludeService: Included service handle
**
** Returns          ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattAddIncludeService 
(
    BTW_GATT_HANDLE hService, 
    BTW_GATT_HANDLE hIncludeService
);


typedef DWORD (WINAPI *FP_BtwGattAddIncludeService)
(
    BTW_GATT_HANDLE hService, 
    BTW_GATT_HANDLE hIncludeService
);

/*******************************************************************************
**
** Function:        BtwGattAddCharacteristic
**
** Description:     This function can be called to add a characteristic into 
**  a service.  It will add a characteristic declaration and characteristic
**  value declaration into the service database identified by the service handle
**
** Parameters:
** hService:        Handle of the service to which characteristic is 
**  added to.
**
** pGuidChar:       Pointer to characteristic GUID 
**
** Peermissions:    Characteristic value declaration attribute permission.
**
** Properties:      Characteristic Properties
**
** Returns          Characteristic value declaration attribute handle in hChar.
**                  ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattAddCharacteristic 
(
    BTW_GATT_HANDLE hService, 
    GUID   *pGuidChar,
    DWORD  Permissions,
    DWORD  Properties,
    BTW_GATT_HANDLE *hChar
);

typedef DWORD (WINAPI *FP_BtwGattAddCharacteristic)
(
    BTW_GATT_HANDLE hService, 
    GUID   *pGuidChar,
    DWORD  Permissions,
    DWORD  Properties,
    BTW_GATT_HANDLE *hChar
);

/*******************************************************************************
**
** Function:        BtwGattAddDescriptor
**
** Description:     This function can be called to add a characteristic 
**  descriptor into service database.  The call to this function should follow
**  call to BtwGattAddCharacteristic which define characteristic of this 
**  descriptor.  And it should proceed call to next BtwGattAddCharacteristic
**  for the next characteristic.
**
** Parameters:
** hService:        Handle of the service to which descriptor is 
**  added to.
**
** pGuidDescr:      Pointer to characteristic GUID 
**
** Peermissions:    Descriptor declaration attribute permission.
**
** Value:           Descriptor Value
**
** Returns          Characteristic descriptor attribute handle in hDescr.
**                  ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattAddDescriptor 
(
    BTW_GATT_HANDLE hService, 
    GUID   *pGuidDescr,
    DWORD  Permissions,
    BTW_GATT_HANDLE *hDescr
);

typedef DWORD (WINAPI *FP_BtwGattAddDescriptor)
(
    BTW_GATT_HANDLE hService, 
    GUID   *pGuidDescr,
    DWORD  Permissions,
    BTW_GATT_HANDLE *hDescr
);

/*******************************************************************************
**
** Function:        BtwGattStartService
**
** Description:     This function starts service
**
** Parameters:
** hService:        Handle returned in BtwGattCreateService
**
** Returns          ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattStartService
(
    BTW_GATT_HANDLE hService
);

typedef DWORD (WINAPI *FP_BtwGattStartService)
(
    BTW_GATT_HANDLE hService
);

/*******************************************************************************
**
** Function:        BtwGattStopService
**
** Description:     This function stops currently running LE service
**
** Parameters:
** hService:        Handle returned in BtwGattCreateService
**
** Returns          ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattStopService
(
    BTW_GATT_HANDLE hService
);

typedef DWORD (WINAPI *FP_BtwGattStopService)
(
    BTW_GATT_HANDLE hService
);

/*******************************************************************************
**
** Function:        BtwGattDeleteService
**
** Description:     This function remove service from GATT database
**
** Parameters:
** hService:        Handle returned in BtwGattCreateService
**
** Returns          ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattDeleteService
(
    BTW_GATT_HANDLE hService
);

typedef DWORD (WINAPI *FP_BtwGattDeleteService)
(
    BTW_GATT_HANDLE hService
);

/*******************************************************************************
**
** Function:        BtwGattCharValueChanged
**
** Description:     This function instructs BLE core to send indication or
**  notification to the connected client.
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_DEVICE_INFO structure. Connection 
**  to the peer device should be established before application can send
**  notification or indication
**
** hChar:           Handle of the characteristic as returned by 
**  AddCharacteristic function for which client registered to receive
**  notification or indication
**
** IsIndication     True if indication needs to be sent, false if notification.
**
** pValue:          Value of the characteristic to indicate or notify.
**
** Returns          ERROR_SUCCESS on success; WinError on error.
**
*******************************************************************************/
DWORD WINAPI BtwGattCharValueChanged
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hChar, 
    BOOL    IsIndication,
    BTW_GATT_VALUE  *pValue
);

typedef DWORD (WINAPI *FP_BtwGattCharValueChanged)
(
    BLUETOOTH_ADDRESS *pAddress, 
    BTW_GATT_HANDLE     hChar, 
    BOOL    IsIndication,
    BTW_GATT_VALUE  *pValue
);

/*******************************************************************************
**
** Function:        BtwGattRequestComplete
**
** Description:     This function should be called by application if previous request callback
**                  returned status GATT_PENDING
**
** Parameters:
** pAddress:        A pointer to a BLUETOOTH_ADDRESS structure.**
** status:          Status of the request - can be any GATT_... code
** pValue:          Value to be sent in response to the pending request
**
** Returns          ERROR_SUCCESS on success; WinError if write fails.
**
*******************************************************************************/
DWORD WINAPI BtwGattRequestComplete
(
    BLUETOOTH_ADDRESS *pAddress,
    DWORD status,
    BTW_GATT_VALUE  *pValue
);

typedef DWORD (WINAPI *FP_BtwGattRequestComplete)
(
    BLUETOOTH_ADDRESS *pAddress,
    DWORD status,
    BTW_GATT_VALUE  *pValue
);

#ifdef __cplusplus
}
#endif

#endif