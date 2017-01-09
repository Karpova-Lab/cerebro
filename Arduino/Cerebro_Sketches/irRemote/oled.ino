void displaySetup(){
   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
//  testdrawchar(mode);
  draw(isEditMode);
  display.display();
}

void draw(bool _mode) {
  display.setTextColor(1,0);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println();
  display.println();
  display.println();
  if (!_mode){ //highlight parameters
    for (int i = 4; i<9; i++){
      if(!isViewingParameters){
        display.setTextColor(highlight!=(1<<(i-4)),highlight==(1<<(i-4)));
      }
      display.print(labels[i]);
      display.setTextColor(1,0);
      display.print(": ");
      display.print(params[i]);
      display.setTextColor(1,0);
      display.print(" ms ");
      fillExcess(params[i],5);
    }
  }
  else{ //highlight values
    for (int i = 4; i<9; i++){
      display.setTextColor(1,0);
      display.print(labels[i]);
      display.print(": ");
      if(!isViewingParameters){
        display.setTextColor(highlight!=(1<<(i-4)),highlight==(1<<(i-4)));
      }
      display.print(params[i]);
      display.setTextColor(1,0);
      display.print(" ms ");
      fillExcess(params[i],5);
    }
  }
  display.display();
}

void fillExcess(unsigned int tempParam,byte wanted){
  byte numDigits = 1;
  while(tempParam/10>0){
    numDigits++;
    tempParam = tempParam/10;
  }
  for (int i = 0; i < wanted-numDigits; i++){
    display.print(" ");
  }
}
