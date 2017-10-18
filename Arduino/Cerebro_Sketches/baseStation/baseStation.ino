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

const byte version = 25;

const int LED = 13;
const int triggerPin = 5;
const int stopPin = 6;

Radio radio(8,7); //slave select pin, interrupt pin
WaveformData waveform;
IntegerPayload radioMessage;
Status currentInfo;
Battery battery;
Feedback diodeStats;
unsigned long valsFromParse[5];
unsigned int msgCount = 0;
unsigned long startTime = 0;
unsigned long spamFilter = 0;
unsigned long triggerClock = 0;

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
    stopCommandReceived();
  }
  //if we read a high signal on pin 6, send a stop command to cerebro
  if (digitalRead(stopPin)) {
    while(digitalRead(stopPin)){}       //wait until signal goes low
    triggerCommandReceived();
  }

  ///////////Receive Message From Xavier//////////////////
  if (Serial1.available()){
    digitalWrite(LED,HIGH);
    char msg = Serial1.read();
    if (msg=='W'){  //Send waveform data
      char msgData[30] = "";
      Serial1.readBytesUntil('\n',msgData,30);
      parseData(msgData);
      assignDataToWaveform();
      sendWaveform();
    }
    else if (msg=='S' || msg == 'L' || msg == 'R' ||  msg == 'l' ||  msg == 'r'){ //parse data then send radio message with integer
      char msgData[30] = "";
      Serial1.readBytesUntil('\n',msgData,30);
      parseData(msgData);    
      radioMessage.variable = msg;
      radioMessage.value = valsFromParse[0];
      Serial1.print("\nSending '"); Serial1.print(msg);Serial1.print("' ") ;Serial1.print(radioMessage.value);Serial1.print("...");
      if (radio.sendWithRetry(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage))){
        Serial1.print("data received");Serial1.print("\n");    
      }
      else{
        Serial1.print("data send fail");Serial1.print("\n");
      }
    }
    else if (msg=='T'){ //Send radio message with msg count
      triggerCommandReceived();
    }
    else if (msg=='A'){ 
      stopCommandReceived();
    }
    else if(msg=='?'){
      Serial1.print(version);Serial1.print("\n");
    }
    else if (msg=='N'){
      startNewSession();
    }
    else if (msg!='\n'){ // "I" for info 
      if (radio.sendWithRetry(CEREBRO, &msg, 1, 0)){  // 0 = only 1 attempt, no retries
        digitalWrite(LED,LOW);
      }
      else{
        digitalWrite(LED,LOW);        
        Serial1.print("\nACK not received");Serial1.print("\n");
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

void parseData(char* dataMessage){
  char* msgPointer;
  msgPointer = strtok(dataMessage,",");
  char i = 0;
  while (msgPointer!=NULL){
    valsFromParse[i] = atol(msgPointer);   
    msgPointer = strtok(NULL,",");
    i++;
  }
}


void printInfo(){
  updateFilter();
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

  Serial1.print("\nSerial Number: ");Serial1.print(currentInfo.serialNumber);Serial1.print("\n");
  Serial1.print("Firmware Version: ");Serial1.print(currentInfo.firmware);Serial1.print("\n");
  Serial1.print("Left Set Point: ");Serial1.print(currentInfo.lSetPoint);Serial1.print("\n");
  Serial1.print("Right Set Point: ");Serial1.print(currentInfo.rSetPoint); Serial1.print("\n");  
  Serial1.print("Start Delay: "); Serial1.print(currentInfo.waveform.startDelay);Serial1.print("\n");    
  Serial1.print("On Time: "); Serial1.print(currentInfo.waveform.onTime);Serial1.print("\n");
  Serial1.print("Off Time: "); Serial1.print(currentInfo.waveform.offTime);Serial1.print("\n");
  Serial1.print("Train Duration: "); Serial1.print(currentInfo.waveform.trainDur);Serial1.print("\n");
  Serial1.print("Ramp Duration: "); Serial1.print(currentInfo.waveform.rampDur);Serial1.print("\n");
}

void printDiodeStats(){
  Serial1.print(currentInfo.lSetPoint);Serial1.print(",");
  Serial1.print(diodeStats.leftFBK);Serial1.print(",");
  Serial1.print(diodeStats.leftDAC);Serial1.print(",");
  Serial1.print(currentInfo.rSetPoint);Serial1.print(",");
  Serial1.print(diodeStats.rightFBK);Serial1.print(",");
  Serial1.print(diodeStats.rightDAC);Serial1.print("\n");
}

void Blink(byte PIN, int msDelay)
{
  digitalWrite(PIN,HIGH);
  delay(msDelay);
  digitalWrite(PIN,LOW);
}

void updateFilter(){
  if (waveform.trainDur>0){
    spamFilter = currentInfo.waveform.startDelay + currentInfo.waveform.trainDur;
  }
  else{
    spamFilter = currentInfo.waveform.startDelay + currentInfo.waveform.onTime;
  }
}

void assignDataToWaveform(){
  waveform.startDelay = valsFromParse[0];
  waveform.onTime = valsFromParse[1];
  waveform.offTime = valsFromParse[2];
  waveform.trainDur = valsFromParse[3];
  waveform.rampDur = valsFromParse[4];

}

void sendWaveform(){
  radio.send(CEREBRO, (const void*)(&waveform), sizeof(waveform));
  msgCount++;  
}

void printBattery(){
  // Now print out those values:
  String toPrint = String(battery.soc) + "% | ";
  toPrint += String(battery.volts) + " mV | ";
  toPrint += String(battery.capacity) + " mAh remaining ";

  Serial1.print(toPrint);Serial1.print("\n");
}

void startNewSession(){
  msgCount = 0;  
}

void triggerCommandReceived(){
  msgCount++;
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger>spamFilter){
    radioMessage.variable = 'T';    
    radioMessage.value = msgCount;
    Serial1.print(millis()-startTime);Serial1.print(",");Serial1.print(msgCount);Serial1.print(",");Serial1.print((char)radioMessage.variable);Serial1.print("\n");
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = millis();
  }
  else{
    radioMessage.variable = 'C';    
    radioMessage.value = msgCount;
    Serial1.print(millis()-startTime);Serial1.print(",");Serial1.print(msgCount);Serial1.print(",");Serial1.print((char)radioMessage.variable);Serial1.print(",");Serial1.print(tSinceTrigger);Serial1.print("\n");
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = millis();    
  }
}

void stopCommandReceived(){
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger<spamFilter){
    radioMessage.variable = 'A';    
    radioMessage.value = msgCount;
    Serial1.print(millis()-startTime);Serial1.print(",");Serial1.print(msgCount);Serial1.print(",");Serial1.print((char)radioMessage.variable);Serial1.print(",");Serial1.print(tSinceTrigger);Serial1.print("\n");
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = -spamFilter; //prevents back to back stop signals from being sent
  }
}