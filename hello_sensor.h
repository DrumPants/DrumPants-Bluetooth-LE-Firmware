/*
 * Copyright 2013, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
*
* BLE Vendor Specific Device
*
* This file provides definitions and function prototypes for Hello Sensor
* device
*
*/
#ifndef HELLO_SENSOR_H
#define HELLO_SENSOR_H

// following definitions are shared between client and sensor
// to avoid unnecessary GATT Discovery
//
#define HANDLE_HELLO_SENSOR_SERVICE_UUID                    0x28
#define HANDLE_HELLO_SENSOR_VALUE_NOTIFY                    0x2a
#define HANDLE_HELLO_SENSOR_CLIENT_CONFIGURATION_DESCRIPTOR 0x2b
#define HANDLE_HELLO_SENSOR_CONFIGURATION                   0x2d

// the maximum length of a characteristic notification packet to send over BLE connection.
// maximum supported is length of BLEPROFILE_DB_PDU.pdu: (LEATT_ATT_MTU-1)
#define BLE_MAX_PACKET_LENGTH 16

// Please note that all UUIDs need to be reversed when publishing in the database

// {1B7E8251-2877-41C3-B46E-CF057C562023}
// static const GUID UUID_HELLO_SERVICE               = { 0x1b7e8251, 0x2877, 0x41c3, { 0xb4, 0x6e, 0xcf, 0x5, 0x7c, 0x56, 0x20, 0x23 } };
#define UUID_HELLO_SERVICE                  0x23, 0x20, 0x56, 0x7c, 0x05, 0xcf, 0x6e, 0xb4, 0xc3, 0x41, 0x77, 0x28, 0x51, 0x82, 0x7e, 0x1b

// {5E9BF2A8-F93F-4481-A67E-3B2F4A07891A}
// static const GUID UUID_HELLO_CHARACTERISTIC_CONFIG = { 0x5e9bf2a8, 0xf93f, 0x4481, { 0xa6, 0x7e, 0x3b, 0x2f, 0x4a, 0x7, 0x89, 0x1a } };
#define UUID_HELLO_CHARACTERISTIC_CONFIG    0x1a, 0x89, 0x07, 0x4a, 0x2f, 0x3b, 0x7e, 0xa6, 0x81, 0x44, 0x3f, 0xf9, 0xa8, 0xf2, 0x9b, 0x5e

// {8AC32D3F-5CB9-4D44-BEC2-EE689169F626}
// static const GUID UUID_HELLO_CHARACTERISTIC_NOTIFY = { 0x8ac32d3f, 0x5cb9, 0x4d44, { 0xbe, 0xc2, 0xee, 0x68, 0x91, 0x69, 0xf6, 0x26 } };
#define UUID_HELLO_CHARACTERISTIC_NOTIFY    0x26, 0xf6, 0x69, 0x91, 0x68, 0xee, 0xc2, 0xbe, 0x44, 0x4d, 0xb9, 0x5c, 0x3f, 0x2d, 0xc3, 0x8a

const UINT8  AppGuid[16] = {UUID_HELLO_SERVICE};
const UINT16 VerMajor = 1;
const UINT16 VerMinor = 1;

#endif
