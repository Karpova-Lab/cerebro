
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
  void send(byte readValues[][6]);
  void send(int newVals[]);
  byte getEmitterType();
  int  getParameter(byte paramIndex);
  bool isNormallyOn;
private:
  int _emitter;
  byte pin;
  unsigned long powers[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
  unsigned int decnum[numParameters];
  char bitIndex = 15;
  void pulseIR(unsigned int pulses);
  void sendMark(bool data);
  void interrupt();
};
#endif
