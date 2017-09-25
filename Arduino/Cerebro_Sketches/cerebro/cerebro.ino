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
#include <EEPROM.h>
#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SparkFunBQ27441.h>  //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library

//*********************************************************************************************
#define NODEID        12    //must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     100  //the same on all nodes that talk to each other (range up to 255)
#define GATEWAYID     1
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_915MHZ
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level (ATC_RSSI)
#define ATC_RSSI      -80
RFM69_ATC radio(7, 1,true, digitalPinToInterrupt(1)); 
//*********************************************************************************************

#define LEFT 0
#define RIGHT 1

const byte indicatorLED = A5; //32u4 pin 41

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
bool isSettled = false;
unsigned int tempPower = 0;
byte vectorIndex = 0;
unsigned int powerLevel;


class LaserDiode{
public:
  volatile uint8_t *slaveOutputReg;
  uint8_t slaveSelectPin;
  uint8_t analogPin;
  bool isMaxed;
  int16_t DAClevel;  

  LaserDiode(volatile uint8_t *slaveDirReg, volatile uint8_t *_slaveOutputReg, uint8_t _slavePin, uint8_t _analogPin);
  bool off();
  void sendDAC(unsigned int value);
  void feedback(int setPoint);
  void fade();
};

LaserDiode::LaserDiode(volatile uint8_t *slaveDirReg,volatile uint8_t *_slaveOutputReg, uint8_t _slavePin, uint8_t _analogPin){
  slaveOutputReg = _slaveOutputReg;
  slaveSelectPin = _slavePin;
  analogPin = _analogPin;
  isMaxed = false;
  *slaveDirReg |= (1<<slaveSelectPin);       //set slave select pin as output
  *slaveOutputReg |= (1<<slaveSelectPin);  //Set slave select HIGH (LOW selects the chip)    
}

bool LaserDiode::off(){
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  *slaveOutputReg &= ~(1<<slaveSelectPin); //latch low  
  SPI.transfer(64);                        //Power down command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(0);                  //shift high byte
  SPI.transfer(0);            //shift low byte
  *slaveOutputReg |= (1<<slaveSelectPin);  //latch high
  if (DAClevel==4095){
    isMaxed = true;
  }
  DAClevel = 0;
  SPI.endTransaction();   
  return false;
}

void LaserDiode::sendDAC(unsigned int value) {
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  *slaveOutputReg &= ~(1<<slaveSelectPin); //latch low  
  SPI.transfer(48);                        //Write to and Update (Power up) DAC Register command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(value>>4);                  //shift high byte
  SPI.transfer(value<<4 & 255);            //shift low byte
  *slaveOutputReg |= (1<<slaveSelectPin);  //latch high
  SPI.endTransaction();
}

void LaserDiode::feedback(int setPoint){
  int photocellVoltage = analogRead(analogPin);
  photocellVoltage = analogRead(analogPin);
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

int meterVal = 0;
int powerMeter = A2;

//---------function prototypes---------//
int triggerEvent(unsigned int desiredPower, LaserDiode* thediode, bool useFeedback=true );

bool active = false;

LaserDiode right(&DDRD,&PORTD,2,A3);
int rightSetPoint = 114;
LaserDiode left(&DDRB,&PORTB,0,A4);
int leftSetPoint = 64;


void setup() {
  SPI.begin();
  Serial.begin(115200);  

  // Laser Diodes
  right.off();
  left.off();

  // Indicator LED
  pinMode(indicatorLED,OUTPUT);

  //*** Radio ***//
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.enableAutoPower(ATC_RSSI);  
  radio.encrypt(null);
  radio.writeReg(0x03, 0x00);  //REG_BITRATEMSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x04, 0x6B);  //REG_BITRATELSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x19, 0x40);  //REG_RXBW: 500kHz
  radio.writeReg(0x1A, 0x80);  //REG_AFCBW: 500kHz
  radio.writeReg(0x05, 0x13);  //REG_FDEVMSB: 300khz (0x1333)
  radio.writeReg(0x06, 0x33);  //REG_FDEVLSB: 300khz (0x1333)
  radio.writeReg(0x29, 240);   //set REG_RSSITHRESH to -120dBm

  //*** Battery Monitor ***//
  setupBQ27441();
  char readyMessage[10] = "Cerebro On";
  byte buffLen=strlen(readyMessage);
  radio.send(GATEWAYID, readyMessage, buffLen);
  
}

void loop() {
  //check for any received packets
  if (radio.receiveDone()){
    if (radio.ACKRequested()){
      radio.sendACK();
      Serial.print(" - ACK sent ");
    }
    char receivedMsg = radio.DATA[0];
    if (receivedMsg=='T'){
      triggerBoth(leftSetPoint,rightSetPoint);
    }
    if (receivedMsg=='B'){
      reportBattery();
    }
    Serial.print("received: ");
    Serial.println(receivedMsg);
  }
  // combinedTest();
  
  // if (Serial.available()){
  //   char msg = Serial.read();
  //   testDAC(msg);
  // }

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
  if (msg=='1'){
    leftSetPoint+=50;
    Serial.println(leftSetPoint);  
    triggerEvent(leftSetPoint,&left,true);      
  }
  else if (msg=='2'){
    leftSetPoint+=10;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
  else if (msg=='3'){
    leftSetPoint+=5;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
  else if (msg=='4'){
    leftSetPoint++;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
  if (msg=='0'){
    leftSetPoint-=50;
    Serial.println(leftSetPoint);  
    triggerEvent(leftSetPoint,&left,true);      
  }
  else if (msg=='9'){
    leftSetPoint-=10;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
  else if (msg=='8'){
    leftSetPoint-=5;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
  else if (msg=='8'){
    leftSetPoint--;
    Serial.println(leftSetPoint);    
    triggerEvent(leftSetPoint,&left,true);  
  }
}
void combinedTest(){
  triggerEvent(leftSetPoint,&left,true);
  delay(1000);
  triggerBoth(leftSetPoint,rightSetPoint);
  delay(1000);
  triggerEvent(rightSetPoint,&right,true);
  delay(5000);  
  Serial.println();
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
    unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
    unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
    int power = lipo.power(); // Read average power draw (mW)
    int health = lipo.soh(); // Read state-of-health (%)
  
    // Now print out those values:
    String toPrint = String(soc) + "% | ";
    toPrint += String(volts) + " mV | ";
    toPrint += String(current) + " mA | ";
    toPrint += String(capacity) + " / ";
    toPrint += String(fullCapacity) + " mAh | ";
    toPrint += String(power) + " mW | ";
    toPrint += String(health) + "%";
    
    Serial.println(toPrint);

    char buff[65];
    toPrint.toCharArray(buff,65);
    byte buffLen=strlen(buff);
    radio.send(1, buff, buffLen);
}

void setupBQ27441(void)
{
  // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
  // connected and communicating.
  if (!lipo.begin()) // begin() will return true if communication is successful
  {
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("  Check wiring and try again.");
    Serial.println("  (Battery must be plugged into Battery Babysitter!)");
    while (1) ;
  }
  Serial.println("Connected to BQ27441!");
  
  // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
  // of your battery.
  lipo.setCapacity(400);
}