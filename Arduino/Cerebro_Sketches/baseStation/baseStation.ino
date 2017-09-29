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
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

const int LED = 13;
Radio radio(8,7); //slave select pin, interrupt pin, NODE ID
WaveformData waveform;
IntegerPayload radioMessage;
Status currentInfo;
Battery battery;
char csValues[5][6];  //2D array, first dimension 0-number of parameters, 2nd dimension holds up to 5 Ascii characters that represent digits 
                      //of a integer. the final spot in the second dimension holds the number of digits read in for that parameter
unsigned int timeSent = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED, OUTPUT);
  radio.radioSetup(1,false); //nodeID, autopower on;
  
}

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
    else if (radio.DATALEN == sizeof(battery)){ //received a waveform data 
      radio.sendACK();
      battery = *(Battery*)radio.DATA;  //update waveform
      printBattery();      
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
  Serial.print("Firmware Version: ");Serial.println(currentInfo.firmware); 
  Serial.print("Left Set Point: ");Serial.println(currentInfo.lSetPoint);
  Serial.print("Right Set Point: ");Serial.println(currentInfo.rSetPoint);   
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

void printBattery(){
  // Now print out those values:
  String toPrint = String(battery.soc) + "% | ";
  toPrint += String(battery.volts) + " mV | ";
  toPrint += String(battery.capacity) + " mAh remaining ";

  Serial.println(toPrint);
}