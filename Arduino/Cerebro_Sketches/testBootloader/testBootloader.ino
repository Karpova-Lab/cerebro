
#include <SparkFunBQ27441.h>      //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <LaserDiode.h>
const uint8_t indicatorLED = A0; //32u4 pin 36
LaserDiode right(&DDRB,&PORTB,0,A4);
LaserDiode left(&DDRD,&PORTD,2,A2);
uint32_t timePassed = 0;
uint16_t startupTime = 10000;
void setup() {                
  Serial.begin(115200);
  timePassed = millis();
  // Indicator LED
  pinMode(indicatorLED,OUTPUT);
  digitalWrite(indicatorLED,HIGH);
  while(millis()-timePassed<startupTime){
    Serial.println(((millis()-timePassed)%startupTime)/1000);
    blink(100);
    blink(100);
    delay(600);
  }
    //*** Battery Monitor ***//
  Serial.println("connecting with battery  monitor...");
  if (!lipo.begin()){
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while(1){ //blink error
      blink(100);
    }
  }
  Serial.println("Connected to BQ27441!");
  digitalWrite(indicatorLED,HIGH);
  lipo.setCapacity(400);
  while(lipo.soc()==0){
    Serial.print(".");
    delay(1);
  }  
  Serial.println("Complete!");
  digitalWrite(indicatorLED,LOW);
}

void loop() {
}

void blink(int blinkDelay){
  digitalWrite(indicatorLED,HIGH);
  delay(blinkDelay);
  digitalWrite(indicatorLED,LOW);
  delay(blinkDelay);
}

