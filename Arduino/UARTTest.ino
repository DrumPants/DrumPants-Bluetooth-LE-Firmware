/*
 Test sketch that sends MIDI data over the serial UART to the BLE chip.

 
 Adapted from a sketch from:
 Spark Fun Electronics 2011
 Nathan Seidle

 */

// set to 1 if using an Arduino Due, set to 0 if using normal Arduino (e.g. Uno).
#define IS_DUE 1

// if 1, sends messages over Serial1 using the BLE baud rate. 
// if 0, sends messages to a MIDI chip at standard MIDI baud rate.
#define SEND_MIDI_TO_BLE 1

#define BROADCOM_BAUD 115200
#define BLUEGIGA_BAUD 19200
  
#if IS_DUE
  // lets the debug printing and input happen on Native port
  #define SerialToComputer Serial
 
  // send midi to HW serial
  // Serial1 is Bluetooth
  // Serial2 is MIDI???
  #if SEND_MIDI_TO_BLE  
    #define mySerial Serial1
  #else 
    #define mySerial Serial2
  #endif
#else
  #define SerialToComputer Serial
  #include <SoftwareSerial.h>
  //SoftwareSerial mySerial(2, 3); //Soft TX on 3, we don't use RX in this code
#endif

byte note = 0; //The MIDI note value to be played
#if IS_DUE
  byte resetMIDI = 87; //Tied to VS1053 Reset line
#else
  byte resetMIDI = 4; //Tied to VS1053 Reset line
#endif

byte ledPin = 13; //MIDI traffic inidicator
int instrument = 0;

void setup() {
  SerialToComputer.begin(57600);

  //Setup soft serial for MIDI control
#if SEND_MIDI_TO_BLE

  mySerial.begin(BROADCOM_BAUD);
#else
  mySerial.begin(31250);
#endif

  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  //digitalWrite(resetMIDI, LOW);
  //pinMode(resetMIDI, INPUT);
  
  
  
  
  
  
  // test pins
  int testPin = 10;
  pinMode(testPin, OUTPUT);
  digitalWrite(testPin, HIGH);
  
  
  
  
  
  
  
  delay(2000);

  noteOn(0, 60, 127);
  noteOn(1, 60, 127);
  noteOn(2, 60, 127);
  //noteOff(0, 60, 127);
  //noteOff(1, 60, 127);
  //noteOff(2, 60, 127);
  //noteOff(0, 80, 127);  
  
  talkMIDI(0xB0, 0x07, 127); //0xB0 is channel message, set channel volume to near max (127)

  noteOn(0, 70, 127);
  
  talkMIDI(0xB0, 0, 0x78); //Select the bank of really fun sounds

  noteOn(0, 81, 127);
  
  //For this bank 0x78, the instrument does not matter, only the note
  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command

  noteOn(0, 80, 127);

  SerialToComputer.println("Press a letter and press enter");
}

int curCount = 0;

long long lastUpdate = 0;
const int updateInterval = 500;

void loop() {
  
  
  long long curMillis = millis();
  if (curMillis - lastUpdate > updateInterval) {
    ++curCount;

    if (curCount > 100) {
      curCount = 0;
      
      // switch instruments
      instrument = (instrument + 5) % 60;
      
      if (instrument > 0) {
        talkMIDI(0xB0, 0, 0x79); //Select the bank of melodic sounds
        talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
        
      }
    }
    
    //noteOn(0, curCount + 20, 60);
    mySerial.write(curCount);
    SerialToComputer.println("Sent midi");
    
    lastUpdate = curMillis;
  }
  
  
  
  
  
  if (SerialToComputer.available() != 0) {

    int note = SerialToComputer.read();
    //Good sounding notes range from 27 to 87
    //So let's take the letter and range it to 27
    note -= 'a';
    note += 57;
  
    SerialToComputer.print("Note=");
    SerialToComputer.println(note, DEC);
  
    //For this bank 0x78, the instrument does not matter, only the note
    noteOn(0, note, 60);
  
    //Note will dissapate automatically
  }
  
  // also write out whatever we receive!
  while (mySerial.available() > 0) {
      SerialToComputer.write(mySerial.read());
  }
}

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);//print(cmd, BYTE);
  mySerial.write(data1);//print(data1, BYTE);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  //if( (cmd & 0xF0) <= 0xB0)
  if( (cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) == 0xE0)
    mySerial.write(data2);//print(data2, BYTE);

  digitalWrite(ledPin, LOW);
}





