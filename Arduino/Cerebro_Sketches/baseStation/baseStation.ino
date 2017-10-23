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

const byte version = 33;

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
    char msg = Serial1.read();
    if (msg=='W'){  //Send waveform data
      char msgData[30] = "";
      Serial1.readBytesUntil('\n',msgData,30);
      parseData(msgData);
      updateWaveform();
    }
    else if (msg=='S' || msg == 'L' || msg == 'R' ||  msg == 'l' ||  msg == 'r'){ //parse data then send radio message with integer
      char msgData[30] = "";
      Serial1.readBytesUntil('\n',msgData,30);
      parseData(msgData);    
      radioMessage.variable = msg;
      radioMessage.value = valsFromParse[0];
      triggerClock = millis();      
      Serial1.print("\nSending '"); Serial1.print(msg);Serial1.print("' ") ;Serial1.print(radioMessage.value);Serial1.print("...");
      if (radio.sendWithRetry(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage))){
        Serial1.print("data received");newline();    
      }
      else{
        Serial1.print("data send fail");newline();
      }
    }
    else if (msg=='T'){ //Send radio message with msg count
      triggerCommandReceived();
    }
    else if (msg=='A'){ 
      stopCommandReceived();
    }
    else if(msg=='?'){
      startTime = millis();
      msgCount = 0;
      Serial1.print("Connected!");
      Serial1.print("\nBase Version,");Serial1.print(version);newline();
      Serial1.print(currentTime());comma();Serial1.print("Connecting with Cerebro...");
      msg = 'I';
      if (radio.sendWithRetry(CEREBRO, &msg, 1, 2)){ 
        Serial1.print("Connected!");
      }
      else{      
        Serial1.print("failed to connect\n\n");
      }
    }
    else if (msg!='\n'){ // "I" for info 
      if (radio.sendWithRetry(CEREBRO, &msg, 1, 0)){  // 0 = only 1 attempt, no retries
      }
      else{
        Serial1.print("Tried Sending ''");Serial1.print(msg);
        Serial1.print("'', ACK not received");newline();
      }
    }
  }

  ///////////Receive Message From Cerebro///////
  if (radio.receiveDone()){
    if (radio.DATALEN == sizeof(currentInfo)){ //received a waveform data 
      if (radio.ACKRequested()){
        radio.sendACK();
      }
      currentInfo = *(Status*)radio.DATA;  //update waveform
      printInfo();      
    }
    else if (radio.DATALEN == sizeof(battery)){ //received a battery data 
      if (radio.ACKRequested()){
        radio.sendACK();
      }
      battery = *(Battery*)radio.DATA;  
      Serial1.print("*");
      Serial1.print(battery.soc);
      Serial1.print("&");   
      Serial1.print(currentTime());
      Serial1.print(",B,");Serial1.print(battery.soc);Serial1.print(",Connection Good!\n");   

    }    
    else if (radio.DATALEN == sizeof(diodeStats)){ //diode stats data
      if (radio.ACKRequested()){
        radio.sendACK();
      }
      diodeStats = *(Feedback*)radio.DATA;  
      printDiodeStats();    
    }
    else if (radio.DATALEN == sizeof(radioMessage)){ // receiving message 'M' or 'm' or 'Y'
      if (radio.ACKRequested()){
        radio.sendACK();
      }
      radioMessage = *(IntegerPayload*)radio.DATA;  
      if (radioMessage.variable == 'M' || radioMessage.variable =='m'){
        Serial1.print((char)radioMessage.variable);comma();Serial1.print(radioMessage.value);newline();      
      }
      else if (radioMessage.variable == 'Y'){
        Serial1.print("Cerebro turned on and connected,");Serial1.print(radioMessage.value);//print time it took to startup and send connection message. 
      }
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
  //update filter
  if (waveform.trainDur>0){
    spamFilter = currentInfo.waveform.startDelay + currentInfo.waveform.trainDur;
  }
  else{
    spamFilter = currentInfo.waveform.startDelay + currentInfo.waveform.onTime;
  }
  
  Serial1.print("\nSerial Number, ");Serial1.print(currentInfo.serialNumber);newline();
  Serial1.print("Cerebro Version, ");Serial1.print(currentInfo.firmware);newline();
  Serial1.print("Left Set Point, ");Serial1.print(currentInfo.lSetPoint);newline();
  Serial1.print("Right Set Point, ");Serial1.print(currentInfo.rSetPoint); newline();  
  Serial1.print("Start Delay, "); Serial1.print(currentInfo.waveform.startDelay);newline();    
  Serial1.print("On Time, "); Serial1.print(currentInfo.waveform.onTime);newline();
  Serial1.print("Off Time, "); Serial1.print(currentInfo.waveform.offTime);newline();
  Serial1.print("Train Duration, "); Serial1.print(currentInfo.waveform.trainDur);newline();
  Serial1.print("Ramp Duration, "); Serial1.print(currentInfo.waveform.rampDur);newline();
  Serial1.print("Battery Level, "); Serial1.print(currentInfo.battery);newline();newline();  

  Serial1.print("*");Serial1.print(currentInfo.serialNumber);
  Serial1.print("~");Serial1.print(currentInfo.firmware); 
  Serial1.print("~");Serial1.print(currentInfo.lSetPoint);
  Serial1.print("~");Serial1.print(currentInfo.rSetPoint);   
  Serial1.print("~"); Serial1.print(currentInfo.waveform.startDelay);       
  Serial1.print("~"); Serial1.print(currentInfo.waveform.onTime);     
  Serial1.print("~"); Serial1.print(currentInfo.waveform.offTime);      
  Serial1.print("~"); Serial1.print(currentInfo.waveform.trainDur);
  Serial1.print("~"); Serial1.print(currentInfo.waveform.rampDur); 
  Serial1.print("~");Serial1.print(currentInfo.battery);Serial1.print("&\n");  
}

void printDiodeStats(){
  Serial1.print(currentTime());comma();
  Serial1.print("F,");
  Serial1.print(currentInfo.lSetPoint);comma();
  Serial1.print(diodeStats.leftFBK);comma();
  Serial1.print(diodeStats.leftDAC);comma();
  Serial1.print(currentInfo.rSetPoint);comma();                 
  Serial1.print(diodeStats.rightFBK);comma();
  Serial1.print(diodeStats.rightDAC);newline();
}

void updateWaveform(){
  Serial1.print(currentTime());comma();Serial1.print(msgCount);comma();Serial1.print('W');newline();
  waveform.startDelay = valsFromParse[0];
  waveform.onTime = valsFromParse[1];
  waveform.offTime = valsFromParse[2];
  waveform.trainDur = valsFromParse[3];
  waveform.rampDur = valsFromParse[4];
  msgCount++;   
  radio.send(CEREBRO, (const void*)(&waveform), sizeof(waveform));
}

void triggerCommandReceived(){
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger>spamFilter){
    msgCount++;    
    radioMessage.variable = 'T';    
    radioMessage.value = msgCount;
    Serial1.print(currentTime());comma();Serial1.print(msgCount);comma();Serial1.print((char)radioMessage.variable);newline();
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = millis();
  }
  else{
    msgCount++;        
    radioMessage.variable = 'C';    
    radioMessage.value = msgCount;
    Serial1.print(currentTime());comma();Serial1.print(msgCount);comma();Serial1.print((char)radioMessage.variable);comma();Serial1.print(tSinceTrigger);newline();
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = millis();    
  }
}

void stopCommandReceived(){
  unsigned long tSinceTrigger = millis() - triggerClock;
  if (tSinceTrigger<spamFilter){
    msgCount++;      
    radioMessage.variable = 'A';    
    radioMessage.value = msgCount;
    Serial1.print(currentTime());comma();Serial1.print(msgCount);comma();Serial1.print((char)radioMessage.variable);comma();Serial1.print(tSinceTrigger);newline();
    radio.send(CEREBRO, (const void*)(&radioMessage), sizeof(radioMessage));
    triggerClock = -spamFilter; //prevents back to back stop signals from being sent
  }
}

unsigned long currentTime(){
  return millis()-startTime;
}

void comma(){
  Serial1.print(",");
}

void newline(){
  Serial1.print("\n");
}