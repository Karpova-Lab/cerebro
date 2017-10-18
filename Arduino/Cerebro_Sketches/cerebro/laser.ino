int triggerEvent(unsigned int desiredPower,LaserDiode* thediode, bool useFeedback){
  unsigned long onClock,offClock,trainClock,delayClock;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  delayClock=millis();              //reset clocks
  int _meterValue;
  if (waveform.startDelay>0){
    while ((millis()-delayClock)<waveform.startDelay){
      //wait. be ready to stop if interrupted.
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){   
    //check if another command (abort or continuation) has been sent since the trigger was activated
    // reset clock on continuation. or abort waveform.
    if (radio.receiveDone()){
      if (radio.DATALEN==1){ 
        if (radio.DATA[0]=='A'){  //abort
          laserEnabled = thediode->off();
          sendACK();      
        }
        else if (radio.DATA[0]=='C'){ //continuation
          onClock=trainClock=millis();     
          sendACK();     
        }
      }
    }
    //else if onClock hasn't expired, turn on/keep on the laser
    else if ((millis()-onClock)<waveform.onTime){
      if(!useFeedback){
        thediode->sendDAC(desiredPower);
      }
      else{
        thediode->sendDAC(thediode->DAClevel);              //Laser on
        thediode->feedback(desiredPower);      //increase or decrease DAClevel to reach desired lightPower
      }
      offClock = millis();
    }
    //else if offClock hasn't expired, turn off/keep off light
    else if((millis()-offClock)<waveform.offTime){
      if (newPulse){                   //if the laser is on then turn it off, otherwise do nothing (i.e. leave turned off)
        newPulse = thediode->off();         //laserOn = false
      }
    }
    //else if trainClock hasn't expired, restart the light pulse
    else if((millis()-trainClock)<waveform.trainDur){
      newPulse = true;
      onClock = millis();
    }
    //else the end of the waveform has been reached. turn off the light.
    else{
      if (useFeedback){
        if (waveform.rampDur>0){
          thediode->fade(waveform.rampDur);
        }
      }
      // Serial.println("during: ");  
      // feedbackReadings();
      // _meterValue = analogRead(powerMeter);   
      // Serial.print("power meter: ");Serial.println(_meterValue);     
      reportLaserStats();
      laserEnabled = thediode->off();
    }
  }
  return _meterValue;
}

void triggerBoth(){
  unsigned long onClock,offClock,trainClock,delayClock;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  delayClock=millis();              //reset clocks
  if (waveform.startDelay>0){
    while ((millis()-delayClock)<waveform.startDelay){
      //wait. be ready to stop if interrupted.
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){   
    //check if another command (abort or continuation) has been sent since the trigger was activated
    if (radio.receiveDone()){
      if (radio.DATALEN == sizeof(radioMessage)){
        radioMessage = *(IntegerPayload*)radio.DATA;
        switch (radioMessage.variable){
          case 'A':
            left.off();
            laserEnabled = right.off();
            checkForMiss();
            break;
          case 'C':
            onClock=trainClock=millis();   
            checkForMiss(); 
            break;
        }
      }
    }
    //else if onClock hasn't expired, turn on/keep on the laser
    else if ((millis()-onClock)<waveform.onTime){
      right.sendDAC(right.DAClevel);
      right.feedback(right.setPoint); //increase or decrease DAClevel to reach desired lightPower
      left.sendDAC(left.DAClevel);
      left.feedback(left.setPoint);
      offClock = millis();
    }
    //else if offClock hasn't expired, turn off/keep off light
    else if((millis()-offClock)<waveform.offTime){
      if (newPulse){                   //if the laser is on then turn it off, otherwise do nothing (i.e. leave turned off)
        newPulse = left.off();         //laserOn = false
        newPulse = right.off();         //laserOn = false
      }
    }
    //else if trainClock hasn't expired, restart the light pulse
    else if((millis()-trainClock)<waveform.trainDur){
      newPulse = true;
      onClock = millis();
    }
    //else the end of the waveform has been reached. turn off the light.
    else{
      if (waveform.rampDur>0){
        unsigned long fadeClock;
        for (int i = 99; i>-1;i--) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
          fadeClock = millis();
          left.feedback(left.setPoint*i/100);
          left.sendDAC(left.DAClevel);
          right.feedback(right.setPoint*i/100);
          right.sendDAC(right.DAClevel);
          while((millis()-fadeClock)<(waveform.rampDur/100)){
            //wait
          }
        }
      }
      reportLaserStats();     
      left.off(); 
      laserEnabled = right.off();
      if (lipo.capacity(REMAIN)<20){
        reportBattery();
      }
      //send alert if feedbacks aren't what were expected
    }
  }
}

void reportLaserStats(){
  // Read battery stats from the BQ27441-G1A
  diodeStats.leftFBK = analogRead(left.analogPin);
  diodeStats.rightFBK = analogRead(right.analogPin);
  diodeStats.leftDAC = left.DAClevel;
  diodeStats.rightDAC = right.DAClevel;
  
  if (radio.sendWithRetry(1, (const void*)(&diodeStats), sizeof(diodeStats))){
    Serial.println("laser stats sent");
  }
  else{
    Serial.println("laser stats send fail");
  }
}