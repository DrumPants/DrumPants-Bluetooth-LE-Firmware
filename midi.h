/*
 * midi.h
 *
 *  Created on: Feb 23, 2015
 *      Author: tyler
 */

#ifndef MIDI_H_
#define MIDI_H_

#include "circular_buffer.h"


// make a big buffer just in case. this shouldn't really actually be longer than 16 bytes at a longshot.
#define midiBuffer_SIZE 128

/***
 * This stores the MIDI bytes WITH timestamps and headers according to the Apple BLE spec.
 * https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0CEcQFjAB&url=https%3A%2F%2Fdeveloper.apple.com%2Fbluetooth%2FApple-Bluetooth-Low-Energy-MIDI-Specification.pdf&ei=YN7qVLG2ApfVoAT46ILYDg&usg=AFQjCNEKQd0Bagh2qTNFyIpSgYa1okrPdQ&bvm=bv.86475890,d.cGU
 */
volatile struct
{
   UINT8     m_getIdx;
   UINT8     m_putIdx;
   UINT8     m_entry[ midiBuffer_SIZE ];

} midiBuffer;


/***
 * Parses and saves the MIDI, and constructs in midiBuffer the packets (with headers and timecodes)
 * to send to Apple MIDI over BLE.
 */
UINT8 saveMIDIDataToBuffer(int timestamp, char midiByte);

void incrementMidiTimestamp();

void onSendMidiPacket();

/***
 * Sends the current buffer up to and including the last full packet.
 */
UINT8 getMidiPacket(BLEPROFILE_DB_PDU* buff);


#endif /* MIDI_H_ */
