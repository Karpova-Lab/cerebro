/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2018 Andy S. Lustig

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
const uint8_t VERSION = 96; // 2018-11-30
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
        ______                   __
       / ____/___   _____ ___   / /_   _____ ____
      / /    / _ \ / ___// _ \ / __ \ / ___// __ \
     / /___ /  __// /   /  __// /_/ // /   / /_/ /
     \____/ \___//_/    \___//_.___//_/    \____/

Documentation for this project can be found at https://karpova-lab.github.io/cerebro/
*/
#include <EEPROM.h>
#include <LaserDiode.h>
#include <SparkFunBQ27441.h>      //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <Radio.h>                //https://github.com/LowPowerLab/RFM69
#include <LowPower.h>             //https://github.com/LowPowerLab/LowPower
#include <avr/wdt.h>

#define SERIAL_NUMBER_ADDRESS 0
#define CHANNEL_ADDRESS 1
#define WAVEFORM_ADDRESS 2
#define LEFT_SETPOINT_ADDRESS 16
#define RIGHT_SETPOINT_ADDRESS 18
#define MISSING_ARRAY_ADDRESS 20

WaveformData waveform;
DiodePowers diodePwrs;
IntegerPayload integerMessage;
Info cerebroInfo;
Feedback diodeStats;

uint16_t meterVal = 0;

const uint8_t amberLED = A0; //32u4 pin 36
const uint8_t greenLED = 12; //32u4 pin 26
const uint8_t userBTN = 6; //32u4 pin 27
LaserDiode right(&DDRB,&PORTB,0,A4);
LaserDiode left(&DDRD,&PORTD,2,A2);

uint8_t channel;
Radio radio(7,1); //slave select pin, interrupt pin
uint16_t msgCount = 0;
uint16_t missedCount = 0;
uint16_t trigCount = 0;
uint8_t batteryUpdateFrequency = 10;
bool reportBatteryFlag = false;

void setup() {
  SPI.begin();
  // Laser Diodes
  EEPROM.get(LEFT_SETPOINT_ADDRESS,left.setPoint);
  EEPROM.get(RIGHT_SETPOINT_ADDRESS,right.setPoint);
  left.off();
  right.off();

  Serial.begin(115200);

  // Initialize waveform
  EEPROM.get(WAVEFORM_ADDRESS,waveform);

  // UI setup
  pinMode(userBTN,INPUT_PULLUP);
  pinMode(greenLED,OUTPUT);
  pinMode(amberLED,OUTPUT);
  digitalWrite(amberLED,HIGH);

  //*** Battery Monitor ***//
  if (!lipo.begin()){
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while(1){ //blink error
      digitalWrite(amberLED,HIGH);
      delay(200);
      digitalWrite(amberLED,LOW);
      delay(200);
    }
  }
  Serial.println("Connected to BQ27441!");
  lipo.setCapacity(400);
  while(lipo.soc()==0){
    delay(1);
    //wait;
  }

  //*** Radio ***//
  EEPROM.get(SERIAL_NUMBER_ADDRESS,channel);
  radio.radioSetup(CEREBRO,false,channel); //nodeID, autopower off;
  integerMessage.variable = 'Y';
  integerMessage.value = millis();
  if (radio.sendWithRetry(BASESTATION, (const void*)(&integerMessage), sizeof(integerMessage),3,250)){
    Serial.println("Connected to Base Station");
  }
  else{
    Serial.println("Failed to Connect to Base Station");
  }
  printChannel();
  reportVersion();
  digitalWrite(amberLED,LOW);
}

