/*
 * midi.c
 *
 *  Created on: Feb 23, 2015
 *      Author: tyler
 */

#include "midi.h"

#include "circular_buffer.h"


#define IS_MIDI_STATUS(b) ((b & 0x80) != 0)
#define MIDI_PACKET_LEN 3

/***
 * Returns the expected length of the following MIDI packet,
 * based on its status byte. Length includes the status byte.
 * TODO: for now, assumes all midi messages are 3 bytes!
 */
#define GET_MIDI_PACKET_LEN(statusByte) (3)


#define TIMESTAMP_LEN 1

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
 * This stores the MIDI bytes WITH timestamps and headers according to the Apple BLE spec.
 * https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0CEcQFjAB&url=https%3A%2F%2Fdeveloper.apple.com%2Fbluetooth%2FApple-Bluetooth-Low-Energy-MIDI-Specification.pdf&ei=YN7qVLG2ApfVoAT46ILYDg&usg=AFQjCNEKQd0Bagh2qTNFyIpSgYa1okrPdQ&bvm=bv.86475890,d.cGU
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

	return 1;
}

// add timestamp first
UINT8 writeTimestamp(UINT8* buff, Timestamp timestamp) {
	UINT8 byte;
	byte = (1 << 7) | (timestamp & 0x7f);

	*buff = byte;

	return TIMESTAMP_LEN;
}

UINT8 saveMIDIDataToBuffer(char midiByte) {

	// TODO: better overflow recovery - skip all unitl next status or something???
	if (!CBUF_IsFull(midiBuffer)) {
		TimestampedByte* entry = CBUF_GetPushEntryPtr(midiBuffer);
		entry->value = midiByte;
		entry->timestamp = midiTimestamp;
		CBUF_AdvancePushIdx(midiBuffer);

		return 1;
	}
	else {
		return 0;
	}
}


void onSendMidiPacket() {

	// not sure if the spec requires a new header with a running status that stretches across packets.
	// for now we'll be on the safe side.
	currentRunningStatus = 0;
}

/***
 * Sends the next packet in the current buffer.
 * Returns true if there are additional full packets still in the buffer.
 *
 * buff : the profile PDU to store the next packet into.
 * maxLen : the maximum amount of bytes to fill (this should be MAX_BLE_MIDI_PACKET_LEN)
 *
 */
BOOL getMidiPacket(BLEPROFILE_DB_PDU* buff, UINT8 maxLen) {

	UINT8 len = maxLen;//(CBUF_Len(midiBuffer) - curMidiEventLen);

	UINT8 i = 0;

	Timestamp lastTimestamp = 0;
	for (; i < len && CBUF_Len(midiBuffer) > 0; i++) {
		// peek first.
		TimestampedByte curByte = CBUF_Get(midiBuffer, 0);

		// if we're at the beginning of a packet,
		// we must throw away any remaining tails of MIDI messages until we find the next full one.
		if (i == 0) {
			while (!IS_MIDI_STATUS(curByte.value) && CBUF_Len(midiBuffer) > 1) {
				CBUF_AdvancePopIdx(midiBuffer);
				curByte = CBUF_Get(midiBuffer, 0);

				ble_trace1("Invalid midi packet - starting in the middle of a MIDI message at start of BLE packet: %X", curByte.value);
			}

			// if there's not a full message left, we have to give up
			if (CBUF_Len(midiBuffer) < GET_MIDI_PACKET_LEN(curByte.value)) {
				break;
			}

			i += writeHeader(&buff.pdu, curByte.timestamp);
		}

		if (IS_MIDI_STATUS(curByte.value)) {

			// add timestamp first, unless it's running status
			if (curByte.value == currentRunningStatus && curByte.timestamp == lastTimestamp) {
				// don't bother writing the running status!
				// pop it and toss it, then get the next byte,
				// as long as there IS another MIDI message waiting and it can fit
				if (CBUF_Len(midiBuffer) > 1 &&
						(i + (GET_MIDI_PACKET_LEN(curByte.value) - 1) < maxLen)) {
					CBUF_AdvancePopIdx(midiBuffer);
					curByte = CBUF_Get(midiBuffer, 0);
				}
				else {
					break;
				}
			}
			else {
				currentRunningStatus = curByte.value;

				// if there's no more room in this packet for the full message,
				// we give up; they'll call it again to get the next full packet.
				if (i + (GET_MIDI_PACKET_LEN(curByte.value) + TIMESTAMP_LEN) >= maxLen) {
					break;
				}
				else {
					// write timestamp for new MIDI message
					i += writeTimestamp(&(buff.pdu[i]), curByte.timestamp);
				}
			}

			// only track timestamps on NEW events, not bytes in-between statuses
			lastTimestamp = curByte.timestamp;
		}

		// make our peek into a pop
		CBUF_AdvancePopIdx(midiBuffer);

		buff->pdu[i] = curByte.value;
	}
	buff->len = i;

	onSendMidiPacket();

	UINT8 remaining = CBUF_Len(midiBuffer);
	return ((remaining > 0) && remaining >= GET_MIDI_PACKET_LEN( (CBUF_Get(midiBuffer, 0)) ));
}


void incrementMidiTimestamp() {
	if (++midiTimestamp > MAX_TIMESTAMP_VALUE) {
		midiTimestamp = 0;
	}
}
