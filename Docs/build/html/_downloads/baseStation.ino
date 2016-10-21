/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2016 Andy S. Lustig

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

/*
Cerebro Base Station(CBS) emits IR signals to communicate with Cerebro.
CBS has two modes: USB and MBED. In USB mode, CBS can be triggered either by a high signal on pin 3, or through serial
communication coming from serialSender. In MBED mode, CBS is triggered by serial communication coming from an MBED.
Based on code from Adafruit IR sensor tutorial: https://learn.adafruit.com/ir-sensor/making-an-intervalometer
Upload to ATtiny85 at 8MHz for proper timing. ATtiny core for Arduino IDE found here: https://code.google.com/p/arduino-tiny/
*/

#include <EEPROM.h>         //***Burned bootloader should have EESAVE bit (Fuse high bit no 3) set low to prevent EEPROM from being cleared whenever a sketch is uploaded***
#include <Cerebro.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
Cerebro cerebro(BASESTATION);
SoftwareSerial mySerial(3,4);
#define triggerIn_reg PINB
#define stopIn_reg    PINB
#define triggerIn   2              //pin 7 on 85
#define stopIn      0              //pin 5 on 85
#define CONTINUATION 1
String startupMsg;
unsigned long timeOffset;
byte values[numParameters][6];
byte version = 21;
unsigned long powers[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
long triggerClock = 0;
unsigned int spamFilter;  /*Bcontrol is indiscriminately sending stop signals every time the center nose poke is entered.
                          We don't want Base Station's log file to be filled with "Stop Sent" events that weren't applicable to Cerebro.
                          Therefore, Base Station filters Bcontrol commands, only emitting stop signals to Cerebro if we think
                          Cerebro is in the middle of a pulse. Likewise for trigger events, we only emit trigger signals if we do NOT think
                          Cerebro is in the middle of a pulse*/

void setup()   {
  MCUSR = 0;  //clear MCU register of previous reset flags
  wdt_disable();//disable watchdog timer
  pinMode(triggerIn, INPUT);
  pinMode(stopIn, INPUT);
  mySerial.begin(9600);
  spamFilter = EEPROM.read(0)<<8 | EEPROM.read(1); //recall spamFilter from memory
  triggerClock = -spamFilter;

  //setup time synchronization//
  byte msgLength = 0;
  while (!mySerial.available()){}       // wait until serial data is available
  while (mySerial.available()) {        //once available, read and store the message that was sent
    startupMsg+= (char)mySerial.read();
    msgLength++;
    delay(2);
  }
  if (msgLength < 2) {                   //if the chip was just reset through software there is going to be a leftover 'R' in the serial buffer. That's not the message we want.
    startupMsg = "";
    while (!mySerial.available()) {}
    while (mySerial.available()) {
      startupMsg+= (char)mySerial.read();
      delay(1);
    }
  }
  mySerial.print(startupMsg);
  mySerial.print(version);
  mySerial.print(",");
  mySerial.print(spamFilter);
  mySerial.print("\r");
  timeOffset = millis();
}



void loop() {
  //if we read high signal on pin 7, send a trigger to cerebro
  if (triggerIn_reg & (1<<triggerIn)) {
    while(triggerIn_reg & (1<<triggerIn)){} //wait until signal goes low
    triggerFromBase();
  }
  //if we read a high signal on pin 5, send a stop command to cerebro
  if (stopIn_reg & (1<<stopIn)) {
    while(stopIn_reg & (1<<stopIn)){}       //wait until signal goes low
    stopFromBase();
  }
  //if we get a serial messsage from Xavier
  if (mySerial.available()) {
    parseMsg(readMsg());
  }
}


byte readMsg(){
  byte digitIndex = 0;
  byte valueIndex = 0;
  while (mySerial.available()) {
    char msg = mySerial.read();
    if (msg == ',') {
      digitIndex = 0;
      valueIndex++;
    }
    else {
      values[valueIndex][digitIndex] = msg;
      digitIndex++;
      values[valueIndex][5] = digitIndex; //the number of digits that have been read in
      delay(20);
    }
  }
  return valueIndex;
}

void parseMsg(byte numValues){
  //Commands received will be single characters ('T' or 'D' etc.)
  //In the special case that new parameters are received ('12345,12345,12345,12345,12345'), read in the ascii chacaters that were sent (they represent integers separated by commas).
  //Each  values[i] has 6 bytes, the first 5 bytes are for the ascii characters representing digits 0-9, the last byte (index 5) is reserved for storing the number of "digits" received.
  //Later on, cerebro.send() function converts these strings of "digits" to actual integers and then sends them as binary data.

  //received ASCII "T", send trigger to cerebro
  if (values[0][0] == 'T') {
    triggerFromBase();
  }
  //received ASCII "D", send start then stop for reception test.
  else if (values[0][0] == 'D') {
    cerebro.test();
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Test Sent\r"));
  }
  //received ASCII "S", send stop command to cerebro.
  else if (values[0][0] == 'S') {
    stopFromBase();
  }
  else if (values[0][0] == 'Q') {
    cerebro.stop();
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Debug Stop Sent\r"));
  }
  else if (values[0][0] == 'C') {
    cerebro.trigger(CONTINUATION);
    triggerClock = millis();
    mySerial.print(triggerClock - timeOffset);
    mySerial.print(F(",Debug Continuation Sent\r"));
  }
  //received ASCII "E", send save to EEPROM command to cerebro.
  else if (values[0][0] == 'E' ){
    cerebro.saveEEPROM();
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Save Sent\r"));
  }
  //received ASCII "R", reset MCU.
  else if (values[0][0] == 'R') {
    while(mySerial.available()){
      mySerial.read();
    }
    wdt_enable(WDTO_15MS);  // turn on the WatchDog timer
    while(1){}              // do nothing and wait for the reset
  }
  //receiveed ASCII "V", reply with version
  else if (values[0][0] == 'V') {
    mySerial.print(F("Base Station Firmware Version,"));
    mySerial.print(version);
    mySerial.print("\r");
  }
  //receiveed ASCII "L", toggle the IR Lamp
  else if (values[0][0] == 'L'){
    if (cerebro.isNormallyOn){
      cerebro.toggle(0);
    }
    else {
      cerebro.toggle(1);
    }
  }
  //receiveed ASCII "F", update spamFilter value
  if (values[0][0] == 'F'){
    spamFilter = 0;
    for (int i = 0; i < values[1][5]; i++) {
      spamFilter = spamFilter + (values[1][i] - 48) * powers[values[1][5] - i - 1];
    }
    saveVal(spamFilter);
    mySerial.print(F("Filter updated to "));
    mySerial.print(spamFilter);
    mySerial.print(" ms\r");
  }
  else if (values[0][0] == 'X') {
    cerebro.sendBinary(117,7);
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Calibration Vector Sent\r"));
  }
  else if (values[0][0] == 'H') {
    cerebro.sendBinary(97,7);
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Hardware Vector Sent\r"));
  }
  else if (values[0][0] == 'Z') {
    cerebro.calibrate();
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Calibration Routine Start Sent\r"));
  }
  //received new parameter settings. Send 8 bytes of data to cerebro.
  else if (numValues==numParameters-1){
    //convert read ascii digits to an integer
    unsigned int decnum[numParameters];
    for( int k=0; k<numParameters; k++){
        decnum[k] = 0;
        for (int i = 0; i < values[k][5]; i++) {
          decnum[k] = decnum[k] + (values[k][i] - 48) * powers[values[k][5] - i - 1];
        }
    }
    mySerial.print(millis() - timeOffset);
    cerebro.send(decnum);
    mySerial.print(F(",New Parameters Sent"));
    for (int i = 0; i<numParameters; i++){
      mySerial.print(",");
      mySerial.print(decnum[i]);
    }
    mySerial.print("\r");
  }
}

void triggerFromBase(){
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger>spamFilter){
    cerebro.trigger();
    triggerClock = millis();
    mySerial.print(triggerClock - timeOffset);
    mySerial.print(F(",Trigger Sent\r"));
  }
  else{
    cerebro.trigger(CONTINUATION);
    triggerClock = millis();
    mySerial.print(triggerClock - timeOffset);
    mySerial.print(F(",Continue Sent,"));
    mySerial.print(tSinceTrigger);
    mySerial.print("\r");
  }
}

void stopFromBase(){
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger<spamFilter){
    cerebro.stop();
    mySerial.print(millis() - timeOffset);
    mySerial.print(F(",Stop Sent,"));
    mySerial.print(tSinceTrigger);
    mySerial.print("\r");
    triggerClock = -spamFilter; //prevents back to back stop signals from being sent
  }
}

void saveVal(int saveValue){
  EEPROM.write(0,saveValue >> 8);
  EEPROM.write(1,saveValue & 255);
}

void recallVal(){
}