void loop() {
  //
  if (!digitalRead(userBTN)){ //user button is pushed
    uint16_t holdCount = 0;
    uint16_t holdThresh = 5000; //need to hold the button for 5 seconds
    digitalWrite(greenLED,HIGH);
    while(!digitalRead(userBTN) && holdCount < holdThresh){
      delay(1);
      holdCount++;
    }
    if (holdCount >= holdThresh) { //Button was held down for 5 seconds
      //Reset settings that are in memory
      EEPROM.update(SERIAL_NUMBER_ADDRESS, 0);
      waveform = {0,0,1000,0,0,0};
      EEPROM.put(WAVEFORM_ADDRESS,waveform);  //save new waveform to memory
      EEPROM.put(LEFT_SETPOINT_ADDRESS,0);
      EEPROM.put(RIGHT_SETPOINT_ADDRESS,0);

      //blink to indicate button was held long enough and reset has occured
      uint8_t blinkDelay = 85;
      digitalWrite(greenLED,LOW);
      delay(blinkDelay);
      digitalWrite(greenLED,HIGH);
      delay(blinkDelay);
      digitalWrite(greenLED,LOW);
      delay(blinkDelay);
      digitalWrite(greenLED,HIGH);
      delay(blinkDelay);

      //restart MCU so it will 
      Serial.println("Restarting...");
      wdt_enable(WDTO_15MS);  // turn on the WatchDog timer
      while(1){}              // do nothing and wait for the reset
    }
  }
  if (Serial.available()){
    char msg = Serial.read();
    if (msg=='?'){
      printChannel();
      Serial.print("Version: ");
      Serial.println(VERSION);

    }
  }
  //check for any received packets
  if (radio.receiveDone()){
    uint8_t dlay = 200;
    if (radio.DATALEN==1){ //received a command or a request for data
      sendACK();
      switch (radio.DATA[0]){
        case 'B':
          reportBattery();break;
        case 'N':
          msgCount = 0;
          missedCount = 0;
          reportVersion();
          delay(dlay);
          reportPower();
          delay(dlay);
          reportWaveform();
          delay(dlay);
          reportBattery();break;
        case 'i':
          isolationTest();break;
        case 'c':
          combinedTest();break;
        default:
          Serial.println("Command not recognized");break;
      }
    }
    else if (radio.DATALEN == sizeof(waveform)){ //received a waveform data
      sendACK();
      msgCount++;
      waveform = *(WaveformData*)radio.DATA;  //update waveform
      EEPROM.put(WAVEFORM_ADDRESS,waveform);  //save new waveform to memory
      reportWaveform();
    }
    else if (radio.DATALEN == sizeof(diodePwrs)){ //received a waveform data
      sendACK();
      msgCount++;
      diodePwrs = *(DiodePowers*)radio.DATA;
      left.setPoint = diodePwrs.lSetPoint;
      right.setPoint = diodePwrs.rSetPoint;
      EEPROM.put(LEFT_SETPOINT_ADDRESS,left.setPoint);
      EEPROM.put(RIGHT_SETPOINT_ADDRESS,right.setPoint);
      reportPower();
    }
    else if (radio.DATALEN == sizeof(integerMessage)){ //received a variable update
      sendACK();
      integerMessage = *(IntegerPayload*)radio.DATA;
      switch (integerMessage.variable){
        case 'T':
          trigCount++;
          checkForMiss();
          triggerBoth();
          break;
        case 'S': // Receiving a new Cerebro S/N
          EEPROM.update(SERIAL_NUMBER_ADDRESS, integerMessage.value);
          reportVersion();
          Serial.print("New Serial:");
          Serial.println(integerMessage.value);
          Serial.println("Restarting...");
          wdt_enable(WDTO_15MS);  // turn on the WatchDog timer
          while(1){}              // do nothing and wait for the reset
          break;
        case 'l': // Receiving a new left setpoint
          // Serial.print("\nTriggering Left @ ");Serial.println(integerMessage.value);
          triggerOne(integerMessage.value,&left);
          break;
        case 'r': // Receiving a new right setpoint
          // Serial.print("\nTriggering Right @");Serial.println(integerMessage.value);
          triggerOne(integerMessage.value,&right);
          break;
        case 'M':
          checkForMiss();
          printMissed();
          break;
      }
    }
    else{
      Serial.println("Unexpected Data size received");
    }
  }
  // LowPower.idle(SLEEP_FOREVER,ADC_OFF,TIMER4_OFF,TIMER3_OFF,TIMER1_OFF,TIMER0_ON,SPI_ON,USART1_OFF,TWI_OFF,USB_OFF);
}

