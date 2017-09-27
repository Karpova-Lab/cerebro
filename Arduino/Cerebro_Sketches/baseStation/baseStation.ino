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

#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NODEID        1    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_915MHZ
//*********************************************************************************************
#define BR_300KBPS          //run radio at max rate of 300kbps!
#define LED           13 

RFM69 radio(8, 7,true, digitalPinToInterrupt(7));  //slave select,interrupt pin, isRFM69HW, interruptNum

typedef struct {
  unsigned int startDelay;
  unsigned int onTime;
  unsigned int offTime;
  unsigned int trainDur;
  unsigned int rampDur;
} WaveformData;
WaveformData waveform;

typedef struct {
  unsigned int variable;
  unsigned int value;
} IntegerPayload;
IntegerPayload radioMessage;

typedef struct {
  byte  serialNumber;
  byte  firmware;
  WaveformData waveform;
  unsigned int lSetPoint;
  unsigned int rSetPoint;
} Status;
Status currentInfo;

char csValues[5][6];  //2D array, first dimension 0-number of parameters, 2nd dimension holds up to 5 Ascii characters that represent digits 
                                  //of a integer. the final spot in the second dimension holds the number of digits read in for that parameter

void setup() {
  Serial.begin(115200);
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
  radio.encrypt(null);  
  radio.promiscuous(false);
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);

#ifdef BR_300KBPS
  radio.writeReg(0x03, 0x00);  //REG_BITRATEMSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x04, 0x6B);  //REG_BITRATELSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x19, 0x40);  //REG_RXBW: 500kHz
  radio.writeReg(0x1A, 0x80);  //REG_AFCBW: 500kHz
  radio.writeReg(0x05, 0x13);  //REG_FDEVMSB: 300khz (0x1333)
  radio.writeReg(0x06, 0x33);  //REG_FDEVLSB: 300khz (0x1333)
  radio.writeReg(0x29, 240);   //set REG_RSSITHRESH to -120dBm
#endif

  pinMode(LED, OUTPUT);

  waveform.startDelay=0;
  waveform.onTime = 100;
  waveform.offTime = 150;
  waveform.trainDur = 2000;
  waveform.rampDur = 0;

}
unsigned int timeSent = 0;

void loop() {
  if (Serial.available()){
    char msg = Serial.read();
    if (msg=='W'){
      readMsg();
      parseWaveform();
      sendWaveform();
    }
    else if (msg=='S' || msg == 'L' || msg == 'R'){
      readMsg();
      radioMessage.variable = msg;
      radioMessage.value = convertAsciiValsToIntegers(0);
      if (radio.sendWithRetry(12, (const void*)(&radioMessage), sizeof(radioMessage))){
        Serial.println("data received");        
      }
      else{
        Serial.println("data send fail");
      }
    }
    else{
      timeSent = micros();  
      if (radio.sendWithRetry(12, &msg, 1, 0)){  // 0 = only 1 attempt, no retries
        timeSent = micros()-timeSent;
        Serial.print("\n[");Serial.print(timeSent);Serial.print("] ");
      }
      else{
         Serial.println("ACK not received");
      }
    }
    
  }
  if (radio.receiveDone()){
    if (radio.DATALEN == sizeof(currentInfo)){ //received a waveform data 
      radio.sendACK();
      currentInfo = *(Status*)radio.DATA;  //update waveform
      printInfo();      
    }
    else{
      for (byte i = 0; i < radio.DATALEN; i++){
        Serial.print((char)radio.DATA[i]); 
      }
      if (radio.ACKRequested()){
        byte theNodeID = radio.SENDERID;
        radio.sendACK();
        Serial.print(" - ACK sent.");
      }
      Blink(LED,50);
    }
  }
}

void printInfo(){
  Serial.print("\nSerial Number: ");Serial.println(currentInfo.serialNumber);
  Serial.print("Firmware Version: ");Serial.println(currentInfo.serialNumber); 
  Serial.print("Left set Point: ");Serial.println(currentInfo.lSetPoint);
  Serial.print("Right set Point: ");Serial.println(currentInfo.rSetPoint);   
  Serial.print("Start Delay: "); Serial.println(currentInfo.waveform.startDelay);       
  Serial.print("On Time: "); Serial.println(currentInfo.waveform.onTime);     
  Serial.print("Off Time: "); Serial.println(currentInfo.waveform.offTime);      
  Serial.print("Train Duration: "); Serial.println(currentInfo.waveform.trainDur);
  Serial.print("Ramp Duration: "); Serial.println(currentInfo.waveform.rampDur);  
}

void Blink(byte PIN, int msDelay)
{
  digitalWrite(PIN,HIGH);
  delay(msDelay);
  digitalWrite(PIN,LOW);
}

void readMsg(){
  byte digitIndex = 0;
  byte valueIndex = -1;
  while (Serial.available()) {
    char msg = Serial.read();
    if (msg == ',') {
      digitIndex = 0;
      valueIndex++;
    }
    else {
      csValues[valueIndex][digitIndex] = msg-48;// convert the ascii character to the number it represents and store it
      digitIndex++;
      csValues[valueIndex][5] = digitIndex; //the number of digits that have been read in
      delay(20);
    }
  }
}

unsigned int convertAsciiValsToIntegers(byte whichParameter){
  unsigned long powers[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
  unsigned int integerVal = 0;
  byte numDigits = csValues[whichParameter][5];
  for (int i = 0; i < numDigits; i++) {
    integerVal = integerVal + csValues[whichParameter][i] * powers[numDigits - i - 1];
  }
  return integerVal;
}
void parseWaveform(){
  waveform.startDelay = convertAsciiValsToIntegers(0);
  waveform.onTime = convertAsciiValsToIntegers(1);
  waveform.offTime = convertAsciiValsToIntegers(2);
  waveform.trainDur = convertAsciiValsToIntegers(3);
  waveform.rampDur = convertAsciiValsToIntegers(4);
}
void sendWaveform(){
  if (radio.sendWithRetry(12, (const void*)(&waveform), sizeof(waveform))){
    Serial.println("waveform received");        
  }
  else{
    Serial.println("waveform send fail");
  }
}