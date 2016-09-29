/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2016 Andy S. Lustig

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

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Cerebro.h>

Cerebro cerebro(REMOTE);

//Software SPI
#define OLED_MOSI  A5
#define OLED_CLK   A4
#define OLED_DC    A3
#define OLED_RESET A2
#define OLED_CS    A1

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const byte battery  = A0;
const byte encoderPin1 = 3;
const byte encoderPin2 = 2;
volatile int lastEncoded = 0;
const byte encoderSwitchPin = 4; //push button switch
const byte triggerButton = 7;
const byte stopButton = 8;
const byte red = 5;
const byte blue = 9;
const byte green = 6;

const unsigned int step[5] = {500,500,500,500,500};
const unsigned int stepAlt[5] = {50,50,50,50,100};
const unsigned int Powers[5] = {1,10,100,1000,10000};
const byte ledLevels[3] = {0,0,0};

char* labels[9] = { " Version",
                    "Cerebro #",
                    "  LD #",
                    " Power level = ",
                    " Strt Dlay(ms)",
                    " On       (ms)",
                    " Off      (ms)",
                    " Train Dur(ms)",
                    " Ramp Down(ms)",
                  };

char msgBuffer[40];
unsigned int params[9] = {0,0,0,0,0,0,0,0,0};
int tempNow=0,tempOld = 0;
float battLevel = 0;
int highlight = 1;
bool isEditMode = false;
bool newButtonState,oldButtonState;
byte pos = 0;
byte paramCount = 0;
byte digitBuffer[5] = {};
byte digitCount = 0;
int letter = 0;
char ltr;
byte count = 0;
unsigned long pressedCount = 0;
bool altStep = false;

