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

When an IR emitter sends a burst of IR light flickering at 38khz, an IR sensor can detect the burst and output
a corresponding low signal (mark). When the IR sensor isn't detecting these bursts of flickering IR light,
it is outputting a high signal (space). We are trying to send a message wirelessly using bursts of flickering
IR light from an IR led on Base Station. The IR sensor on Cerebro will detect these bursts and output a sequence of corrseponding marks
and spaces. Unique messages can be sent using patterns of marks and spaces with particular durations.

Cerebro is waiting for a train of marks from the IR sensor. If it receives exactly four marks of certain lengths,
(long, short, long, short), it will provide power to the onboard laser diode. If it receives exactly 87 marks,
it will check if the first 7 marks are of particular lengths (long, long, short, short , long, short, long). If they
match, the next 80 marks will be converted to binary based on their durations (long mark=1, short mark=0). The 80 bits
are converted to 5 separate 16 bit unsigned integers (0-65535) and then assigned as new pulse parameters. If Cerebro receives exactly 26
marks, it will write the most recently used memory address to the first 2 bytes of the memory. When the log is being printed, it will
know which address to stop at by looking at the first 2 bytes of memory.

listenForIR() function is based on code from Adafruit IR sensor tutorial: https://learn.adafruit.com/ir-sensor/reading-ir-commands
Adafruit code: https://github.com/adafruit/IR-Commander/blob/master/ircommander.pde
Detailed explanation of how infrared (IR) remotes work: http://www.sbprojects.com/knowledge/ir/index.php#top
Directions for uploading can be found at http://cerebro.readthedocs.io/
*/
#include <SoftwareSerial.h>
#include <TinyWireM.h>        // Library for ATtiny hardware I2C communication using the USI pins https://github.com/adafruit/TinyWireM
#include <avr/pgmspace.h>
SoftwareSerial mySerial(7,5); //rx,tx pin6 pin8 on attiny84

//--------------OPERATING PARAMETERS--------------------
const int memorySize =     8169;   /*The number of bytes we will save to on the EEPROM. Each event recording is 4 bytes of data
(3 for the timestamp integer and 1 for the event description character). Parameter changes are 9 bytes, 1 for the event description character
and 8 for the former parameters.8192 bytes will provide space for 2730 events to be recorded.*/
#define LOG_START 216
#define FADE_START 16
#define HARDWARE_START 12

//IR sensor input//
#define IR_dirReg         DDRA
#define IR_inputReg       PINA
#define IR_pin            1       //attiny84 pin12
//BTN  input//
#define BTN_dirReg        DDRA
#define BTN_outputReg     PORTA
#define BTN_inputReg      PINA
#define BTN_pin           0       //attiny84 pin13
//DAC pins setup//
#define DATA_dirReg       DDRB
#define DATA_outputReg    PORTB
#define DATA_pin          0       //attiny84 pin2
#define CLK_dirReg        DDRB
#define CLK_outputReg     PORTB
#define CLK_pin           1       //attiny84 pin3
#define LATCH_dirReg      DDRA
#define LATCH_outputReg   PORTA
#define LATCH_pin         3       //attiny84 pin10
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
void characterizeRoutine();
void triggerEvent(unsigned int desiredPower, bool useFeedback=true);
void feedback(int setPoint);
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
void myShift(int val);
void sendDAC(unsigned int value);
bool laserOFF();
void updateFadeVector(uint16_t (&marks)[NUMPULSES],byte offset);
void updateHardware(uint16_t (&marks)[NUMPULSES]);
void printFadeVector();
void printParameters();
int getLightLevel();
void recordLightLevel(int lightLevel);

void setup() {
  ///////////Analog setup////////////////////////////
  ADMUX = B00000010;       //Vcc used as analog reference reference. PA2 is pin 11 on ATtiny84. ADMUX explained in SECTION 16.13.1 of datasheet
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1);  //set division factor of 64. ADC frequncy = 8Mhz/64=125khz (ADC needs to be in 50-200khz range)
  ADCSRA |= (1<<ADEN);                //enable the ADC
  ////////////Digital setup//////////////////////////
  IR_dirReg &= ~(1<<IR_pin);          //IR sensor pin as input
  BTN_dirReg &= ~(1<<BTN_pin);        //BTN pin as input
  BTN_outputReg |= (1<<BTN_pin);      //Enable pullup
  DATA_dirReg |= (1<<DATA_pin);       //Output
  CLK_dirReg  |= (1<<CLK_pin);        //Output
  LATCH_dirReg |=  (1<<LATCH_pin);    //Output
  LATCH_outputReg |= (1<<LATCH_pin);  //Set Chip Select HIGH (LOW selects the chip)
  pinMode(indicatorLED,OUTPUT);       //Output
  digitalWrite(indicatorLED,LOW);

  mySerial.begin(115200);
  TinyWireM.begin();

  //Get the saved parameters from EEPROM
  for (int i = 0; i<NUMPARAM; i++){
    waveform[i] = word(eepromReadByte(2*i+2)<<8|eepromReadByte(2*i+3));
  }
  cerebroNum = word(eepromReadByte(HARDWARE_START)<<8|eepromReadByte(HARDWARE_START+1));
  LD = word(eepromReadByte(HARDWARE_START+2)<<8|eepromReadByte(HARDWARE_START+3));
  powerLevel = word(eepromReadByte(FADE_START)<<8|eepromReadByte(FADE_START+1));

  //if the BTN is pressed (Low Signal) when cerebro starts up then print the log from the eeprom
  #ifdef OLDBOARD
  if((BTN_inputReg & (1<<BTN_pin))){
  #else
  if(!(BTN_inputReg & (1<<BTN_pin))){
  #endif
    printEEPROM();
    printFadeVector();
  }
  //otherwise just print the waveform parameters
  else{
    printParameters();
  }
  laserOFF();
}

