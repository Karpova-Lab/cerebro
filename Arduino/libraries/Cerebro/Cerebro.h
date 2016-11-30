
#ifndef Cerebro_h
#define Cerebro_h
#define BASESTATION 1
#define REMOTE 2
#define numParameters 5
#include "Arduino.h"
class Cerebro{
public:
  Cerebro(byte emitter);
  void trigger(bool isContinuation = 0);
  void stop();
  void test();
  void saveEEPROM();
  void toggle(bool turnON);
  void send(unsigned int newVals[]);
  void implantCharacterize();
  void diodeCharacterize();
  void dumpMemory();
  void sendBinary(unsigned int value, unsigned char valSize);
  void powerTest(unsigned int testLevel);
  byte getEmitterType();
  bool isNormallyOn;
private:
  int _emitter;
  byte pin;
  unsigned long powers[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
  void pulseIR(unsigned int pulses);
  void sendMark(bool data);
  void interrupt();
};
#endif
