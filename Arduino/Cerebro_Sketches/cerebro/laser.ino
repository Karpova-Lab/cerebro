
void characterizeRoutine(){
  bool firstMax = true;
  unsigned int dlay = 3000;
  if (!implantMode){
    dlay = 4000;
  }
  delay(dlay);
  byte initial = 70;
  byte increment = 30;
  byte maxNumDataPts = 12;
  for (byte sample = 0; sample<maxNumDataPts; sample++){
    int b = sample*increment+initial;
  // for (int b = 40; b<1023; b+=30){
		mySerial.print(b);
		mySerial.print(",");
    if(!isMaxed){
      triggerEvent(b);
      delay(dlay);
    }
    else if(firstMax){
      triggerEvent(b);
      firstMax = false;
    }
  }
  isMaxed = false; //reset isMaxed
}

void triggerEvent(unsigned int desiredPower,bool useFeedback){
  isSettled = false;
  unsigned long onClock,offClock,trainClock,delayClock,alt=0;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  bool triggerRecorded = false;
  delayClock=millis();              //reset clocks
  byte rcvd = 0;
  unsigned int onDelay,onTime,offTime,trainDur,rampDur;
  if (implantMode){
    onDelay = 0;
    onTime = 2000;
    offTime = 0;
    trainDur = 0;
    rampDur = 0;
  }
  else if(diodeMode){
    onDelay = 0;
    onTime = 1000;
    offTime = 0;
    trainDur = 0;
    rampDur = 0;
  }
  else{
    onDelay = waveform[ON_DELAY];
    onTime = waveform[ON_TIME];
    offTime = waveform[OFF_TIME];
    trainDur = waveform[TRAIN_DUR];
    rampDur  = waveform[RAMP_DUR];
  }
  if (onDelay>0){
    while ((millis()-delayClock)<waveform[ON_DELAY]){
      if (!(IR_inputReg & (1<<IR_pin))){
        while(! (IR_inputReg & (1<<IR_pin))){
          //wait until stop pulse is finished
        }
        rcvd = listenForIR(5000);
        if (stopMatch){
          laserEnabled = laserOFF();
          if (address < memorySize) {       //record abort event
            recordEvent('A');
          }
          stopMatch = false;
        }
        rcvd = 0;
      }
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){
    //check if another command (abort or continuation) has been sent since the trigger was activated
    if (!(IR_inputReg & (1<<IR_pin))){
      while(! (IR_inputReg & (1<<IR_pin))){
        //wait until stop pulse is finished
      }
      rcvd = listenForIR(5000);
      if (trigMatch){
        onClock = millis();
        trainClock = millis();
        if (address < memorySize) {       //record continue event
          recordEvent('C');
        }
        trigMatch = false;
      }
      else if (stopMatch){
        recordLightLevel(getLightLevel());
        if (rampDur>0){
          fade();
        }
        laserEnabled = laserOFF();
        if (address < memorySize) {       //record abort event
          recordEvent('A');
        }
        stopMatch = false;
      }
      rcvd = 0;
    }
    //else if onClock hasn't expired, turn on/keep on the laser
    else if ((millis()-onClock)<onTime){
      if(!useFeedback){
        sendDAC(desiredPower);
      }
      else{
        sendDAC(DAClevel);              //Laser on
        feedback(desiredPower);         //increase or decrease DAClevel to reach desired lightPower
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
        newPulse = laserOFF();         //laserOn = false
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
      if(implantMode || diodeMode){
        mySerial.print(desiredPower);
        mySerial.print(",");
      }
      laserEnabled = laserOFF();
    }
  }
}

void feedback(int setPoint){
  ADCSRA |= (1<<ADSC);                    //start analog conversion
  loop_until_bit_is_clear(ADCSRA,ADSC);   //wait until conversion is done
  int photodiodeVoltage = ADC;
  error = setPoint-photodiodeVoltage;
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
  unsigned long fadeClock;
  unsigned int param1;
  for (int k = FADE_START; k < FADE_START+200 ; k+=2) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
    fadeClock = millis();
    param1 = eepromReadByte(k)<<8;
    feedback(word(param1|eepromReadByte(k+1)));
    sendDAC(DAClevel);
    while((millis()-fadeClock)<(waveform[RAMP_DUR]/100)){
      //wait
    }
  }
}

void sendDAC(int value) {
  LATCH_outputReg &= ~(1<<LATCH_pin); //latch low selects the chip
  myShift(48);                        //Write to and Update (Power up) DAC Register command (page 13, table 1 of LTC2630-12 datasheet)
  myShift(value>>4);                  //shift high byte
  myShift(value<<4 & 255);            //shift low byte
  LATCH_outputReg |= (1<<LATCH_pin);  //latch high de-selects the chip
}

bool laserOFF(){
  ADCSRA |= (1<<ADSC);                    //start analog conversion
  loop_until_bit_is_clear(ADCSRA,ADSC);   //wait until conversion is done
  mySerial.println(ADC);
  LATCH_outputReg &= ~(1<<LATCH_pin); //latch low
  myShift(64);                        //Power down command (page 13, table 1 of LTC2630-12 datasheet)
  myShift(0);
  myShift(0);
  LATCH_outputReg |= (1<<LATCH_pin);  //latch high
  #ifndef MCUBE
  if (DAClevel==4095){
    isMaxed = true;
  }
  DAClevel = 0;
  #endif
  return false;
}
