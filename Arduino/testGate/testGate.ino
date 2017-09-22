// Sample RFM69 receiver/gateway sketch, with ACK and optional encryption, and Automatic Transmission Control
// Passes through any wireless received messages to the serial port & responds to ACKs
// It also looks for an onboard FLASH chip, if present
// **********************************************************************************
// Copyright Felix Rusu 2016, http://www.LowPowerLab.com/contact
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************
#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NODEID        1    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_915MHZ
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************
#define SERIAL_BAUD   115200
#define BR_300KBPS             //run radio at max rate of 300kbps!
#define LED           13 // Moteinos have LEDs on D9

#ifdef ENABLE_ATC
  // RFM69_ATC radio;
  RFM69_ATC radio(8, 7,true, digitalPinToInterrupt(7));  //slave select,interrupt pin, isRFM69HW, interruptNum
#else
  RFM69 radio;
#endif


void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
  radio.encrypt(null);  
  radio.promiscuous(false);
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);

#ifdef BR_300KBPS
  radio.writeReg(0x03, 0x00);  //REG_BITRATEMSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x04, 0x6B);  //REG_BITRATELSB: 300kbps (0x006B, see DS p20)
  radio.writeReg(0x19, 0x40);  //REG_RXBW: 500kHz
  radio.writeReg(0x1A, 0x80);  //REG_AFCBW: 500kHz
  radio.writeReg(0x05, 0x13);  //REG_FDEVMSB: 300khz (0x1333)
  radio.writeReg(0x06, 0x33);  //REG_FDEVLSB: 300khz (0x1333)
  radio.writeReg(0x29, 240);   //set REG_RSSITHRESH to -120dBm
#endif

  pinMode(LED, OUTPUT);
}
unsigned int timeSent = 0;
void loop() {
  timeSent = micros();  
  if (radio.sendWithRetry(12, "T", 8, 0)){  // 0 = only 1 attempt, no retries
    timeSent = micros()-timeSent;
    Serial.print(" trip: ");Serial.println(timeSent);
  }
  else{
     Serial.println("nothing");
  }
  delay(1000);
  if (radio.receiveDone())
  {
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);
    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
    
    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      Serial.print(" - ACK sent.");
    }
    Serial.println();
    Blink(LED,3);
  }
}

void Blink(byte PIN, int msDelay)
{
  digitalWrite(PIN,HIGH);
  delay(msDelay);
  digitalWrite(PIN,LOW);
}

  // int currPeriod = millis()/TRANSMITPERIOD;
  // if (currPeriod != lastPeriod)
  // {
  //   lastPeriod=currPeriod;
  //   Serial.print("Sending[");
  //   Serial.print(sendSize);
  //   Serial.print("]: ");
  //   for(byte i = 0; i < sendSize; i++)
  //     Serial.print((char)payload[i]);
  //   timeSent = micros();
  //   if (radio.sendWithRetry(GATEWAYID, payload, 1)){

  //   }
  //   else{
  //     Serial.print(" no response :(");
  //   } 
  //   sendSize = (sendSize + 1) % 31;
  //   Serial.println();
  //   Blink(LED,3);
  // }