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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Software SPI
#define OLED_DATA  7
#define OLED_CLK   6
#define OLED_DC    5
#define OLED_RESET 4
#define OLED_CS    3

#define NWvolt A0
#define NEvolt A1
#define SEvolt A2
#define SWvolt A3

Adafruit_SSD1306 display(OLED_DATA, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const byte voltPins[4] = {NWvolt,NEvolt,SEvolt,SWvolt};
const byte xpos[4] = {0,79,79,0};
const byte ypos[4] = {0,0,50,50};
const byte muxEnable = 10; //PB2
const byte button = 9; //PB1

const byte numSamples = 100;

unsigned long refreshClock = 0;
unsigned long refreshTime = 10000; //refresh every 15 seconds.


void setup() {
  for (byte i=0; i<4; i++){
    pinMode(voltPins[i],INPUT);
  }
  pinMode(muxEnable,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  digitalWrite(muxEnable,LOW);
  displaySetup();
}

void displaySetup(){
   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
  draw();
  display.display();
}

void draw(){
  display.setTextColor(1,0);
  display.setTextSize(2);
  display.setCursor(0,0);
}

void getVoltage(byte pin){
  digitalWrite(muxEnable,HIGH);
  unsigned long sample = 0;
  for (int i = 0; i<numSamples; i++){
    sample += analogRead(voltPins[pin]);
  }
  display.setCursor(xpos[pin],ypos[pin]);
  float voltage = (sample*0.86/numSamples)/1023.0*5.1;
  if (voltage>3){
    display.print(voltage);
  }
}

void refreshDisplay(){
  refreshClock = 0;
  display.clearDisplay();
  for (byte i = 0; i<4; i++){
    getVoltage(i);
  }
  display.display();
}

void loop(){
  delay(1);
  if(!digitalRead(button)){
    refreshDisplay();
  }
  if(refreshClock>refreshTime){ //
    refreshDisplay();
  }
  else{
    digitalWrite(muxEnable,LOW);
    refreshClock++;
  }
}
