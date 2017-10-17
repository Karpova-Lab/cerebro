
#ifndef Radio_h
#define Radio_h
#include "Arduino.h"
#include <SPI.h>
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69

//*********************************************************************************************
#define NETWORKID     100  //the same on all nodes that talk to each other (range up to 255)
#define GATEWAYID     1
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_915MHZ
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level (ATC_RSSI)
#define ATC_RSSI      -80

typedef struct {
  unsigned int  startDelay;
  unsigned int  onTime;
  unsigned int  offTime;
  unsigned long trainDur;
  unsigned int  rampDur;
} WaveformData;

typedef struct {
  unsigned char variable;
  unsigned int value;
} IntegerPayload;

typedef struct {
  byte  serialNumber;
  byte  firmware;
  WaveformData waveform;
  unsigned int lSetPoint;
  unsigned int rSetPoint;
} Status;

typedef struct {
  unsigned int soc;
  unsigned int volts;
  unsigned int capacity;
} Battery;

typedef struct {
  unsigned int leftFBK;
  unsigned int rightFBK;
  unsigned int leftDAC;
  unsigned int rightDAC;
} Feedback;

class Radio: public RFM69_ATC {
public:
  Radio(uint8_t slaveSelectPin, uint8_t interruptPin):
  RFM69_ATC(slaveSelectPin, interruptPin,true, digitalPinToInterrupt(interruptPin)){
  }

  void radioSetup(uint8_t nodeID,bool autoPowe);
};
#endif
