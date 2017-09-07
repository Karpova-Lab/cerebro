void characterizeDiode(){
  // triggerEvent(50); //warmup
  // delay(60000);
  // triggerEvent(50); //~ 1mW
  // delay(60000);
  // triggerEvent(70); //~1.5 mW
  // delay(60000);
  // triggerEvent(90); //~2 mW

}
//
void characterizeImplant(){
  // bool firstMax = true;
  // unsigned int dlay = 4000;
  // delay(dlay);
  // for (int testLevel = 500; testLevel<751; testLevel+=50){
  //   triggerEvent(testLevel);
  //   delay(dlay);
  // }
  // for (int testLevel = 760; testLevel<901; testLevel+=10){
  //   triggerEvent(testLevel);
  //   delay(dlay);
  // }
  // for (int testLevel = 905; testLevel<1026; testLevel+=5){
  //   Serial.print(testLevel);
	// 	Serial.print(",");
  //   if(!isMaxed){
  //     triggerEvent(testLevel);
  //     delay(dlay);
  //   }
  //   else if(firstMax){  //trigger once more after being maxed
  //     triggerEvent(testLevel);
  //     firstMax = false;
  //   }
  //   else{
  //     //do nothing
  //   }
  // }
  // isMaxed = false; //reset isMaxed
}

void triggerEvent(unsigned int desiredPower,bool whichLaser,bool useFeedback){
  int select;
  int photoSignal;
  if (whichLaser==LEFT){
    select = oneSelect;
    photoSignal = photoONE;
  }
  else{
    select = twoSelect;
    photoSignal = photoTWO;
  }
  isSettled = false;
  unsigned long onClock,offClock,trainClock,delayClock,alt=0;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  bool triggerRecorded = false;
  delayClock=millis();              //reset clocks
  byte rcvd = 0;
  unsigned int onDelay,onTime,offTime,trainDur,rampDur;
  if (diodeMode){
    onDelay = 0;
    onTime = 2000;
    offTime = 6000;
    trainDur = 57000;
    rampDur = 0;
  }
  else if (implantMode){
    onDelay = 0;
    onTime = 1000;
    offTime = 0;
    trainDur = 0;
    rampDur = 0;
  }
  else{
    onDelay = 0;
    onTime = 1000;
    offTime = 0;
    trainDur = 0;
    rampDur = 0;
    // onDelay = waveform[ON_DELAY];
    // onTime = waveform[ON_TIME];
    // offTime = waveform[OFF_TIME];
    // trainDur = waveform[TRAIN_DUR];
    // rampDur  = waveform[RAMP_DUR];
  }
  if (onDelay>0){
    while ((millis()-delayClock)<waveform[ON_DELAY]){
      // if (!(IR_inputReg & (1<<IR_pin))){
      //   while(! (IR_inputReg & (1<<IR_pin))){
      //     //wait until stop pulse is finished
      //   }
      //   rcvd = listenForIR(5000);
      //   if (stopMatch){
      //     laserEnabled = laserOFF();
      //     if (address < memorySize) {       //record abort event
      //       recordEvent('A');
      //     }
      //     stopMatch = false;
      //   }
      //   rcvd = 0;
      // }
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){   
    // //check if another command (abort or continuation) has been sent since the trigger was activated
    // if (!(IR_inputReg & (1<<IR_pin))){
    //   while(! (IR_inputReg & (1<<IR_pin))){
    //     //wait until stop pulse is finished
    //   }
    //   rcvd = listenForIR(5000);
    //   if (trigMatch){
    //     onClock = millis();
    //     trainClock = millis();
    //     if (address < memorySize) {       //record continue event
    //       recordEvent('C');
    //     }
    //     trigMatch = false;
    //   }
    //   else if (stopMatch){
    //     recordLightLevel(getLightLevel());
    //     if (rampDur>0){
    //       fade();
    //     }
    //     laserEnabled = laserOFF();
    //     if (address < memorySize) {       //record abort event
    //       recordEvent('A');
    //     }
    //     stopMatch = false;
    //   }
    //   rcvd = 0;
    // }
    //else if onClock hasn't expired, turn on/keep on the laser
    // else if ((millis()-onClock)<onTime){
    if ((millis()-onClock)<onTime){
        
      if(!useFeedback){
        sendDAC(desiredPower,select);
      }
      else{
        sendDAC(DAClevel,select);              //Laser on
        if(alt%interval==0){              //it takes time for the photocell to respond, so only implement feedback every fourth loop
          feedback(desiredPower,photoSignal);         //increase or decrease DAClevel to reach desired lightPower
        }
        alt++;
      }
      if (!triggerRecorded){           //event has not yet been recorded
        if (address < memorySize) {    //record trigger event
          recordEvent('T');
        }
        triggerRecorded = true;
      }
      offClock = millis();
    }
    //else if offClock hasn't expired, turn off/keep off light
    else if((millis()-offClock)<offTime){
      if (newPulse){                   //if the laser is on then turn it off, otherwise do nothing (i.e. leave turned off)
        newPulse = laserOFF(select,photoSignal);         //laserOn = false
      }
    }
    //else if trainClock hasn't expired, restart the light pulse
    else if((millis()-trainClock)<trainDur){
      newPulse = true;
      onClock = millis();
    }
    //else the end of the waveform has been reached. turn off the light.
    else{
      recordLightLevel(getLightLevel());
      if (useFeedback){
        if (rampDur>0 && !implantMode && !diodeMode && !powerTestMode){
          fade();
        }
      }
      laserEnabled = laserOFF(select,photoSignal);
    }
  }
}

void feedback(int setPoint, int phototransistor ){
  int photocellVoltage = analogRead(phototransistor);
  error = setPoint-photocellVoltage;
  DAClevel = DAClevel+int(error*KP);
  if (DAClevel>4095) {
    DAClevel = 4095;
  }
  else if (DAClevel<0){
    DAClevel = 0;
  }
}

int getLightLevel(){
  ADCSRA |= (1<<ADSC);                    //start analog conversion
  loop_until_bit_is_clear(ADCSRA,ADSC);   //wait until conversion is done
  return ADC;
}

void fade(){
  // unsigned long fadeClock;
  // unsigned int param1;
  // for (int k = FADE_START; k < FADE_START+200 ; k+=2) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
  //   fadeClock = millis();
  //   param1 = eepromReadByte(k)<<8;
  //   feedback(word(param1|eepromReadByte(k+1)));
  //   sendDAC(DAClevel);
  //   while((millis()-fadeClock)<(waveform[RAMP_DUR]/100)){
  //     //wait
  //   }
  // }
}

void sendDAC(unsigned int value, int slaveSelect) {
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelect,LOW);  
  SPI.transfer(48);                        //Write to and Update (Power up) DAC Register command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(value>>4);                  //shift high byte
  SPI.transfer(value<<4 & 255);            //shift low byte
  digitalWrite(slaveSelect,HIGH);
  SPI.endTransaction();
}

bool laserOFF(int slaveSelect , int phototransistor){
  Serial.println("laser off");
  Serial.print(analogRead(phototransistor));
  Serial.print(',');
  Serial.println(DAClevel);
  SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelect,LOW);
  SPI.transfer(64);                        //Power down command (page 13, table 1 of LTC2630-12 datasheet)
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(slaveSelect,HIGH);
  #ifndef MCUBE
  if (DAClevel==4095){
    isMaxed = true;
  }
  DAClevel = 0;
  #endif
  SPI.endTransaction();   
  return false;
}