void sendACK(){
  if (radio.ACKRequested()){
    radio.sendACK();
  }
}
 
void checkForMiss(){
  msgCount++;
  if (trigCount%batteryUpdateFrequency==0){
    reportBatteryFlag = true;
  }
  if (msgCount!=integerMessage.value){
    for (msgCount; msgCount<integerMessage.value; msgCount++){
      EEPROM.put(MISSING_ARRAY_ADDRESS + 2*missedCount,msgCount);
      missedCount++;
    }
  }
}

void combinedTest(){
  triggerOne(left.setPoint,&left);
  delay(1000);
  triggerBoth();
  delay(1000);
  triggerOne(right.setPoint,&right);
  delay(5000);
  Serial.println();
}

void isolationTest(){
  // Serial.print("Before left: ");
  feedbackReadings();
  triggerOne(left.setPoint,&left);
  delay(1000);
  // Serial.print("Before right: ");
  feedbackReadings();
  triggerOne(right.setPoint,&right);
}

void feedbackReadings(){
  Serial.print(analogRead(left.analogPin));
  Serial.print(", ");
  Serial.println(analogRead(right.analogPin));
}

void printMissed(){
  reportBattery();
  delay(200);
  uint16_t missed;
  // Serial.print("missed,");Serial.println(missedCount);
  integerMessage.variable = 'M';
  integerMessage.value = missedCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&integerMessage), sizeof(integerMessage),3,250)){
    // Serial.print("Sent");Serial.println(missedCount);
  }
  else{
    // Serial.println("Sending again");
  }
  integerMessage.variable = 'm';
  for (int i = 0; i <missedCount; i++ ){
    EEPROM.get(MISSING_ARRAY_ADDRESS+2*i,missed);
    integerMessage.value = (int)missed;
    if (radio.sendWithRetry(BASESTATION, (const void*)(&integerMessage), sizeof(integerMessage),3,250)){
      // Serial.println(missed);
    }
    else{
      // Serial.println("Sending again");
    }
  }
  // Serial.println("done");
}

void reportVersion(){
  EEPROM.get(SERIAL_NUMBER_ADDRESS,cerebroInfo.serialNumber);
  cerebroInfo.firmware = VERSION;
  cerebroInfo.msgCount = msgCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&cerebroInfo), sizeof(cerebroInfo),4)){
    // Serial.println("Version sent successfully");
  }
  else{
    // Serial.println("Error sending Version");
  }
}

void reportPower(){
  diodePwrs.lSetPoint = left.setPoint;
  diodePwrs.rSetPoint = right.setPoint;
  diodePwrs.msgCount = msgCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&diodePwrs), sizeof(diodePwrs),4)){
    // Serial.println("Diode Powers sent successfully");
  }
  else{
    // Serial.println("Error sending Diode Powers");
  }
}

void reportWaveform(){
  waveform.msgCount = msgCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&waveform), sizeof(waveform),4)){
    // Serial.println("Waveform sent successfully");
  }
  else{
    // Serial.println("Error sending Waveform");
  }
}

void reportBattery(){
  integerMessage.variable = 'B';
  integerMessage.value = lipo.soc();
  integerMessage.msgCount = msgCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&integerMessage), sizeof(integerMessage),4)){
    //
  }
  else{
    // Serial.println("Battery info send fail");
  }
}

void printChannel(){
  Serial.print("Channel: ");
  Serial.println(channel);
}
uint8_t parseData(){
  char msgData[5];
  Serial.readBytesUntil('\n',msgData,5);
  char* msgPointer;
  msgPointer = strtok(msgData,",");
  return atoi(msgPointer);
}