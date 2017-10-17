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

#include <Radio.h>
#include <SPI.h>

const int LED = 13;
const int triggerPin = 5;
const int stopPin = 6;

Radio radio(8,7); //slave select pin, interrupt pin
WaveformData waveform;
IntegerPayload radioMessage;
Status currentInfo;
Battery battery;
Feedback diodeStats;
char csValues[5][8];  //2D array, first dimension 0-number of parameters, 2nd dimension holds up to 5 Ascii characters that represent digits 
                      //of a integer. the final spot in the second dimension holds the number of digits read in for that parameter
unsigned int timeSent = 0;
unsigned int msgCount = 0;
unsigned long startTime = 0;

void setup() {
  Serial1.begin(57600);
  delay(10);
  pinMode(LED, OUTPUT);
  radio.radioSetup(1,false); //nodeID, autopower on;
  pinMode(triggerPin,INPUT);
  pinMode(stopPin,INPUT);
  startTime = millis();  
}

void loop() {
  ////////////Receive Message From Bcontrol
  if (digitalRead(triggerPin)) {
    while(digitalRead(triggerPin)){} //wait until signal goes low
    triggerFromTTL();
  }
  //if we read a high signal on pin 6, send a stop command to cerebro
  if (digitalRead(stopPin)) {
    while(digitalRead(stopPin)){}       //wait until signal goes low
    stopFromTTL();
  }

  ///////////Receive Message From Xavier//////////////////
  if (Serial1.available()){
    digitalWrite(LED,HIGH);
    char msg = Serial1.read();
    // Serial1.print("received "); Serial1.println(msg);
    if (msg=='W'){
      // char msgData[30] = "";
      // Serial1.readBytesUntil('\n',msgData,30);
      // Serial1.println(String(msgData));
      delay(500);   
      readMsg();
      parseWaveform();
      sendWaveform();
    }
    else if (msg=='S' || msg == 'L' || msg == 'R' ||  msg == 'l' ||  msg == 'r'){
      delay(500);     
      readMsg();
      radioMessage.variable = msg;
      radioMessage.value = convertDigitsToNumbers(0);
      Serial1.print("\nSending '"); Serial1.print(msg);Serial1.print("' ") ;Serial1.print(radioMessage.value);Serial1.print("...");
      if (radio.sendWithRetry(12, (const void*)(&radioMessage), sizeof(radioMessage))){
        Serial1.println("data received");        
      }
      else{
        Serial1.println("data send fail");
      }
    }
    else if (msg=='T'){
      radioMessage.variable = msg;
      msgCount++;
      radioMessage.value = msgCount;
      Serial1.print(millis()-startTime);Serial1.print(",");Serial1.print(msgCount);Serial1.print(",");Serial1.println(msg);
      radio.send(12, (const void*)(&radioMessage), sizeof(radioMessage));
    }
    else if(msg=='?'){
      Serial1.println("Base Station Connected");
    }
    else if (msg=='N'){
      startNewSession();
    }
    else{ // "I" for info 
      timeSent = micros();  
      if (radio.sendWithRetry(12, &msg, 1, 0)){  // 0 = only 1 attempt, no retries
        digitalWrite(LED,LOW);
        timeSent = micros()-timeSent;
        Serial1.print("\n[");Serial1.print(timeSent);Serial1.print("] ");
      }
      else{
        digitalWrite(LED,LOW);        
        Serial1.println("\nACK not received");
      }
    }    
  }



  ///////////Receive Message From Cerebro///////
  if (radio.receiveDone()){
    if (radio.DATALEN == sizeof(currentInfo)){ //received a waveform data 
      radio.sendACK();
      currentInfo = *(Status*)radio.DATA;  //update waveform
      printInfo();      
    }
    else if (radio.DATALEN == sizeof(battery)){ //received a battery data 
      radio.sendACK();
      battery = *(Battery*)radio.DATA;  
      printBattery();      
    }    
    else if (radio.DATALEN == sizeof(diodeStats)){ //diode stats data
      radio.sendACK();
      diodeStats = *(Feedback*)radio.DATA;  
      printDiodeStats();    
    }
    else{
      for (byte i = 0; i < radio.DATALEN; i++){
        Serial1.print((char)radio.DATA[i]); 
      }
      if (radio.ACKRequested()){
        byte theNodeID = radio.SENDERID;
        radio.sendACK();
        Serial1.print(" - ACK sent.");
      }
      Blink(LED,50);
    }
  }
}


