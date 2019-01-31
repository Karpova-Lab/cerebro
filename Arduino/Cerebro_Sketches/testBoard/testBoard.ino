// Updated 2018-05-08
#include <SparkFunBQ27441.h>      //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library
#include <LaserDiode.h>
const uint8_t amberLED = A0; //32u4 pin 36
const uint8_t greenLED = 12; //32u4 pin 26
const uint8_t userBTN = 6; //32u4 pin 27
LaserDiode right(&DDRB,&PORTB,0,A4);
LaserDiode left(&DDRD,&PORTD,2,A2);
uint32_t timePassed = 0;
uint16_t startupTime = 10000;
void setup() {                
  Serial.begin(115200);
  // Indicator LED
  pinMode(userBTN,INPUT_PULLUP);
  pinMode(greenLED,OUTPUT);
  pinMode(amberLED,OUTPUT);
  digitalWrite(greenLED,LOW);
  digitalWrite(amberLED,HIGH);
  while(digitalRead(userBTN)){
    Serial.println("Hold \"BTN\" to begin");
    blink(100);
    blink(100);
    delay(600);
  }
  Serial.println("\"BTN\" can be released\n");

    //*** Battery Monitor ***//
  Serial.print("Establishing connection with BQ27441 battery monitor...");
  if (!lipo.begin()){ 	// If communication fails, print an error message and loop forever.
    Serial.println("\nError: Unable to communicate with BQ27441.");
    while(1){ //blink error
      blink(100);
    }
  }
  Serial.println("connected");
  digitalWrite(amberLED,HIGH);
  Serial.print("Setting Capacity...");
  lipo.setCapacity(400);
  Serial.println("set");
  Serial.print("Waiting for state of charge to initialize...");
  while(lipo.soc()==0){
    Serial.print(".");
    delay(1);
  } 
  Serial.println("done\n");
  Serial.print("Battery Charge is at ");
  Serial.print(lipo.soc());
  Serial.println("%");
  digitalWrite(amberLED,LOW);

}

void loop() {
}

void blink(int blinkDelay){
  digitalWrite(amberLED,HIGH);
  digitalWrite(greenLED,HIGH);
  delay(blinkDelay);
  digitalWrite(amberLED,LOW);
  digitalWrite(greenLED,LOW);
  delay(blinkDelay);
}

