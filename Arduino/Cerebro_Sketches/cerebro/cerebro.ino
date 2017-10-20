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
const byte version = 74;
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
#include <EEPROM.h>
#include <SparkFunBQ27441.h>  //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <LaserDiode.h>
#include <Radio.h>
#include <Adafruit_SleepyDog.h>


#define SERIAL_NUMBER_ADDRESS 0
#define WAVEFORM_ADDRESS 1
#define LEFT_SETPOINT_ADDRESS 13
#define RIGHT_SETPOINT_ADDRESS 15
#define MISSING_ARRAY_ADDRESS 17

WaveformData waveform;
IntegerPayload radioMessage;
Status currentInfo;
Battery battery;
Feedback diodeStats;

int meterVal = 0;
int powerMeter = A3;
const byte indicatorLED = A5; //32u4 pin 41

LaserDiode left(&DDRB,&PORTB,0,A4);
LaserDiode right(&DDRD,&PORTD,2,A2);

Radio radio(7,1);
unsigned int msgCount = 0;
unsigned int missedCount = 0;
byte batteryUpdateFrequency = 20;

//---------function prototypes---------//
int triggerEvent(unsigned int desiredPower, LaserDiode* thediode, bool useFeedback=true );

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

  // Indicator LED
  pinMode(indicatorLED,OUTPUT);
  digitalWrite(indicatorLED,HIGH);
  delay(1000);
  digitalWrite(indicatorLED,LOW);
  delay(1000);  
  digitalWrite(indicatorLED,HIGH);
  delay(1000);
  digitalWrite(indicatorLED,LOW);  

  //*** Battery Monitor ***//
  setupBQ27441();
  
  radio.radioSetup(CEREBRO,true); //nodeID, autopower on;
  char readyMessage[22] = "\nCerebro Connected!\n";
  byte buffLen=strlen(readyMessage);
  radio.send(BASESTATION, readyMessage, buffLen);  
  sendInfo();
}

void loop() {
  //check for any received packets
  if (radio.receiveDone()){
    // Serial.print("data length = ");Serial.println(radio.DATALEN);    
    if (radio.DATALEN==1){ //received a command or a request for data
      sendACK();
      switch (radio.DATA[0]){
        case 'B':
          reportBattery();break;
        case 'I':    
          printMissed();
          delay(100);
          msgCount = 0;
          missedCount = 0;       
          sendInfo();break;
        case 'i':
          isolationTest();break;
        case 'c':
          combinedTest();break;
        case 'M':
          printMissed();break;
        default:
          Serial.println("Command not recognized");break;
      }
    }
    else if (radio.DATALEN == sizeof(waveform)){ //received a waveform data 
      msgCount++;
      waveform = *(WaveformData*)radio.DATA;  //update waveform
      EEPROM.put(WAVEFORM_ADDRESS,waveform);  //save new waveform to memory
      delay(1000);
      sendInfo();
    }
    else if (radio.DATALEN == sizeof(radioMessage)){ //received a variable update
      sendACK();
      radioMessage = *(IntegerPayload*)radio.DATA;
      switch (radioMessage.variable){
        case 'T':
          checkForMiss();      
          triggerBoth();
          break;
        case 'S': // Receiving a new Cerebro S/N
          EEPROM.update(SERIAL_NUMBER_ADDRESS, radioMessage.value);
          break;
        case 'L': // Receiving a new left setpoint
          left.setPoint = radioMessage.value;                   //update setpoint 
          EEPROM.put(LEFT_SETPOINT_ADDRESS,left.setPoint);     //save new setpoint to memory     
          break;
        case 'R': // Receiving a new right setpoint
          right.setPoint = radioMessage.value;                  //update setpoint 
          EEPROM.put(RIGHT_SETPOINT_ADDRESS,right.setPoint);    //save new setpoint to memory
          break;
        case 'l': // Receiving a new left setpoint
          Serial.print("\nTriggering Left @ ");Serial.println(radioMessage.value);
          triggerEvent(radioMessage.value,&left,true);
          break;
        case 'r': // Receiving a new right setpoint
          Serial.print("\nTriggering Right @");Serial.println(radioMessage.value);        
          triggerEvent(radioMessage.value,&right,true);
          break;
      }
    }
    else{
      Serial.println("Unexpected Data size received");
    }
  }

  if (Serial.available()){
    bool isLeft = false;
    bool isRight = false;
    char whichDiode = Serial.read();
    if (whichDiode=='l'){
      isLeft = true;
      Serial.println("left");
    }
    else if (whichDiode == 'r'){
      isRight = true;
    }
    char msg[20] = {};
    byte msgIndex = 0;
    while(Serial.available()){ 
      msg[msgIndex] = Serial.read()-48;
      msgIndex++;      
    }
    unsigned long powers[4] = {1, 10, 100, 1000};
    unsigned int integerVal = 0;
    for (int i = 0; i < msgIndex; i++) {
      integerVal = integerVal + msg[i] * powers[msgIndex-1-i];
    }
    if (isLeft){
      triggerEvent(integerVal,&left,true);
    }
    else if (isRight){
      triggerEvent(integerVal,&right,true);
    }
  }
}
void sendInfo(){
  currentInfo.serialNumber = EEPROM.read(SERIAL_NUMBER_ADDRESS);
  currentInfo.firmware = version;
  currentInfo.waveform = waveform;
  currentInfo.lSetPoint = left.setPoint;
  currentInfo.rSetPoint = right.setPoint;
  currentInfo.battery = lipo.soc();
  if (radio.sendWithRetry(1, (const void*)(&currentInfo), sizeof(currentInfo))){
    Serial.println("data sent successfully");
  }
  else{
    Serial.println("Info failure send fail");
  }
}

