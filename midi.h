/*
 * midi.h
 *
 *  Created on: Feb 23, 2015
 *      Author: tyler
 */

#ifndef MIDI_H_
#define MIDI_H_

#ifndef TEST_SHIM
#	include "bleprofile.h"
#endif


#define IS_MIDI_STATUS(b) ((b & 0x80) != 0)

/***
 * Saves the UART data for later retreival by getMidiPacket().
 *
 * Returns 0 on error.
 */
UINT8 saveMIDIDataToBuffer(char midiByte);

void clearMIDIBuffer();

/***
 * Call every 1ms.
 */
void incrementMidiTimestamp();

/***
 * Sends the next packet in the current buffer.
 * Returns >= 1 if there are additional full packets still in the buffer,
 * 0 on success, or -1 if no packets are available for copying.
 *
 * buff : the profile PDU to store the next packet into.
 * maxLen : the maximum amount of bytes to fill (this should be MAX_BLE_MIDI_PACKET_LEN)
 *
 */
int getMidiPacket(BLEPROFILE_DB_PDU* buff, UINT8 maxLen);


#endif /* MIDI_H_ */
