/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2017 Andy S. Lustig

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
const byte version = 62;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// #define MCUBE //uncomment to disable feedback during a trigger
// #define OLDBOARD //uncomment if uploading code to Cerebro 4.7 or older
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*

        ______                   __
       / ____/___   _____ ___   / /_   _____ ____
      / /    / _ \ / ___// _ \ / __ \ / ___// __ \
     / /___ /  __// /   /  __// /_/ // /   / /_/ /
     \____/ \___//_/    \___//_.___//_/    \____/

Documentation for this project can be found at http://cerebro.readthedocs.io/
*/
// #include <SoftwareSerial.h>
#include <avr/pgmspace.h>
#include <SPI.h>

// SoftwareSerial mySerial(7,5); //rx,tx pin6 pin8 on attiny84

//--------------OPERATING PARAMETERS--------------------
const int memorySize =     8169;   /*The number of bytes we will save to on the EEPROM. Each event recording is 4 bytes of data
(3 for the timestamp integer and 1 for the event description character). Parameter changes are 9 bytes, 1 for the event description character
and 8 for the former parameters.8192 bytes will provide space for 2730 events to be recorded.*/
#define LOG_START 216
#define FADE_START 16
#define HARDWARE_START 12
#define LEFT 0
#define RIGHT 1

// //IR sensor input//
// #define IR_dirReg         DDRA
// #define IR_inputReg       PINA
// #define IR_pin            1       //attiny84 pin12
// //BTN  input//
// #define BTN_dirReg        DDRA
// #define BTN_outputReg     PORTA
// #define BTN_inputReg      PINA
// #define BTN_pin           0       //attiny84 pin13
//DAC pins setup//
const int oneSelect = 6;
const int twoSelect = 5;
const int photoONE = A0;
const int photoTWO = A1;

const byte indicatorLED = 8; //attiny pin 5, Arduino language pin8

const unsigned int maxpulselength = 1000;//maximum pulse length (in microseconds) that we will listen for. A signal longer than this maxpulselength is not a pulse and indicates the message is over
const byte NUMPULSES = 88;               //maximum length of the message we can receive
const byte irResolution = 4;             //# of microseconds that we will delay before checking the state of the IR sensor again
#define NUMPARAM 5
const char string_0[] PROGMEM = "Start_Delay,";
const char string_1[] PROGMEM = "On_Time,";
const char string_2[] PROGMEM = "Off_Time,";
const char string_3[] PROGMEM = "Train_Duration,";
const char string_4[] PROGMEM = "Ramp_Duration,";

#define ON_DELAY  0
#define ON_TIME   1
#define OFF_TIME  2
#define TRAIN_DUR 3
#define RAMP_DUR  4

// Then set up a table to refer to your strings.
const char* const parameterLabels[] PROGMEM = {string_0, string_1, string_2, string_3, string_4};
char label[20];            // make sure this is large enough for the largest string it must hold
unsigned int waveform[NUMPARAM] = {};
unsigned int onDelay = 2000;
char marksReceived;
unsigned int address = LOG_START;  /*start at the 217th byte address of eeprom. The first 216 bytes are reserved for persistant variables that are recalled from session to session (waveform parameters,hardware parameters,fade vector)*/
bool trigMatch = false;
bool stopMatch = false;
int error;
const float KP = 0.2;
const byte interval = 3;
bool isSettled = false;
bool isMaxed = false;
bool implantMode = false;
bool diodeMode = false;
bool receivingFadeVector = false;
bool receivingHardwareVector = false;
bool receivingPowerTest = false;
bool powerTestMode = false;
unsigned int tempPower = 0;
byte vectorIndex = 0;
unsigned int powerLevel;
unsigned int cerebroNum;
unsigned int LD;

const char saveMemoryFlag = -1;
const char memoryDumpFlag = -2;
const char resetAddressFlag = -3;

#ifdef MCUBE
int DAClevel = 725;
#else
int DAClevel = 0;
#endif

//---------function prototypes---------//
void characterizeDiode();
void characterizeImplant();
void triggerEvent(unsigned int desiredPower,bool whichLaser, bool useFeedback=true );
void feedback(int setPoint,int phototransistor);
void fade();
byte listenForIR(int timeout=0);
void updateWaveform(uint16_t (&marks)[NUMPULSES]);
unsigned int convertBIN(uint16_t (&marks)[NUMPULSES],byte numMarks=4,byte start=0);
void recordEvent(byte letter);
void eepromWriteByte( unsigned int writeAddress, byte data ) ;
byte eepromReadByte( unsigned int readAddress );
void save2EEPROM();
void readAddresses(int start, int finish);
void printEEPROM();
void sendDAC(unsigned int value, int slaveSelect);
bool laserOFF(int slaveSelect , int phototransistor);
void updateFadeVector(uint16_t (&marks)[NUMPULSES],byte offset);
void updateHardware(uint16_t (&marks)[NUMPULSES]);
void printFadeVector();
void printParameters();
int getLightLevel();
void recordLightLevel(int lightLevel);

void setup() {
  ///////////Analog setup////////////////////////////
  // ADMUX = B00000010;       //Vcc used as analog reference reference. PA2 is pin 11 on ATtiny84. ADMUX explained in SECTION 16.13.1 of datasheet
  // ADCSRA |= (1<<ADPS2) | (1<<ADPS1);  //set division factor of 64. ADC frequncy = 8Mhz/64=125khz (ADC needs to be in 50-200khz range)
  // ADCSRA |= (1<<ADEN);                //enable the ADC
  ////////////Digital setup//////////////////////////

  pinMode(oneSelect,OUTPUT);
  digitalWrite(oneSelect,HIGH);
  pinMode(twoSelect,OUTPUT);
  digitalWrite(twoSelect,HIGH);
  SPI.begin();
  pinMode(indicatorLED,OUTPUT);       //Output
  digitalWrite(indicatorLED,LOW);

  Serial.begin(115200);  
  Serial.println("hello. is this thing on?");
  laserOFF(oneSelect,photoONE);
  laserOFF(twoSelect,photoTWO);
  
}

void loop() {
  Serial.println("new loop");
  delay(1000);
  triggerEvent(20,LEFT,true);
  delay(1000);
  triggerEvent(40,RIGHT,true);
}

