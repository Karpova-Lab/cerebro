/*This program repeatedly emits an IR signal to trigger the laser diode during light power testing.
It also monitors the battery voltage of cerebro and prints the data to the serial monitor.
The delay between IR emissions can be adjusted by sending 0-9 value in the serial monitor. The
new delay will be the (value sent)*100, i.e. sending a 3 will result in a 300ms delay between triggers.
*/
#define BATTERYTEST //comment this line to perform gaptest

#include <Cerebro.h>
Cerebro cerebro(REMOTE);
#define battery A0
unsigned long battClock = 0;
unsigned int battPeriod = 30000;   //30 seconds
unsigned int interTrigger = 9000;
unsigned long pause = 3;
unsigned int count = 0;
unsigned long startTime;
#define arrayLength 4
int trainGaps[arrayLength] = {4250,4750,5250,5750};//msec between triggers
int dataLengths[arrayLength] = {8,8,8,8};//minutes of consecutive triggers
int restTime = 5; //minutes between different traingap parameter testing
byte i = 0;
void (* resetFunc)(void) = 0;

void setup()   {
  startTime = millis();
  pinMode(battery,INPUT);
  Serial.begin(9600);
  Serial.println("Send something to begin test");
  while(!Serial.available()){};    //wait unit something is sent before starting
  Serial.read();
  #ifdef BATTERYTEST
  Serial.print("\nTriggering every " + String(interTrigger) + " msecs, battery monitor every " + String(battPeriod/1000) + " seconds\n");
  printData();
  #else
  printGap();
  #endif
}
void loop (){
  #ifdef BATTERYTEST
  batTest();
  #else
  gapTest();
  #endif
}

void printGap(){
  Serial.print("Gap = ");
  Serial.print(trainGaps[i]);
  Serial.print(", count= ");
  Serial.println(count);
}

void batTest(){
  if ((millis()-battClock)>battPeriod){
    printData();
  }
  cerebro.trigger();
  count++;
  delay(interTrigger);
}

void gapTest(){
  Serial.println(millis());
  cerebro.trigger();
  count++;
  delay(trainGaps[i]);
  if ((millis()-startTime)>dataLengths[i]*60000){
    i++;
    if(i==arrayLength){
      resetFunc();
    }
    printGap();
    delay(restTime*60000); //wait 5 minutes
    startTime = millis();
  }
}

void printData(){
  battClock = millis();
  Serial.print(millis());
  Serial.print(",");
  float read = float(analogRead(A0));
  Serial.print(read);
  Serial.print(",");
  Serial.print(read/1023.0*5.0);//+.15); //there is a 0.15v offset
  Serial.print(",");
  Serial.println(count);
}

/*if(Serial.available()){
  pause = int(Serial.read()-65);
  Serial.print("new pause: ");
  Serial.println(250*pause);
}*/
