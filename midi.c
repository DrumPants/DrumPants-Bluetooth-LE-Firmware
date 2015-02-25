/*
 * midi.c
 *
 *  Created on: Feb 23, 2015
 *      Author: tyler
 */

#include "midi.h"
#include "hello_sensor.h"

#define IS_MIDI_STATUS(b) ((b & 0x80) != 0)
#define MIDI_PACKET_LEN 3

#define MAX_BLE_MIDI_PACKET_LEN (BLE_MAX_PACKET_LENGTH - 3)

#define MAX_TIMESTAMP_VALUE 8192

#define TRUE 1
#define FALSE 0

int midiTimestamp = 0;
BOOL isNewPacket = TRUE;

byte curPacketEndLen = 0;

/***
 * Tracks the length of the current MIDI packet.
 */
UINT8 curMidiEventLen = 0;

UINT8 currentRunningStatus = 0;

/***
 * Table 2-3 Bluetooth Low Energy MIDI Packet Header Byte
￼●
●
Bit
Name
Notes
7
Start Bit
Shall be set to 1
6
Reserved
Shall be set to 0
5
Timestamp[12]
Bit 12 of MIDI timestamp information
4
Timestamp[11]
Bit 11 of MIDI timestamp information
3
Timestamp[10]
Bit 10 of MIDI timestamp information
2
Timestamp[9]
Bit 9 of MIDI timestamp information
1
Timestamp[8]
Bit 8 of MIDI timestamp information
0
Timestamp[7]
Bit 7 of MIDI timestamp information
 */
UINT8 writeHeader() {
	UINT8 byte;
	byte = (1 << 7) | ((midiTimestamp >> 6) & 0x3f);

	CBUF_Push(midiBuffer, byte);

	curPacketLen = 0;
	isNewPacket = FALSE;
	currentRunningStatus = 0;
}

// add timestamp first
UINT8 writeTimestamp() {
	UINT8 byte;
	byte = (1 << 7) | (midiTimestamp & 0x7f);

	CBUF_Push(midiBuffer, byte);
}


void inline markEndOfBLEPacket() {
	curPacketEndLen = CBUF_Len(midiBuffer);
}

UINT8 saveMIDIDataToBuffer(char midiByte) {

	if (IS_MIDI_STATUS(midiByte)) {
		// we got a new status, which means the previous midi message is complete
		// and can be sent if necessary.
		markEndOfBLEPacket();

		if (isNewPacket) {
			writeHeader();
		}

		// if there's no more room in this packet for the full message,
		// start the next one first and add this to that one.
BLEARG HOW TO DO THIS???
		if (curPacketLen + (MIDI_PACKET_LEN - curMidiEventLen) > MAX_BLE_MIDI_PACKET_LEN) {

			writeHeader();
		}

		curMidiEventLen = 1;

		// add timestamp first, unless it's running status
		if (midiByte == currentRunningStatus) {
			// don't bother writing the running status!
			return 0;
		}

		writeTimestamp();
		currentRunningStatus = midiByte;
	}
	else if (isNewPacket) {
		ble_trace1("Invalid midi packet - starting in the middle of a MIDI message at start of BLE packet: %X", midiByte);
		return 0;
	}
	else {
		// assumes the longest packet can be 3 bytes
		if (++curMidiEventLen >= MIDI_PACKET_LEN) {
			curMidiEventLen = 0;
		}
	}

	// TODO: better overflow recovery - skip all unitl next status or something???
	// for now we just overwrite the oldest entry.
	//if (CBUF_IsFull(midiBuffer))
	CBUF_Push(midiBuffer, midiByte);

	++curPacketLen;

	return 1;
}


void onSendMidiPacket() {
	isNewPacket = TRUE;

	// not sure if the spec requires a new header with a running status that stretches across packets.
	// for now we'll be on the safe side.
	currentRunningStatus = 0;
}

/***
 * Sends the next packet in the current buffer.
 * Returns true if there are additional full packets still in the buffer.
 *
 */
BOOL getMidiPacket(BLEPROFILE_DB_PDU* buff) {

	UINT8 len = MIN(curPacketEndLen, MAX_BLE_MIDI_PACKET_LEN);//(CBUF_Len(midiBuffer) - curMidiEventLen);

	for (UINT8 i = 0; i < len; i++) {
		buff->pdu[i] = CBUF_Pop(midiBuffer);

		curPacketEndLen--;
	}
	buff->len = len;

	onSendMidiPacket();

	return (curPacketEndLen > 0);
}


void incrementMidiTimestamp() {
	if (++midiTimestamp > MAX_TIMESTAMP_VALUE) {
		midiTimestamp = 0;
	}
}
