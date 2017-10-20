#include "LaserDiode.h"
LaserDiode::LaserDiode(volatile uint8_t *slaveDirReg,volatile uint8_t *_slaveOutputReg, uint8_t _slavePin, uint8_t _analogPin){
  slaveOutputReg = _slaveOutputReg;
  slaveSelectPin = _slavePin;
  analogPin = _analogPin;
  setPoint = 0;
  DACisMaxed = false;
  *slaveDirReg |= (1<<slaveSelectPin);       //set slave select pin as output
  *slaveOutputReg |= (1<<slaveSelectPin);  //Set slave select HIGH (LOW selects the chip)    
}

bool LaserDiode::off(){
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  *slaveOutputReg &= ~(1<<slaveSelectPin); //latch low  
  SPI.transfer(64);                        //Power down command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(0);                  //shift high byte
  SPI.transfer(0);            //shift low byte
  *slaveOutputReg |= (1<<slaveSelectPin);  //latch high
  if (DAClevel==4095){
    DACisMaxed = true;
  }
  DAClevel = 0;
  SPI.endTransaction();   
  return false;
}

void LaserDiode::sendDAC(unsigned int value) {
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  *slaveOutputReg &= ~(1<<slaveSelectPin); //latch low  
  SPI.transfer(48);                        //Write to and Update (Power up) DAC Register command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(value>>4);                  //shift high byte
  SPI.transfer(value<<4 & 255);            //shift low byte
  *slaveOutputReg |= (1<<slaveSelectPin);  //latch high
  SPI.endTransaction();
}

void LaserDiode::feedback(int setPoint){
  const float KP = 0.2;
  int photoTransistorVoltage = analogRead(analogPin);
  int error = setPoint-photoTransistorVoltage;
  DAClevel = DAClevel+int(error*KP);
  if (DAClevel>4095) {
    DAClevel = 4095;
  }
  else if (DAClevel<0){
    DAClevel = 0;
  }
}

void LaserDiode::fade(int rampDuration){
  unsigned long fadeClock;
  unsigned int param1;
  for (int i = 99; i>-1;i--) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
    fadeClock = millis();
    feedback(setPoint*i/100);
    sendDAC(DAClevel);
    while((millis()-fadeClock)<(rampDuration/100)){
      //wait
    }
  }
}