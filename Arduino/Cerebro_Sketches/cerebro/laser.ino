void triggerEvent(unsigned int desiredPower,LaserDiode* thediode,bool useFeedback){
  unsigned long onClock,offClock,trainClock,delayClock,alt=0;
  bool laserEnabled = true; //set flag for entering waveform loop
  bool newPulse = true;      //
  delayClock=millis();              //reset clocks
  unsigned int onDelay,onTime,offTime,trainDur,rampDur;
  onDelay = 0;
  onTime = 1000;
  offTime = 0;
  trainDur = 0;
  rampDur = 0;

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
        thediode->sendDAC(desiredPower);              //Laser on
        if(alt%interval==0){              //it takes time for the photocell to respond, so only implement feedback every fourth loop
          thediode->feedback(desiredPower);      //increase or decrease DAClevel to reach desired lightPower
        }
        alt++;
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
          thediode->fade();
        }
      }
      Serial.print("\tduring: ");  
      feedbackReadings();
      laserEnabled = thediode->off();
    }
  }
}
