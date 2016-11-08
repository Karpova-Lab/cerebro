#include "Cerebro.h"

Cerebro::Cerebro(byte emitter)  //
{
  #define  spikes   7      //number of spikes in burst
  #define  space    280    //microsecond gap between bursts
  #define  sensorDelay 13
  _emitter = emitter;
  switch (_emitter){  //setup pin outputs and set initialize isNormallyOn
    case 1: //PB1 baseStation.ino(Attiny85:pin6)
      DDRB |= (1<<1); //set PB1 to output
      // PORTB |= (1<<1); //begin IR high
      PORTB &= ~(1<<0);//begin IR low
      pin = 1;
      isNormallyOn = false;//
      break;
    case 2: //PB2 UNO pin 10
      DDRB |= (1<<2); //set PB2 to output
      PORTB &= ~(1<<0);//begin IR low
      pin = 2;
      isNormallyOn = false;
      break;
  }
}

byte Cerebro::getEmitterType(){
  return _emitter;
}
void Cerebro::trigger(bool isContinuation)
{
  if (isContinuation){
    interrupt();
  }
  sendMark(true);
  sendMark(false);
  sendMark(true);
  sendMark(false);
}

void Cerebro::stop(){
  interrupt();
  sendMark(false);
  sendMark(true);
  sendMark(true);
  sendMark(false);
}

void Cerebro::interrupt()
{
  pulseIR(14);
  delay(1);       //assurance that back to back stop signals aren't mistaken for 1 trigger signal
}

void Cerebro::test()
{
  trigger();
  delay(100);
  stop();
  delay(100);
}

void Cerebro::saveEEPROM()
{
    for(int y = 0;y<26;y++){
      sendMark(false);
    }
}

void Cerebro::sendMark(bool data)
{
  pulseIR((3*data+1)*spikes);
  delayMicroseconds(space);
}

void Cerebro::pulseIR(unsigned int pulses)
{
  noInterrupts();  // this turns off any background interrupts
  while (pulses > 0) {
    /*sends high-low pulse. DigitalWrite in itself takes about 12µs so instead we control the registers directly. An oscilliscope is used
    to confirm a proper pulse length such that the signal is 38kHz (13 microsends HIGH,13 microseconds LOW) and thus picked up by the IR sensor*/
    if(isNormallyOn){
      PORTB &= ~(1<<pin);       //LOW
      delayMicroseconds(sensorDelay);
      PORTB |= (1<<pin);        //HIGH
      delayMicroseconds(sensorDelay);
      pulses--;
    }
    else{
      PORTB |= (1<<pin);        //HIGH
      delayMicroseconds(sensorDelay);
      PORTB &= ~(1<<pin);       //LOW
      delayMicroseconds(sensorDelay);
      pulses--;
    }
  }
  interrupts(); // this turns them back on
}

void Cerebro::toggle(bool turnON)
{
  if(turnON){
    PORTB |= (1<<pin);        //HIGH
  }
  else{
    PORTB &= ~(1<<pin);       //LOW
  }
  isNormallyOn = !isNormallyOn;
}

void Cerebro::sendBinary(unsigned int value, unsigned char bitWidth){
  while (bitWidth > 0) {
    sendMark(bitRead(value, bitWidth-1));
    bitWidth--;
  }
}

void Cerebro::send(unsigned int newVals[]) //ir remote send
{
  //send key that says that data is to follow
  sendBinary(101,7);
  //send 10 bytes (5 parameters*16bits) worth of data
  for (int k = 0; k<numParameters; k++){
    sendBinary(newVals[k],16);
  }
}

void Cerebro::calibrate()
{
  //send key that says that data is to follow
  sendBinary(22,7);
}

void Cerebro::powerTest(unsigned int testLevel)
{
  sendBinary(36,7);
  delay(100);
  unsigned int tempVals[5] = {testLevel,0,0,0,0};
  send(tempVals);
}
