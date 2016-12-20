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

const byte numSamples = 10;

char* unit = " volts";

char* labels[9] = { " Version",
                    "Cerebro #",
                    "  LD #",
                    "  Power Level = ",
                    " Strt Dlay",
                    " On       ",
                    " Off      ",
                    " Train Dur",
                    " Ramp Down",
                  };

void setup() {
  for (byte i=0; i<4; i++){
    pinMode(voltPins[i],INPUT);
  }
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
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("A");
  display.setCursor(30,0);
  display.print("B");
  display.println(1);
  display.println(1);
}

void loop(){
  display.setCursor(0,0);
  display.print(analogRead(NWvolt));
  display.display();

  // digitalWrite(2,HIGH);
  // delay(2000);
  // digitalWrite(2,LOW);
  // delay(2000);

  // for (int i = 0; i <4; i++){
  //   float juice = 0;
  //   for (int j = 0 ; j<numSamples; j++){
  //     juice += analogRead(voltPins[i]);
  //   }
  //   juice = juice/numSamples;
  // }
  // if (juice>10){ //cerebro is docked
  //
  // }
  // else{ //nothing is there
  //
  // }
  // juice += map(analogRead(voltPins[i]),675,860,0,100);


  // testDigits();

}