void loop() {
  marksReceived = listenForIR();      //wait for IR signal and return the number of marks received
  if (trigMatch) {                    //trigger light upon receiving exactly 4 marks of with durations that match a key
    trigMatch = false;
    #ifdef MCUBE
    triggerEvent(powerLevel,false);
    #else
    triggerEvent(powerLevel,true);
    #endif
  }
  else if(implantMode || diodeMode){
    digitalWrite(indicatorLED,HIGH);
    characterizeRoutine();
    implantMode = false;
    diodeMode = false;
    digitalWrite(indicatorLED,LOW);
  }
  else if (powerTestMode){
    #ifdef MCUBE
    eepromWriteByte(FADE_START,tempPower>>8);
    eepromWriteByte(FADE_START+1,tempPower & 255);
    delay(100);
    powerLevel = word(eepromReadByte(FADE_START)<<8|eepromReadByte(FADE_START+1));
    printParameters();
    #else
    triggerEvent(tempPower);
    #endif
    powerTestMode = false;
  }
  else if (marksReceived==memoryDumpFlag){
    mySerial.println(F("Memory Contents:"));
    readAddresses(LOG_START,8100); //print the remaining contents
  }
  else if (marksReceived == saveMemoryFlag) {         //save data to EEPROM upon receiving exactly 26 marks
    save2EEPROM();
  }
  else if (marksReceived == resetAddressFlag){
    address = LOG_START; // reset address
    for (byte i=0; i <3; i ++){
      digitalWrite(indicatorLED,HIGH);
      delay(100);
      digitalWrite(indicatorLED,LOW);
      delay(100);
    }
  }
}

byte listenForIR(int timeout=0) {
  /*This function builds an array of high pulse and low pulse lengths
  When it receives a pulse, it starts counting in 2 microsecond (the resolution) increments
  then stores that sum in an array. We can later estimate how long a IR pulse was by mulitplying this sum by the resolution.
  If at any point the pulse length is too long, we know the there are no more pulses coming for that particular message,
  so we should exit */
  byte pulsePairIndex = 0;
  uint16_t marks[NUMPULSES];    //array that stores the marks lengths
  while (1) {
    uint16_t spaceLength = 0;
    uint16_t markLength = 0;
    while (IR_inputReg & (1 << IR_pin)) {    //while the pin is high, count how long it is high by adding to spaceLength
      spaceLength++;
      delayMicroseconds(irResolution);
      if (pulsePairIndex==4){
        if (convertBIN(marks)==10){     //We received a trigger message (High,Low,High,Low)
          trigMatch = true;
          return pulsePairIndex;
        }
        else if(convertBIN(marks)==6){ //we received stop message (Low,High,High,Low)
          stopMatch = true;
          return pulsePairIndex;
        }
      }
      if((spaceLength >= (maxpulselength + timeout) && (pulsePairIndex != -1*timeout)) || pulsePairIndex == NUMPULSES) { //if the space is too long, the message is over.
        delayMicroseconds(1);//Don't know why this is needed, but it is....
        if (pulsePairIndex==87 && convertBIN(marks,7)==101){ //We received data.
          if (receivingFadeVector){
            digitalWrite(indicatorLED, HIGH);
            updateFadeVector(marks,vectorIndex*10);
            if (vectorIndex<19){ //100 fade values sent 5 at a time = 20 messages
              vectorIndex++;
            }
            else{ //after 20th set of values is received
              receivingFadeVector = false;
              mySerial.print(F("Fade Vector Updated:\r"));
              printFadeVector();
            }
            digitalWrite(indicatorLED, LOW);
          }
          else if (receivingHardwareVector){
            updateHardware(marks);
            receivingHardwareVector = false;
            digitalWrite(indicatorLED, LOW);
            mySerial.print(F("Hardware Properties Updated:\r"));
          }
          else if (receivingPowerTest){
            powerTestMode = true;
            receivingPowerTest = false;
            tempPower = convertBIN(marks,16,7);
          }
          else {  //recieved new waveform values
            updateWaveform(marks);
          }
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==22){ //enter characterize mode
          implantMode = true;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==105){ //enter characterize mode
          diodeMode = true;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==117){ //about to receive fade vector
          receivingFadeVector = true;
          vectorIndex = 0;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==97){ //about to receive hardware vector
          receivingHardwareVector = true;
          digitalWrite(indicatorLED, HIGH);
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==36){ //about to receive powerTest level
          receivingPowerTest = true;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==45){ //set dump Memory flag
          return memoryDumpFlag;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==76){ //set saveMemory flag
          return saveMemoryFlag;
        }
        else if (pulsePairIndex==7 && convertBIN(marks,7)==77){ //set resetAddress flag
          return resetAddressFlag;
        }
        // else{ //We received a message that we don't understand
        //   mySerial.println(pulsePairIndex);
        // }
        return pulsePairIndex;
      }
    }
    while (! (IR_inputReg & (1<<IR_pin))) {   // while the pin is low, count how long it is low by adding to markLength
      markLength++;
      delayMicroseconds(irResolution);
    }
    marks[pulsePairIndex] = markLength;
    pulsePairIndex++;
  }
}
