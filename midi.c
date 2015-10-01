/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
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

/*
 * midi.c
 *
 *  Created on: Feb 23, 2015
 *      Author: tyler
 */

#include "midi.h"

#include "circular_buffer.h"

/***
 * Set to 0 to disable running status and always send the full packet.
 */
#define ENABLE_RUNNING_STATUS 1

#ifdef ENABLE_TEST_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#define MIDI_PACKET_LEN 3

/***
 * Returns the expected length of the following MIDI packet,
 * based on its status byte. Length includes the status byte.
 * TODO: for now, assumes all midi messages are 3 bytes!
 */
#define GET_MIDI_PACKET_LEN(statusByte) (3)

#define MIDI_TIMESTAMP_LEN 1

#define MAX_TIMESTAMP_VALUE 8192

#define TRUE 1
#define FALSE 0



// make a big buffer just in case. this shouldn't really actually be longer than 16 bytes at a longshot.
#define midiBuffer_SIZE 128

typedef int Timestamp;

typedef struct {
	Timestamp timestamp;
	UINT8 value;
} TimestampedByte;


/***
 * This stores the MIDI bytes WITH timestamps.
 */
volatile struct
{
   UINT8     m_getIdx;
   UINT8     m_putIdx;
   TimestampedByte     m_entry[ midiBuffer_SIZE ];

} midiBuffer;





int midiTimestamp = 0;

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
UINT8 writeHeader(UINT8* buff, Timestamp timestamp) {
	UINT8 byte;
	byte = (1 << 7) | ((timestamp >> 6) & 0x3f);

	*buff = byte;

	DEBUG_PRINT("\nwriteHeader: %d : %d", timestamp, byte);

	return 1;
}

// add timestamp first
UINT8 writeTimestamp(UINT8* buff, Timestamp timestamp) {
	UINT8 byte;
	byte = (1 << 7) | (timestamp & 0x7f);

	*buff = byte;

	DEBUG_PRINT("\nwriteTimestamp: %d : %d", timestamp, byte);

	return MIDI_TIMESTAMP_LEN;
}

UINT8 saveMIDIDataToBuffer(char midiByte) {

	// TODO: better overflow recovery - skip all unitl next status or something???
	if (!CBUF_IsFull(midiBuffer)) {
		volatile TimestampedByte* entry = CBUF_GetPushEntryPtr(midiBuffer);
		entry->value = midiByte;
		entry->timestamp = midiTimestamp;
		CBUF_AdvancePushIdx(midiBuffer);

		return 1;
	}
	else {
		return 0;
	}
}


void clearMIDIBuffer() {

	currentRunningStatus = 0;

	CBUF_Init(midiBuffer);
}

void onSendMidiPacket() {

	// not sure if the spec requires a new header with a running status that stretches across packets.
	// for now we'll be on the safe side.
	currentRunningStatus = 0;
}

/***
 * Sends the next packet in the current buffer.
 * Returns > 1 if there are additional full packets still in the buffer,
 * 0 on success, or -1 if no packets are available for copying.
 *
 * buff : the profile PDU to store the next packet into.
 * maxLen : the maximum amount of bytes to fill (this should be MAX_BLE_MIDI_PACKET_LEN)
 *
 */
