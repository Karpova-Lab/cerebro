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
const byte version = 84;
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
#include <LaserDiode.h>
#include <SparkFunBQ27441.h>      //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <Radio.h>                //https://github.com/LowPowerLab/RFM69
// #include <Adafruit_SleepyDog.h>   //https://github.com/adafruit/Adafruit_SleepyDog
#include <LowPower.h>             //https://github.com/LowPowerLab/LowPower

#define SERIAL_NUMBER_ADDRESS 0
#define WAVEFORM_ADDRESS 1
#define LEFT_SETPOINT_ADDRESS 13
#define RIGHT_SETPOINT_ADDRESS 15
#define MISSING_ARRAY_ADDRESS 17

WaveformData waveform;
IntegerPayload radioMessage;
Status currentInfo;
Feedback diodeStats;

int meterVal = 0;

const byte indicatorLED = A0; //32u4 pin 36

LaserDiode left(&DDRB,&PORTB,0,A4);
LaserDiode right(&DDRD,&PORTD,2,A2);

Radio radio(7,1); //slave select pin, interrupt pin
unsigned int msgCount = 0;
unsigned int missedCount = 0;
unsigned int trigCount = 0;
byte batteryUpdateFrequency = 10;
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

  // Indicator LED
  pinMode(indicatorLED,OUTPUT); 
  digitalWrite(indicatorLED,HIGH);

  //*** Battery Monitor ***//
  if (!lipo.begin()){
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while(1){ //blink error
      digitalWrite(indicatorLED,HIGH);
      delay(1000);
      digitalWrite(indicatorLED,LOW);
      delay(1000);
    }
  }  
  Serial.println("Connected to BQ27441!");
  lipo.setCapacity(400);
  while(lipo.soc()==0){
    delay(1);
    //wait;
  }

  //*** Radio ***//  
  radio.radioSetup(CEREBRO,true); //nodeID, autopower on;
  radioMessage.variable = 'Y'; 
  radioMessage.value = millis();  
  if (radio.sendWithRetry(BASESTATION, (const void*)(&radioMessage), sizeof(radioMessage),3,250)){
    Serial.print("Connected to Base Station");
  }
  else{
    Serial.println("Failed to Connect to Base Station");
  }  
  sendInfo();
  digitalWrite(indicatorLED,LOW);  
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
      sendACK();
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
          trigCount++;
          checkForMiss();      
          triggerBoth();
          break;
        case 'S': // Receiving a new Cerebro S/N
          EEPROM.update(SERIAL_NUMBER_ADDRESS, radioMessage.value);
          sendInfo();          
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
          triggerOne(radioMessage.value,&left);
          break;
        case 'r': // Receiving a new right setpoint
          Serial.print("\nTriggering Right @");Serial.println(radioMessage.value);        
          triggerOne(radioMessage.value,&right);
          break;
      }
    }
    else{
      Serial.println("Unexpected Data size received");
    }
  }
  LowPower.idle(SLEEP_FOREVER,ADC_OFF,TIMER4_OFF,TIMER3_OFF,TIMER1_OFF,TIMER0_ON,SPI_ON,USART1_OFF,TWI_OFF,USB_OFF);

}
void sendInfo(){
  currentInfo.serialNumber = EEPROM.read(SERIAL_NUMBER_ADDRESS);
  currentInfo.firmware = version;
  currentInfo.waveform = waveform;
  currentInfo.lSetPoint = left.setPoint;
  currentInfo.rSetPoint = right.setPoint;
  currentInfo.battery = lipo.soc();
  if (radio.sendWithRetry(BASESTATION, (const void*)(&currentInfo), sizeof(currentInfo))){
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
  if (trigCount%batteryUpdateFrequency==0){
    reportBatteryFlag = true;
  }
  if (msgCount!=radioMessage.value){
    for (msgCount; msgCount<radioMessage.value; msgCount++){
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
  Serial.print("Before left: ");    
  feedbackReadings();
  triggerOne(left.setPoint,&left);
  delay(1000);  
  Serial.print("Before right: ");  
  feedbackReadings();  
  triggerOne(right.setPoint,&right);
}

void feedbackReadings(){ 
  Serial.print(analogRead(left.analogPin));
  Serial.print(", ");
  Serial.println(analogRead(right.analogPin));
}

void reportBattery(){
  radioMessage.variable = 'B'; 
  radioMessage.value = lipo.soc();
  if (radio.sendWithRetry(BASESTATION, (const void*)(&radioMessage), sizeof(radioMessage))){
    //
  }
  else{
    Serial.println("battery info send fail");
  }
}

void printMissed(){
  reportBattery();
  delay(200);
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