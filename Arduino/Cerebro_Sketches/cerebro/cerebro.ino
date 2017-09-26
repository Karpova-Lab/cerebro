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
#include <EEPROM.h>
#include <SparkFunBQ27441.h>  //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <LaserDiode.h>
#include <Radio.h>

const char string_0[] PROGMEM = "Start_Delay,";
const char string_1[] PROGMEM = "On_Time,";
const char string_2[] PROGMEM = "Off_Time,";
const char string_3[] PROGMEM = "Train_Duration,";
const char string_4[] PROGMEM = "Ramp_Duration,";

// Then set up a table to refer to your strings.
const char* const parameterLabels[] PROGMEM = {string_0, string_1, string_2, string_3, string_4};

typedef struct {
  unsigned int startDelay;
  unsigned int onTime;
  unsigned int offTime;
  unsigned int trainDur;
  unsigned int rampDur;
} Payload;
Payload waveform;

int meterVal = 0;
int powerMeter = A2;
const byte indicatorLED = A5; //32u4 pin 41
LaserDiode right(&DDRD,&PORTD,2,A3,114);
LaserDiode left(&DDRB,&PORTB,0,A4,64);
Radio radio;

//---------function prototypes---------//
int triggerEvent(unsigned int desiredPower, LaserDiode* thediode, unsigned int rampDur, bool useFeedback=true );

void setup() {
  SPI.begin();
  Serial.begin(115200);  

  // Laser Diodes
  right.off();
  left.off();

  // Indicator LED
  pinMode(indicatorLED,OUTPUT);

  //*** Battery Monitor ***//
  setupBQ27441();
  
  radio.radioSetup();
  char readyMessage[15] = "\nCerebro On\n";
  byte buffLen=strlen(readyMessage);
  radio.send(GATEWAYID, readyMessage, buffLen);  
}

void loop() {
  //check for any received packets
  if (radio.receiveDone()){
    if (radio.DATALEN == sizeof(Payload)){
      updateWaveform();
    }
    else{
      if (radio.ACKRequested()){
        radio.sendACK();
        Serial.println("ACK sent");
      }
      char receivedMsg = radio.DATA[0];
      if (receivedMsg=='T'){
        triggerBoth();
      }
      else if (receivedMsg=='B'){
        reportBattery();
      }
      Serial.print("received: ");
      Serial.println(receivedMsg);
    }
  }
  // triggerEvent(leftSetPoint,&left,2000,true);  
  // delay(8000);
  // triggerEvent(leftSetPoint,&left,0,true);  
  // delay(8000);
  // combinedTest();
  
  // if (Serial.available()){
  //   char msg = Serial.read();
  //   testDAC(msg);
  // }
}

void updateWaveform(){
  if (radio.ACKRequested()){
    radio.sendACK();
    Serial.println("ACK sent");
  }
  waveform = *(Payload*)radio.DATA;
  Serial.print("Start Delay = ");
  Serial.println(waveform.startDelay);
  Serial.print("On = ");
  Serial.println(waveform.onTime);
  Serial.print("Off = ");
  Serial.println(waveform.offTime);
  Serial.print("Train = ");
  Serial.println(waveform.trainDur);
  Serial.print("Ramp = ");
  Serial.println(waveform.rampDur);
}
void testDAC(char msg){
  if (msg=='1'){
    left.sendDAC(200);
  }
  if (msg=='2'){
    left.sendDAC(4000);
  }
  if (msg=='3'){
    right.sendDAC(200);
  }
  if (msg=='4'){
    right.sendDAC(4000);
  }      
}
void leftTune(char msg){
  // switch (msg){
  //   case '1':
  //     leftSetPoint+=50;
  //     Serial.println(leftSetPoint);  
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '2':
  //     leftSetPoint+=10;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '3':
  //     leftSetPoint+=5;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '4':
  //     leftSetPoint++;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);   
  //     break;
  //   case '7':
  //     leftSetPoint--;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '8':
  //     leftSetPoint-=5;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '9':
  //     leftSetPoint-=10;
  //     Serial.println(leftSetPoint);    
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;
  //   case '0':
  //     leftSetPoint-=50;
  //     Serial.println(leftSetPoint);  
  //     triggerEvent(leftSetPoint,&left,true);  
  //     break;    
  // }
}

void combinedTest(){
  // triggerEvent(leftSetPoint,&left,true);
  // delay(1000);
  // triggerBoth(leftSetPoint,rightSetPoint);
  // delay(1000);
  // triggerEvent(rightSetPoint,&right,true);
  // delay(5000);  
  // Serial.println();
}

void isolationTest(){
  // delay(5000);
  // Serial.print("Before left: ");    
  // feedbackReadings();
  // triggerEvent(951,&left,true);
  // delay(1000);  
  // Serial.print("Before right: ");  
  // feedbackReadings();  
  // triggerEvent(74,&right,true);
}

void feedbackReadings(){ 
  // Serial.print(analogRead(left.analogPin));
  // Serial.print(", ");
  // Serial.println(analogRead(right.analogPin));
}

void calibrate(){
  // // int a = EEPROM.read(0)<<8;// high byte
  // // int b = EEPROM.read(1);
  // // Serial.print("last Value:");
  // // Serial.println(a|b,DEC);
  // // delay(5000);
  // int tryPower  = 1005;
  // while ( meterVal <96){
  //   tryPower++;
  //   meterVal =  triggerEvent(tryPower,&left,true);
  //   Serial.print("Try: ");
  //   Serial.print(tryPower);
  //   Serial.print(", Result: ");
  //   Serial.println(meterVal);
  //   delay(2000);
  // }
  // EEPROM.write(0,tryPower>>8);// high byte
  // EEPROM.write(1,tryPower&255);//low byte
  // Serial.println("Done!");
  // Serial.print(tryPower);
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
    unsigned int soc = lipo.soc();  // Read state-of-charge (%)
    unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
    int current = lipo.current(AVG); // Read average current (mA)
    unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  
    // Now print out those values:
    String toPrint = String(soc) + "% | ";
    toPrint += String(volts) + " mV | ";
    toPrint += String(current) + " mA | ";
    toPrint += String(capacity) + " mAh remaining ";

    Serial.println(toPrint);

    char buff[65];
    toPrint.toCharArray(buff,65);
    byte buffLen=strlen(buff);
    radio.send(1, buff, buffLen);
}

void setupBQ27441(void)
{
  if (!lipo.begin()) // begin() will return true if communication is successful
  {
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while(1){
      digitalWrite(indicatorLED,HIGH);
      delay(1000);
      digitalWrite(indicatorLED,LOW);
      delay(1000);
    }
  }
  Serial.println("Connected to BQ27441!");
  lipo.setCapacity(400);
}