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
 * Resets the MIDI to the given time.
 * Use to sync to the Master's clock.
 *
 * ts : Current timestamp in ms.
 */
void syncMidiTimestamp(int ts);

/***
 * Takes a BLE MIDI packet and parses the header and timestamps
 *
 * Returns the most recent/last timestamp.
 */
int parseMidiTimestamp(UINT8  *packet, int len);


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
