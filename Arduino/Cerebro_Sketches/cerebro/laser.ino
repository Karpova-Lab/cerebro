int triggerEvent(unsigned int desiredPower,LaserDiode* thediode,unsigned int rampDur, bool useFeedback){
  unsigned long onClock,offClock,trainClock,delayClock;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  delayClock=millis();              //reset clocks
  unsigned int onDelay,onTime,offTime,trainDur;
  onDelay = 0;
  onTime = 1000;
  offTime = 0;
  trainDur = 0;
  int _meterValue;
  if (onDelay>0){
    while ((millis()-delayClock)<onDelay){
      //wait. be ready to stop if interrupted.
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){   
    // //check if another command (abort or continuation) has been sent since the trigger was activated
    // reset clock on continuation. or abort waveform.
    //else if onClock hasn't expired, turn on/keep on the laser
    if ((millis()-onClock)<onTime){
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
    else if((millis()-offClock)<offTime){
      if (newPulse){                   //if the laser is on then turn it off, otherwise do nothing (i.e. leave turned off)
        newPulse = thediode->off();         //laserOn = false
      }
    }
    //else if trainClock hasn't expired, restart the light pulse
    else if((millis()-trainClock)<trainDur){
      newPulse = true;
      onClock = millis();
    }
    //else the end of the waveform has been reached. turn off the light.
    else{
      if (useFeedback){
        if (rampDur>0){
          thediode->fade(rampDur);
        }
      }
      // Serial.print("\tduring: ");  
      // feedbackReadings();
      // Serial.print(thediode->DAClevel);
      // Serial.print(",");
      // Serial.println(analogRead(thediode->analogPin));
      _meterValue = analogRead(powerMeter);         
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
  unsigned int onDelay,onTime,offTime,trainDur,rampDuration;
  onDelay = 0;
  onTime = 1000;
  offTime = 0;
  trainDur = 0;
  rampDuration = 2000;
  if (onDelay>0){
    while ((millis()-delayClock)<onDelay){
      //wait. be ready to stop if interrupted.
    }
  }
  onClock=trainClock=millis();
  while(laserEnabled){   
    // //check if another command (abort or continuation) has been sent since the trigger was activated
    // reset clock on continuation. or abort waveform.
    //else if onClock hasn't expired, turn on/keep on the laser
    if ((millis()-onClock)<onTime){
      right.sendDAC(right.DAClevel);
      right.feedback(right.setPoint); //increase or decrease DAClevel to reach desired lightPower
      left.sendDAC(left.DAClevel);
      left.feedback(left.setPoint);
      offClock = millis();
    }
    //else if offClock hasn't expired, turn off/keep off light
    else if((millis()-offClock)<offTime){
      if (newPulse){                   //if the laser is on then turn it off, otherwise do nothing (i.e. leave turned off)
        newPulse = left.off();         //laserOn = false
        newPulse = right.off();         //laserOn = false
      }
    }
    //else if trainClock hasn't expired, restart the light pulse
    else if((millis()-trainClock)<trainDur){
      newPulse = true;
      onClock = millis();
    }
    //else the end of the waveform has been reached. turn off the light.
    else{
      if (rampDuration>0){
        unsigned long fadeClock;
        for (int i = 99; i>-1;i--) {  //fade values are stored in addresses 16-216 (100 values,2 bytes each)
          fadeClock = millis();
          left.feedback(left.setPoint*i/100);
          left.sendDAC(left.DAClevel);
          right.feedback(right.setPoint*i/100);
          right.sendDAC(right.DAClevel);
          while((millis()-fadeClock)<(rampDuration/100)){
            //wait
          }
        }
      }
      // Serial.print("\tduring: ");  
      // feedbackReadings();
      // Serial.println(thediode->DAClevel);   
      Serial.print("*****");
      Serial.print(left.DAClevel);
      Serial.print("*****");      
      Serial.print(analogRead(left.analogPin)); 
      Serial.print("*****");
      Serial.print(right.DAClevel);
      Serial.print("*****");      
      Serial.println(analogRead(right.analogPin));
      laserEnabled = left.off();
      laserEnabled = right.off();

    }
  }
}
