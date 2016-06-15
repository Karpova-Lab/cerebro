void clicked(){
  Serial.println(F("clicked"));
  delay(100);
  isEditMode = !isEditMode;
  letter = 0;
  draw(isEditMode);
}

void left(){
  Serial.println(F("left"));
  if (!isEditMode){
  highlight=highlight>>1;
    if (highlight<1){
      highlight=1;
    }
  }
  else{
    pos = getPos();
    if(altStep){
      if (params[pos]>stepAlt[pos-4]){
        params[pos] = params[pos] - stepAlt[pos-4];
      }
      else{
        params[pos] = 0;
      }
    }
    else{
      if (params[pos]>step[pos-4]){
        params[pos] = params[pos] - step[pos-4];
      }
      else{
        params[pos] = 0;
      }
    }
  }
  tempOld = tempNow;
  draw(isEditMode);
}

void right(){
  Serial.println(F("right"));
  if (!isEditMode){
    highlight=highlight<<1;
    if (highlight>16){
      highlight=16;
    }
  }
  else{
    pos = getPos();
    if(altStep){
      if (params[pos]<65535-stepAlt[pos-4]){
        params[pos] = params[pos] + stepAlt[pos-4];
      }
      else{
        params[pos] = 65535;
      }
    }
    else{
      if (params[pos]<65535-step[pos-4]){
        params[pos] = params[pos] + step[pos-4];
      }
      else{
        params[pos] = 65535;
      }
    }
  }
  tempOld = tempNow;
  draw(isEditMode);
}