void setup(){
  Serial.begin(115200);
  Serial.println(F("Serial Connected"));
  displaySetup();
  encoderSetup();
  pinMode(triggerButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(battery,INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  analogWrite(red,255-ledLevels[0]);
  analogWrite(green,255-ledLevels[1]);
  analogWrite(blue,255-ledLevels[2]);
  Serial.println(F("Setup complete\r\n"));
}

void loop() {
  if (analogRead(battery)>100){
    takeBatteryReading();
  }
  else{
    showBatteryStatus(false);
  }
  if (Serial.available()){ //Serial message received
    fillBuffer();
    if(letter>1){
      parseMsg();
    }
  }
  if (tempNow-tempOld>2){ //CCW knob turn
    left();
  }
  else if (tempNow-tempOld<-2){ //CW knob turn
    right();
  }
  checkButtonPress(triggerButton,triggerSent,green,sendSave,blue);
  checkButtonPress(stopButton,stopSent,red,sendVals,blue);
  newButtonState = digitalRead(encoderSwitchPin);
  if (newButtonState){
    pressedCount++;
    altStep = true;
  }
  if ((newButtonState-oldButtonState)==-1){ //rotary button released
    if (pressedCount<10){
      clicked();
    }
    pressedCount = 0;
    altStep = false;
  }
  oldButtonState = newButtonState;
}

void checkButtonPress( byte buttonPin, void (*fxn_1)(), byte c_1, void (*fxn_2)(), byte c_2 ){
  if (!digitalRead(buttonPin)){ //button pressed.
    delay(20);
    unsigned int holdCount = 0;
    analogWrite(green,255);
    analogWrite(red,255);
    analogWrite(blue,255);
    while(!digitalRead(buttonPin) && holdCount<255){
      delay(2);
      analogWrite(c_1,255-holdCount);
      holdCount++;
    }
    if (!(holdCount<255)){    //short press
      analogWrite(c_1,255);
      analogWrite(c_2,1);
      delay(100);
      analogWrite(c_2,255);
      delay(100);
      analogWrite(c_2,1);
      delay(100);
      analogWrite(c_2,255);
      (*fxn_2)();
    }
    else{                     //long press
      (*fxn_1)();
    }
  }
}

void parseMsg(){
  for (int i = 0 ; i <letter; i++){
    ltr = msgBuffer[i];
    if (ltr=='*'){
      Serial.println();
      for (int i  = 0; i < 9; i++){
        Serial.println(params[i]);
      }
      break;
    }
    else if (ltr=='~'){
      digitCount = convertToInt(paramCount,digitCount);
      paramCount++;
    }
    else if (ltr!=13 && ltr!=10 && ltr!=0){  //in between '~'
      digitBuffer[digitCount] = ltr-48;
      digitCount++;
    }
  }
  paramCount = 0;
  highlight = 1;
  isEditMode = false;
  display.display();
  draw(isEditMode);
  letter = 0;
}

byte convertToInt(byte _paramCount,byte _digitCount){
  params[_paramCount] = 0;
  for (int i = 0; i < _digitCount; i++){
    params[_paramCount] += Powers[_digitCount-i-1]*digitBuffer[i];
  }
  return 0;
}

byte getPos(){
  for (int i = 4; i<9; i++){
    if (highlight == (1<<(i-4))){
      return i;
    }
  }
}

void triggerSent(){
  analogWrite(green,1);
  cerebro.trigger();
  display.clearDisplay();
  display.setTextColor(1,0);
  display.setTextSize(3);
  display.setCursor(1,10);
  display.print(F("Trigger"));
  display.setCursor(30,36);
  display.print(F("Sent"));
  display.display();
  unsigned long tempTime = millis();
  while(millis()-tempTime<750){ //leave messsage up for 750ms, all the while checking for a stop button
    if (!digitalRead(stopButton)){ //stopButton pushed
      stopSent();
    }
  }
  display.clearDisplay();
  draw(isEditMode);
  analogWrite(green,255);
}

void stopSent(){
  analogWrite(green,255);
  analogWrite(red,1);
  cerebro.stop();
  display.clearDisplay();
  display.setTextColor(1,0);
  display.setTextSize(3);
  display.setCursor(1,10);
  display.print(F("  Stop"));
  display.setCursor(30,36);
  display.print(F("Sent"));
  display.display();
  delay(750);
  display.clearDisplay();
  draw(isEditMode);
  analogWrite(red,255);
}

void sendVals(){
  delay(50);
  int newVals[5] = {params[4],params[5],params[6],params[7],params[8]};
  cerebro.send(newVals);
  display.clearDisplay();
  display.setTextColor(1,0);
  display.setTextSize(2);
  display.setCursor(5,10);
  display.print(F("Parameters"));
  display.setCursor(35,36);
  display.print(F("Sent"));
  display.display();
  delay(750);
  display.clearDisplay();
  draw(isEditMode);
}

void sendSave(){
  cerebro.saveEEPROM();
  display.clearDisplay();
  display.setTextColor(1,0);
  display.setTextSize(3);
  display.setCursor(1,10);
  display.print(F("  Save"));
  display.setCursor(30,36);
  display.print(F("Sent"));
  display.display();
  delay(750);
  display.clearDisplay();
  draw(isEditMode);
}

void showBatteryStatus(bool show){
  display.setTextColor(1,0);
  display.setTextSize(1);
  display.setCursor(0,0); //column, row
  if(show){
    display.print(labels[1]);//Cerebro label
    display.print(params[1]);//Cerebro#
    fillExcess(params[1],3);
    display.print(labels[2]);//LD label
    display.println(params[2]);//LD#
    display.print(F("Battery = "));
    display.print(battLevel);
    display.print(" volts");
    display.print(labels[3]);//Power label
    display.print(params[3]);//Power Level
  }
  else{
    display.print("                     ");
    display.print("                     ");
    display.print("                     ");
  }
  display.display();
}

void takeBatteryReading(){
  byte averageCount = 1000;
  for(int i = 0; i <averageCount; i++){ //take multiple readings
    battLevel += analogRead(battery);
  }
  battLevel = battLevel/averageCount/1023.0*3.295/(270.0/370.0) + 0.08 ;//calculate the average of the readings voltage. 270/370 because of voltage divider. there is 80 mV drop from battery to MOSI
  if (battLevel>3){
    showBatteryStatus(true);
  }
  else{
  }
}

void fillBuffer(){
  while (Serial.available()){
    msgBuffer[letter] = Serial.read();
    letter++;
    int tempTime = millis();
    while(!Serial.available() && (millis()-tempTime)<250){
      delay(10);
    }
  }
}