void printInfo(){
  Serial1.print("*");Serial1.print(currentInfo.serialNumber);
  Serial1.print("~");Serial1.print(currentInfo.firmware); 
  Serial1.print("~");Serial1.print(currentInfo.lSetPoint);
  Serial1.print("~");Serial1.print(currentInfo.rSetPoint);   
  Serial1.print("~"); Serial1.print(currentInfo.waveform.startDelay);       
  Serial1.print("~"); Serial1.print(currentInfo.waveform.onTime);     
  Serial1.print("~"); Serial1.print(currentInfo.waveform.offTime);      
  Serial1.print("~"); Serial1.print(currentInfo.waveform.trainDur);
  Serial1.print("~"); Serial1.print(currentInfo.waveform.rampDur); 
  Serial1.print("&"); 

  Serial1.print("\nSerial Number: ");Serial1.println(currentInfo.serialNumber);
  Serial1.print("Firmware Version: ");Serial1.println(currentInfo.firmware); 
  Serial1.print("Left Set Point: ");Serial1.println(currentInfo.lSetPoint);
  Serial1.print("Right Set Point: ");Serial1.println(currentInfo.rSetPoint);   
  Serial1.print("Start Delay: "); Serial1.println(currentInfo.waveform.startDelay);       
  Serial1.print("On Time: "); Serial1.println(currentInfo.waveform.onTime);     
  Serial1.print("Off Time: "); Serial1.println(currentInfo.waveform.offTime);      
  Serial1.print("Train Duration: "); Serial1.println(currentInfo.waveform.trainDur);
  Serial1.print("Ramp Duration: "); Serial1.println(currentInfo.waveform.rampDur);  
}

void printDiodeStats(){
  Serial1.print(currentInfo.lSetPoint);Serial1.print(",");
  Serial1.print(diodeStats.leftFBK);Serial1.print(",");
  Serial1.print(diodeStats.leftDAC);Serial1.print(",");
  Serial1.print(currentInfo.rSetPoint);Serial1.print(",");
  Serial1.print(diodeStats.rightFBK);Serial1.print(",");
  Serial1.println(diodeStats.rightDAC);  
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
  while (Serial1.available()) {
    char msg = Serial1.read();
    if (msg == ',') {
      digitIndex = 0;
      valueIndex++;
    }
    else {
      csValues[valueIndex][digitIndex] = msg-48;// convert the ascii character to the number it represents and store it
      digitIndex++;
      csValues[valueIndex][7] = digitIndex; //the number of digits that have been read in
      delay(100);
    }
  }
}

unsigned long convertDigitsToNumbers(byte whichParameter){
  unsigned long powers[7] = {1, 10, 100, 1000, 10000, 100000,1000000};
  unsigned long integerVal = 0;
  byte numDigits = csValues[whichParameter][7];
  for (int i = 0; i < numDigits; i++) {
    integerVal = integerVal + csValues[whichParameter][i] * powers[numDigits - i - 1];
  }
  return integerVal;
}
void parseWaveform(){
  waveform.startDelay = convertDigitsToNumbers(0);
  waveform.onTime = convertDigitsToNumbers(1);
  waveform.offTime = convertDigitsToNumbers(2);
  waveform.trainDur = convertDigitsToNumbers(3);
  waveform.rampDur = convertDigitsToNumbers(4);
  Serial1.print("Parsed Waveform ");
  Serial1.println(waveform.startDelay);
  Serial1.println(waveform.onTime);
  Serial1.println(waveform.offTime);
  Serial1.println(waveform.trainDur);
  Serial1.println(waveform.rampDur);
  
}
void sendWaveform(){

  if (radio.sendWithRetry(12, (const void*)(&waveform), sizeof(waveform))){
    Serial1.println("\nwaveform received");        
  }
  else{
    Serial1.println("waveform send fail");
  }
  msgCount++;  
}

void printBattery(){
  // Now print out those values:
  String toPrint = String(battery.soc) + "% | ";
  toPrint += String(battery.volts) + " mV | ";
  toPrint += String(battery.capacity) + " mAh remaining ";

  Serial1.println(toPrint);
}

void startNewSession(){
  msgCount = 0;  
}

void triggerFromTTL(){
  // unsigned long tSinceTrigger = millis() - triggerClock;
  // if (tSinceTrigger>spamFilter){
  //   cerebro.trigger();
  //   triggerClock = millis();
  //   mySerial1.print(triggerClock - timeOffset);
  //   mySerial1.print(F(",Trigger Sent\r"));
  // }
  // else{
  //   cerebro.trigger(CONTINUATION);
  //   triggerClock = millis();
  //   mySerial1.print(triggerClock - timeOffset);
  //   mySerial1.print(F(",Continue Sent,"));
  //   mySerial1.print(tSinceTrigger);
  //   mySerial1.print("\r");
  // }
}

void stopFromTTL(){
  // unsigned long tSinceTrigger = millis() - triggerClock;
  // if (tSinceTrigger<spamFilter){
  //   cerebro.stop();
  //   mySerial1.print(millis() - timeOffset);
  //   mySerial1.print(F(",Stop Sent,"));
  //   mySerial1.print(tSinceTrigger);
  //   mySerial1.print("\r");
  //   triggerClock = -spamFilter; //prevents back to back stop signals from being sent
  // }
}