int getMidiPacket(BLEPROFILE_DB_PDU* buff, UINT8 maxLen) {

	UINT8 len = maxLen;//(CBUF_Len(midiBuffer) - curMidiEventLen);

	UINT8 i = 0;

	Timestamp lastTimestamp = 0;
	for (; i < len && CBUF_Len(midiBuffer) > 0; i++) {
		// peek first.
		TimestampedByte curByte = CBUF_Get(midiBuffer, 0);

		// if we're at the beginning of a packet,
		// we must throw away any remaining tails of MIDI messages until we find the next full one.

DEBUG_PRINT("\ngetMidiPacket: %X to %d. (buflen: %d)", curByte.value, i, CBUF_Len(midiBuffer));
		if (i == 0) {
			while (!IS_MIDI_STATUS(curByte.value) && CBUF_Len(midiBuffer) > 1) {
				CBUF_AdvancePopIdx(midiBuffer);
				curByte = CBUF_Get(midiBuffer, 0);

				ble_trace1("\nERROR: Invalid midi packet - starting in the middle of a MIDI message at start of BLE packet: %X", curByte.value);
			}

			// if there's not a full message left, we have to give up
			if (CBUF_Len(midiBuffer) < GET_MIDI_PACKET_LEN(curByte.value)) {

DEBUG_PRINT("\ngetMidiPacket: not a full message left, giving up %d", CBUF_Len(midiBuffer));
				break;
			}

			i += writeHeader(&(buff->pdu[i]), curByte.timestamp);
		}

		if (IS_MIDI_STATUS(curByte.value)) {

			if (CBUF_Len(midiBuffer) < GET_MIDI_PACKET_LEN(curByte.value)) {
				DEBUG_PRINT("\ngetMidiPacket: buffer has no more full messages, giving up with %d bytes left", CBUF_Len(midiBuffer));
				break;
			}

DEBUG_PRINT("\ngetMidiPacket: got status %X", curByte.value);
			// add timestamp first, unless it's running status
			if (curByte.value == currentRunningStatus && curByte.timestamp == lastTimestamp) {

				DEBUG_PRINT("\ngetMidiPacket: skipping RUNNING status %X", curByte.value);

				// don't bother writing the running status!
				// pop it and toss it, then get the next byte,
				// as long as there IS another MIDI message waiting and it can fit
				if (i + (GET_MIDI_PACKET_LEN(curByte.value) - 1) < maxLen) {
					CBUF_AdvancePopIdx(midiBuffer);
					curByte = CBUF_Get(midiBuffer, 0);
				}
				else {

					DEBUG_PRINT("\ngetMidiPacket: not enough room for running status %X", curByte.value);
					break;
				}
			}
			else {
#if ENABLE_RUNNING_STATUS
				currentRunningStatus = curByte.value;
#endif
				// if there's no more room in this packet for the full message,
				// we give up; they'll call it again to get the next full packet.
				int packetLen = GET_MIDI_PACKET_LEN(curByte.value);
				if (i + (packetLen + MIDI_TIMESTAMP_LEN) >= maxLen) {
					DEBUG_PRINT("\ngetMidiPacket: no more room, giving up %d", curByte.value);
					break;
				}
				else {
					// write timestamp for new MIDI message
					i += writeTimestamp(&(buff->pdu[i]), curByte.timestamp);
				}
			}

			// only track timestamps on NEW events, not bytes in-between statuses
			lastTimestamp = curByte.timestamp;
		}

		// make our peek into a pop
		CBUF_AdvancePopIdx(midiBuffer);

		buff->pdu[i] = curByte.value;
	}

	DEBUG_PRINT("\ngetMidiPacket: done processing. total len: %d", i);
	buff->len = i;

	onSendMidiPacket();

	if (i > 0) {
		UINT8 remaining = CBUF_Len(midiBuffer);
		return ((remaining > 0) && remaining >= GET_MIDI_PACKET_LEN( (CBUF_Get(midiBuffer, 0)) ));
	}
	else {
		return -1;
	}
}


void incrementMidiTimestamp() {
	if (++midiTimestamp > MAX_TIMESTAMP_VALUE) {
		midiTimestamp = 0;
	}
}


void syncMidiTimestamp(int ts) {
	midiTimestamp = MIN(ts, MAX_TIMESTAMP_VALUE);
}


/***
 * Takes a BLE MIDI packet and parses the header and timestamps
 *
 * Returns the most recent/last timestamp, or -1 if cannot be parsed.
 */
int parseMidiTimestamp(UINT8  *packet, int len) {

	if (len > 1) {
		// validate that it is a true timestamp byte
		if (packet[1] & 0x80 == 0) {
			return -1;
		}

		int upperbytes = (packet[0] & 0x3f) << 6; // header
		int lowerbytes = (packet[1] & 0x7f); // timestamp

// TODO: this won't work for sysexes! keep track of them!

		// TODO: skip all midi events until the last timestamp
//		int i = 1;
//		for (; i < len; i += 3) {
//
//		}

		return upperbytes | lowerbytes; // full 13-bit timestamp
	}
	else {
		return -1;
	}
}