void printInfo(){
  Serial.print("\nSerial Number: ");Serial.println(currentInfo.serialNumber);
  Serial.print("Firmware Version: ");Serial.println(currentInfo.firmware); 
  Serial.print("Left set Point: ");Serial.println(currentInfo.lSetPoint);
  Serial.print("Right set Point: ");Serial.println(currentInfo.rSetPoint);   
  Serial.print("Start Delay: "); Serial.println(currentInfo.waveform.startDelay);       
  Serial.print("On Time: "); Serial.println(currentInfo.waveform.onTime);     
  Serial.print("Off Time: "); Serial.println(currentInfo.waveform.offTime);      
  Serial.print("Train Duration: "); Serial.println(currentInfo.waveform.trainDur);
  Serial.print("Ramp Duration: "); Serial.println(currentInfo.waveform.rampDur);  
}

void sendACK(){
  if (radio.ACKRequested()){
    radio.sendACK();
  }
}

void checkForMiss(){
  msgCount++;
  if (msgCount!=radioMessage.value){
    for (msgCount; msgCount<radioMessage.value; msgCount++){
      EEPROM.put(MISSING_ARRAY_ADDRESS + 2*missedCount,msgCount);
      missedCount++;
    }
  }
}

void combinedTest(){
  triggerEvent(left.setPoint,&left,true);
  delay(1000);
  triggerBoth();
  delay(1000);
  triggerEvent(right.setPoint,&right,true);
  delay(5000);  
  Serial.println();
}

void isolationTest(){
  Serial.print("Before left: ");    
  feedbackReadings();
  triggerEvent(left.setPoint,&left,true);
  delay(1000);  
  Serial.print("Before right: ");  
  feedbackReadings();  
  triggerEvent(right.setPoint,&right,true);
}

void feedbackReadings(){ 
  Serial.print(analogRead(left.analogPin));
  Serial.print(", ");
  Serial.println(analogRead(right.analogPin));
}

void calibrate(){
  int tryPower  = 1005;
  while ( meterVal <96){
    tryPower++;
    meterVal =  triggerEvent(tryPower,&left,true);
    Serial.print("Try: ");
    Serial.print(tryPower);
    Serial.print(", Result: ");
    Serial.println(meterVal);
    delay(2000);
  }
  Serial.println("Done!");
  Serial.print(tryPower);
}

void pauseUntilCommand(){
  while(!Serial.available()){
    //wait
  }
  while(Serial.available()){
    Serial.read();
  }
}

void reportBattery(){
  // Read battery stats from the BQ27441-G1A
  battery.soc = lipo.soc();
  battery.volts = lipo.voltage();
  battery.capacity = lipo.capacity(REMAIN);
  if (radio.sendWithRetry(BASESTATION, (const void*)(&battery), sizeof(battery))){
    Serial.println("battery info sent successfully");
  }
  else{
    Serial.println("battery info send fail");
  }
}

void setupBQ27441(void)
{
  if (!lipo.begin()) // begin() will return true if communication is successful
  {
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    blinkError();
  }
  Serial.println("Connected to BQ27441!");
  lipo.setCapacity(400);
}

void blinkError(){
  while(1){
    digitalWrite(indicatorLED,HIGH);
    delay(1000);
    digitalWrite(indicatorLED,LOW);
    delay(1000);
  }
}

void printBattery(){
  // Now print out those values:
  String toPrint = String(battery.soc) + "% | ";
  toPrint += String(battery.volts) + " mV | ";
  toPrint += String(battery.capacity) + " mAh remaining ";

  Serial.println(toPrint);
}

void printMissed(){
  reportBattery();
  delay(500);
  unsigned int missed;
  Serial.print("missed,");Serial.println(missedCount);
  radioMessage.variable = 'M'; 
  radioMessage.value = missedCount;
  if (radio.sendWithRetry(BASESTATION, (const void*)(&radioMessage), sizeof(radioMessage),3,250)){
    Serial.print("Sent");Serial.println(missedCount);
  }
  else{
    Serial.println("Sending again");
  }  
  radioMessage.variable = 'm';
  for (int i = 0; i <missedCount; i++ ){
    EEPROM.get(MISSING_ARRAY_ADDRESS+2*i,missed);
    radioMessage.value = (int)missed;
    if (radio.sendWithRetry(BASESTATION, (const void*)(&radioMessage), sizeof(radioMessage),3,250)){
      Serial.println(missed);
    }
    else{
      Serial.println("Sending again");
    }    
  }
  Serial.println("done");
}