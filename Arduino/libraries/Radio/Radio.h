
#ifndef Radio_h
#define Radio_h
#include "Arduino.h"
#include <SPI.h>
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69

//*********************************************************************************************
#define NODEID        12    //must be unique for each node on same network (range up to 254, 255 is used for broadcast)
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

class Radio: public RFM69_ATC {
public:
  Radio():RFM69_ATC(7, 1,true, digitalPinToInterrupt(1)){
  }
  void radioSetup();
};
#endif
