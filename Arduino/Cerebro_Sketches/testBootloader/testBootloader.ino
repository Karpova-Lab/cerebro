
int indicatorLED = A0;
#include <SparkFunBQ27441.h>      //https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library


void setup() {                
  Serial.begin(115200);
  delay(15000);
   // Indicator LED
  pinMode(indicatorLED,OUTPUT);
  digitalWrite(indicatorLED,HIGH);
    //*** Battery Monitor ***//
  Serial.println("connecting with battery  monitor...");
  if (!lipo.begin()){
	// If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while(1){ //blink error
      digitalWrite(indicatorLED,HIGH);
      delay(1000);
      digitalWrite(indicatorLED,LOW);
      delay(1000);
    }
  }
  Serial.println("Connected to BQ27441!");
  lipo.setCapacity(400);
  while(lipo.soc()==0){
    delay(1);
    //wait;
  }  
  Serial.println("Complete!");
   // Indicator LED
  pinMode(indicatorLED,OUTPUT);
  digitalWrite(indicatorLED,HIGH);
}

// the loop routine runs over and over again forever:
void loop() {              // wait for a second
}

