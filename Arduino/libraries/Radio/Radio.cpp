#include "Radio.h"

void Radio::radioSetup(uint8_t nodeID, bool autoPower, uint8_t channel){
  //*** Radio ***//
  initialize(FREQUENCY,nodeID,channel);
  if (autoPower){
    enableAutoPower(ATC_RSSI);  
  }
  encrypt(null);
  writeReg(0x03, 0x00);  //REG_BITRATEMSB: 300kbps (0x006B, see DS p20)
  writeReg(0x04, 0x6B);  //REG_BITRATELSB: 300kbps (0x006B, see DS p20)
  writeReg(0x19, 0x40);  //REG_RXBW: 500kHz
  writeReg(0x1A, 0x80);  //REG_AFCBW: 500kHz
  writeReg(0x05, 0x13);  //REG_FDEVMSB: 300khz (0x1333)
  writeReg(0x06, 0x33);  //REG_FDEVLSB: 300khz (0x1333)
  writeReg(0x29, 240);   //set REG_RSSITHRESH to -120dB
}
