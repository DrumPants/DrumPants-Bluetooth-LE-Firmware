
#ifndef TEST_SHIM
#define TEST_SHIM

#include <stdio.h>

#define ENABLE_TEST_DEBUG


typedef unsigned char UINT8;
typedef unsigned char BOOL;

// test shim so we don't need to include BLE libs
// GATT_PDU
typedef struct
{
  UINT8 len;
  UINT8 header;
  UINT8 pdu[21-1];
} BLEPROFILE_DB_PDU;


#define ble_trace1(...) printf(__VA_ARGS__)

// END TEST_SHIM
#endif
