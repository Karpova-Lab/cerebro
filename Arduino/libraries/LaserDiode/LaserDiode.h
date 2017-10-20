
#ifndef LaserDiode_h
#define LaserDiode_h
#include "Arduino.h"
#include <SPI.h>
class LaserDiode{
public:
  volatile uint8_t *slaveOutputReg;
  uint8_t slaveSelectPin;
  uint8_t analogPin;
  bool DACisMaxed;
  int16_t DAClevel;
  int16_t setPoint;

  LaserDiode(volatile uint8_t *slaveDirReg, volatile uint8_t *_slaveOutputReg, uint8_t _slavePin, uint8_t _analogPin);
  bool off();
  void sendDAC(unsigned int value);
  void feedback(int setPoint);
  void fade(int setLevel);
};
#endif
