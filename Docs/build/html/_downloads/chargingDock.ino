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

byte digits[10]  = {252,96,218,242,102,182,190,224,254,230}; //{0,1,2,3,4,5,6,7,8,9}  add 1 to value to add decimal point ("1" is 252, "1." is 253)
//byte pins[10] = {2,3,4,5,6,7,8,9,10,11}; //{A,B,C,D,E,F,G,DP,first,second} //attiny2313
byte pins[10] = {0,1,2,3,4,5,7,8,9,10}; //{A,B,C,D,E,F,G,DP,first,second} //attiny84
#define batt 6
int pov = 20;
int juice;
int tempjuice;
int count = 0;
int show = 0;

void setup() {
  pinMode(batt,INPUT);
  for(int i=0; i<11; i++){
    pinMode(pins[i],OUTPUT);
    if(i>8){
      digitalWrite(pins[i],HIGH);
    }
    else{
      digitalWrite(pins[i],LOW);
    }
  }
}

void loop(){
  juice = map(analogRead(batt),0,724,0,99);
  if (juice>45){
    delay(500);
    for(int i = 0; i<1000; i++){
      segmentize(juice/10,0); //display 1st digit
      segmentize(juice%10,1); ///display 2nd digit
    }
  }
  // testDigits();

}

void segmentize(int number,boolean place){
  //
  for (int i=7; i>-1; i--){
    digitalWrite(pins[i],bitRead(digits[number],7-i));
  }
  digitalWrite(pins[8],place); //pulling Digit 1's common cathode low, illuminates the digit 1
  digitalWrite(pins[9],!place); //pulling Digit 2's common cathode low, illuminates digit 2
  delay(1);
  digitalWrite(pins[8+place],HIGH); //pull Digits High to turn them off.
}

void testDigits(){
  for (int i = 0; i<10; i++){
    for (int j=0; j<200; j++){
      segmentize(i,0);
      segmentize(i,1);
    }
  }
}
