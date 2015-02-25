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

/***
 * Parses and saves the MIDI, and constructs in midiBuffer the packets (with headers and timecodes)
 * to send to Apple MIDI over BLE.
 *
 * Returns 0 on error.
 */
UINT8 saveMIDIDataToBuffer(char midiByte);

/***
 * Call every 1ms.
 */
void incrementMidiTimestamp();

/***
 * Sends the next packet in the current buffer.
 *
 * Returns true if there are additional full packets still in the buffer.
 *
 * buff : the profile PDU to store the next packet into. Sets the len field to the number of bytes written.
 * maxLen : the maximum amount of bytes to fill (this should be MAX_BLE_MIDI_PACKET_LEN)
 *
 */
BOOL getMidiPacket(BLEPROFILE_DB_PDU* buff, UINT8 maxLen);


#endif /* MIDI_H_ */
