#include "midi.h"

#include <stdio.h>

#define NUM_TESTS (1000)
#define MAX_TIME_BETWEEN_NEXT_TICK (5)

UINT8 goodMessages[] = {
		0x90, 60, 64,
		0x90, 63, 127,
		0x90, 113, 70,

		0x80, 60, 0,

		0x90, 40, 4,
		0x90, 23, 120,
		0x80, 63, 127,
		0x80, 113, 70,

		0x80, 40, 4,
		0x80, 23, 120
};

UINT8 badMessages[] = {
		0x90, 60, 64,
		0x90, 63, 127,
		0x90, 13, 70,

		0x80, 60, 0,

		0x90, 40, 4,
		34, // this should throw a wrench in things.
		0x90, 23, 120,
		0x80, 63, 127,
		0x80, 13, 70,

		0x80, 40, 4,
		0x80, 23, 120
};

UINT8 badMessages2[] = {
		// start off on a stale half-message
		60, 64,
		0x90, 60, 64,
		0x90, 63, 127,
		0x90, 13, 70,

		0x80, 60, 0,

		0x90, 40,
		0x90, 23, 120,
		0x80, 63, 127,
		0x80, 13, 70,

		0x80, 40, 4,
		0x80, 23, 120,


		0x90, 60, 64,
		0x90, 63, 127,
		0x90, 13, 70,

		0x80, 60, 0,

		0x0, 40, 4, // also this should fuck it up.
		0x90, 23, 120,
		0x80, 63, 127,
		0x80, 13, 70,

		0x80, 40, 4,
		0x80, 23, 120
};


void printMidiPacket(BLEPROFILE_DB_PDU* buff) {
	int i;
	for (i = 0; i < buff->len; i++) {
		if (IS_MIDI_STATUS(buff->pdu[i])) {
			printf("\n");
		}

		printf(" %X", buff->pdu[i]);
	}
}

// send all test messages and print results
void testMessages(UINT8 midi[], int len) {
	int timeTillNextTick = MAX_TIME_BETWEEN_NEXT_TICK;
	int timeTillNextSend = MAX_TIME_BETWEEN_NEXT_TICK + 3;


	printf("\nAdding %d bytes", len);

	int i;
	for (i = 0; i < len + 10; i++) { // also try to send empty midi packets.

		if (--timeTillNextTick <= 0) {
			incrementMidiTimestamp();

			// TODO: make this somewhat random
			timeTillNextTick = MAX_TIME_BETWEEN_NEXT_TICK;
		}

		//printf("\nReading in new MIDI data");
		if (i < len && !saveMIDIDataToBuffer(midi[i])) {
			printf("\nsaveMIDIDataToBuffer failed");
		}

		if (--timeTillNextSend <= 0 || i == len - 1) {
			BLEPROFILE_DB_PDU buff;

			printf("\nstarting getMidiPacket");
			while (getMidiPacket(&buff, 21) >= 0) {
				printf("\n\nReturned MidiPacket: ");
				printMidiPacket(&buff);
				printf("\n\n");
			}

			timeTillNextSend = MAX_TIME_BETWEEN_NEXT_TICK + 3;
		}

	}
}


int main( int argc, const char* argv[] ) {
	int numTests = NUM_TESTS;

	int j;
	for (j = 0; j < numTests; j++) {

		printf("\n\n\ntestMessages: good");
		testMessages(goodMessages, sizeof(goodMessages));

		printf("\n\n\ntestMessages: bad");
		testMessages(badMessages, sizeof(badMessages));

		printf("\n\n\ntestMessages: bad2");
		testMessages(badMessages2, sizeof(badMessages2));

	}


	// test timestamps
	for (j = 0; j < numTests; j++) {

	}
}
