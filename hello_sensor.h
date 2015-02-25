/*
 * Copyright 2013, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/**
 * Modifications by Tyler Freeman, Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Modifications licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

#include "circular_buffer.h"
#include "config_mode.h"

/**
 * The current version of the drumpants BLE firmware.
 *
 * Should not be higher than 9 without fixing UUID_CHARACTERISTIC_FIRMWARE_REVISION_STRING
 */
#define DRUMPANTS_FIRMWARE_VERSION 0x01

// following definitions are shared between client and sensor
// to avoid unnecessary GATT Discovery
//
#define HANDLE_HELLO_SENSOR_SERVICE_UUID                    0x28
#define HANDLE_HELLO_SENSOR_VALUE_NOTIFY                    0x2a
#define HANDLE_HELLO_SENSOR_CLIENT_CONFIGURATION_DESCRIPTOR 0x2b
#define HANDLE_HELLO_SENSOR_CONFIGURATION                   0x2d

/**
 * Always set to the last handle + 1: the next available slot.
 * This is so so midi_over_ble.h can start there.
 */
#define HANDLE_HELLO_SENSOR_NEXT_AVAILABLE_ADDRESS 0x0064

// the maximum length of a characteristic notification packet to send over BLE connection.
// maximum supported is length of BLEPROFILE_DB_PDU.pdu: (LEATT_ATT_MTU-1)
// NOTE: if you change this, you must also add more 0x00s to the "CHARACTERISTIC_UUID128 (0x0029, HANDLE_HELLO_SENSOR_VALUE_NOTIFY," descriptor default value.
// 20 is the broadcom maximum (max 23-1, plus 2 for the something or other): http://community.broadcom.com/message/7243#7243 and http://community.broadcom.com/message/5175#5175
#define BLE_MAX_PACKET_LENGTH (MIN(20, LEATT_ATT_MTU-1))

/***
 * Maximum length of a MIDI over BLE packet.
 *
 * (defined in Apple doc, section 2.2): "The maximum MIDI payload for a Bluetooth Low Energy packet is the negotiated MTU minus 3 bytes"
 */
#define MAX_BLE_MIDI_PACKET_LEN (BLE_MAX_PACKET_LENGTH - 3)



/*****************************
 *
 *  CONNECTION LATENCY SETTINGS
 *
 *****************************/

// the minimum desired connection interval, in BLE frames (a multiple of 1.25ms).
// APPLE iOS minimum: 9 (11.25ms minimum for HID device (from Apple guidelines section 3.6))
// Android minimum: 6??? (6 is BLE spec minimum)
#define CONNECTION_INTERVAL_MINIMUM 8

// the maximum desired connection interval, in BLE frames (a multiple of 1.25ms).
// from testing, it seems that iOS 7 gives a interval of 39 when given min: 8, max: 24, slave latency 3. but guidlines require 9, 25, 3. so that's what we do. blarg.
// according to iOS guidelines, this must be at least 20ms more than CONNECTION_INTERVAL_MINIMUM
#define CONNECTION_INTERVAL_MAXIMUM 16

// requested number of frames the slave can ignore read requests from the master.
// Apple guidelines say this must be <= 4, but 3 seems to work better.
// bigger is better because we're focused on sending notifications; we don't care when the master wants to read us.
#define CONNECTION_INTERVAL_SLAVE_LATENCY 3

// connection supervision timeout value in 100ms units.
// Apple guidelines say it must be <= 6 seconds, but 5 seconds seems to work better.
// bigger is better because we don't want to disconnect if they aren't playing notes for a few seconds.
#define CONNECTION_INTERVAL_TIMEOUT 50

// when 1, cycles through different combinations of connection intervals and prints results.
// use to find the best combinations for that device, then go cry in a corner because they're all different.
// DISABLE FOR RELASE BUILDS!!!
#define ENABLE_CONNECTION_INTERVAL_TESTING 0






/*****************************
 *
 *  UUIDS
 *
 *****************************/

// Please note that all UUIDs need to be reversed when publishing in the database

// {1B7E8251-2877-41C3-B46E-CF057C562023}
// static const GUID UUID_HELLO_SERVICE               = { 0x1b7e8251, 0x2877, 0x41c3, { 0xb4, 0x6e, 0xcf, 0x5, 0x7c, 0x56, 0x20, 0x23 } };
#define UUID_HELLO_SERVICE                  0x25, 0x24, 0x56, 0x7c, 0x05, 0xcf, 0x6e, 0xb4, 0xc3, 0x41, 0x77, 0x28, 0x51, 0x82, 0x7e, 0x1b

// {5E9BF2A8-F93F-4481-A67E-3B2F4A07891A}
// static const GUID UUID_HELLO_CHARACTERISTIC_CONFIG = { 0x5e9bf2a8, 0xf93f, 0x4481, { 0xa6, 0x7e, 0x3b, 0x2f, 0x4a, 0x7, 0x89, 0x1a } };
#define UUID_HELLO_CHARACTERISTIC_CONFIG    0x1a, 0x89, 0x07, 0x4a, 0x2f, 0x3b, 0x7e, 0xa6, 0x81, 0x44, 0x3f, 0xf9, 0xa8, 0xf2, 0x9b, 0x5e

// {8AC32D3F-5CB9-4D44-BEC2-EE689169F626}
// static const GUID UUID_HELLO_CHARACTERISTIC_NOTIFY = { 0x8ac32d3f, 0x5cb9, 0x4d44, { 0xbe, 0xc2, 0xee, 0x68, 0x91, 0x69, 0xf6, 0x26 } };
#define UUID_HELLO_CHARACTERISTIC_NOTIFY    0x25, 0xf5, 0x69, 0x91, 0x68, 0xee, 0xc2, 0xbe, 0x44, 0x4d, 0xb9, 0x5c, 0x3f, 0x2d, 0xc3, 0x8a

const UINT8  AppGuid[16] = {UUID_HELLO_SERVICE};
const UINT16 VerMajor = DRUMPANTS_FIRMWARE_VERSION;
const UINT16 VerMinor = 1;

typedef UINT8 byte;



#endif
