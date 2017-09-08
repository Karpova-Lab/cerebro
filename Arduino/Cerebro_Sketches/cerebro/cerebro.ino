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
/*
        ______                   __
       / ____/___   _____ ___   / /_   _____ ____
      / /    / _ \ / ___// _ \ / __ \ / ___// __ \
     / /___ /  __// /   /  __// /_/ // /   / /_/ /
     \____/ \___//_/    \___//_.___//_/    \____/

Documentation for this project can be found at https://karpova-lab.github.io/cerebro/
*/
#include <avr/pgmspace.h>
#include <SPI.h>


#define LEFT 0
#define RIGHT 1

//DAC pins setup//
const int leftSelect = 6;
const int rightSelect = 5;
const int leftPhotoFeedback = A0;
const int rightPhotoFeedback = A1;

const byte indicatorLED = 8; //attiny pin 5, Arduino language pin8

const byte NUMPULSES = 88;               //maximum length of the message we can receive
const byte irResolution = 4;             //# of microseconds that we will delay before checking the state of the IR sensor again
#define NUMPARAM 5
const char string_0[] PROGMEM = "Start_Delay,";
const char string_1[] PROGMEM = "On_Time,";
const char string_2[] PROGMEM = "Off_Time,";
const char string_3[] PROGMEM = "Train_Duration,";
const char string_4[] PROGMEM = "Ramp_Duration,";

// Then set up a table to refer to your strings.
const char* const parameterLabels[] PROGMEM = {string_0, string_1, string_2, string_3, string_4};
char label[20];            // make sure this is large enough for the largest string it must hold
unsigned int waveform[NUMPARAM] = {};
int error;
const float KP = 0.2;
const byte interval = 3;
bool isSettled = false;
// bool isMaxed = false;
unsigned int tempPower = 0;
byte vectorIndex = 0;
unsigned int powerLevel;
unsigned int cerebroNum;
unsigned int LD;

int DAClevel = 0;


class LaserDiode{
public:
  uint8_t slaveSelectPin;
  uint8_t analogPin;
  bool isMaxed;
  int16_t DAClevel;  

  LaserDiode(uint8_t _slavePin, uint8_t _analogPin);
  bool off();
  void sendDAC(unsigned int value);
  void feedback(int setPoint);
  void fade();
};

LaserDiode::LaserDiode(uint8_t _slavePin, uint8_t _analogPin){
  slaveSelectPin = _slavePin;
  analogPin = _analogPin;
  isMaxed = false;
  pinMode(slaveSelectPin,OUTPUT);
  digitalWrite(slaveSelectPin,HIGH);
}

bool LaserDiode::off(){
  Serial.println("laser off");
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPin,LOW);
  SPI.transfer(64);                        //Power down command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(slaveSelectPin,HIGH);
  if (DAClevel==4095){
    isMaxed = true;
  }
  DAClevel = 0;
  SPI.endTransaction();   
  return false;
}

void LaserDiode::sendDAC(unsigned int value) {
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPin,LOW);  
  SPI.transfer(48);                        //Write to and Update (Power up) DAC Register command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(value>>4);                  //shift high byte
  SPI.transfer(value<<4 & 255);            //shift low byte
  digitalWrite(slaveSelectPin,HIGH);
  SPI.endTransaction();
}

void LaserDiode::feedback(int setPoint){
  int photocellVoltage = analogRead(analogPin);
  error = setPoint-photocellVoltage;
  DAClevel = DAClevel+int(error*KP);
  if (DAClevel>4095) {
    DAClevel = 4095;
  }
  else if (DAClevel<0){
    DAClevel = 0;
  }
}

void LaserDiode::fade(){
  // unsigned long fadeClock;
  // unsigned int param1;
  // for (int k = FADE_START; k < FADE_START+200 ; k+=2) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
  //   fadeClock = millis();
  //   param1 = eepromReadByte(k)<<8;
  //   feedback(word(param1|eepromReadByte(k+1)));
  //   sendDAC(DAClevel);
  //   while((millis()-fadeClock)<(waveform[RAMP_DUR]/100)){
  //     //wait
  //   }
  // }
}

LaserDiode left(6,A0);
LaserDiode right(5,A1);

//---------function prototypes---------//
void triggerEvent(unsigned int desiredPower, LaserDiode* thediode, bool useFeedback=true );

void setup() {
  SPI.begin();
  Serial.begin(115200);  
  Serial.println("hello. is this thing on?");
  left.off();
  right.off();


}

void loop() {


}

void isolationTest(){
  delay(5000);
  Serial.print("Before left: ");    
  feedbackReadings();
  triggerEvent(600,&left,true);
  delay(1000);  
  Serial.print("Before right: ");  
  feedbackReadings();  
  triggerEvent(600,&right,true);
}
void feedbackReadings(){ 
  Serial.print(analogRead(left.analogPin));
  Serial.print(", ");
  Serial.println(analogRead(right.analogPin));
}