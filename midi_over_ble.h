/***
 * Defines constants for Apple's MIDI over BLE spec.
 */

#ifndef MIDI_OVER_BLE_H_
#define MIDI_OVER_BLE_H_

#include "hello_sensor.h"


/*****************************
 *
 *  UUIDS
 *
 *****************************/

// Please note that all UUIDs need to be reversed when publishing in the database

/***
 * Handy Javascript trick to reverse the UUIDs in normal form to the code:

function convertUUIDToBytes(uuidStr) {
	var uuid = uuidStr.replace(/-/gi, ''),
		numChunks = 2,
		resultLength = Math.ceil(uuid.length / numChunks),
		result = [];

	for (var i = 0; i < resultLength; i++) {
		result[i] = uuid.substr(i * numChunks, numChunks);
	}

	return result;
}


function reverseUUIDString(uuid) {
	return convertUUIDToBytes(uuid).reverse();
}

var uuid = "03B80E5A-EDE8-4B33-A751-6CE34EC4C700",
	broadcomUUIDDefine = '0x' + reverseUUIDString(uuid).join(', 0x');

console.log(broadcomUUIDDefine);


 */

/***
 * From spec section 2.1.5 Apple Bluetooth Low Energy MIDI Service
 */

/***
 * 03B80E5A-EDE8-4B33-A751-6CE34EC4C700
 */
#define UUID_MIDI_SERVICE 0x00, 0xC7, 0xC4, 0x4E, 0xE3, 0x6C, 0x51, 0xA7, 0x33, 0x4B, 0xE8, 0xED, 0x5A, 0x0E, 0xB8, 0x03


/***
 *  7772E5DB-3868-4112-A1A9-F2669D106BF3
 */
#define UUID_MIDI_CHARACTERISTIC 0xF3, 0x6B, 0x10, 0x9D, 0x66, 0xF2, 0xA9, 0xA1, 0x12, 0x41, 0x68, 0x38, 0xDB, 0xE5, 0x72, 0x77


// following definitions are shared between client and sensor
// to avoid unnecessary GATT Discovery
//
#define HANDLE_MIDI_SERVICE_UUID                    0xff00
#define HANDLE_MIDI_TX_VALUE_NOTIFY      			(HANDLE_MIDI_SERVICE_UUID + 2)
#define HANDLE_MIDI_CLIENT_CONFIGURATION_DESCRIPTOR (HANDLE_MIDI_TX_VALUE_NOTIFY + 1)
#endif
