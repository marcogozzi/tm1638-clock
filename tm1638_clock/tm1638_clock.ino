#include <TM1638.h>

#define DATAPIN 8
#define CLKPIN 7
#define STBPIN 6

unsigned long waitcheckTime=0;
unsigned long waitcheckButtons=0;

unsigned long intervalcheckTime=1000;
unsigned long intervalcheckButtons=100;

unsigned long gapSecond=0;
boolean dots=0;

boolean moduleOff=0;
byte intensity=0;
byte pos=2;

TM1638 tm = TM1638(DATAPIN, CLKPIN, STBPIN, true, intensity);

void setup(){
  waitcheckTime = intervalcheckTime;
  waitcheckButtons = intervalcheckButtons;
}

void loop(){
  checkTime();
  checkButtons();
}

void checkTime(){
  if (millis() >= waitcheckTime) {
    dots = !dots;
    drawToModule();
    waitcheckTime += intervalcheckTime;
  }
}

void checkButtons(){
  if (millis() >= waitcheckButtons) {
    tm.setLEDs(0);
    byte buttons=tm.getButtons();
    if(buttons>0){
      for (byte i=0;i<8;i++){
        if ((buttons & 1<<i) != 0) {
          buttonEvent(i);
          waitcheckButtons +=intervalcheckButtons;
          drawToModule();
        }
      }
    }
    waitcheckButtons +=intervalcheckButtons;
  }
}

void drawToModule(){
  if (!moduleOff){
    unsigned long elapSecond = round(millis() / 1000);
    unsigned long totalSecond = gapSecond + elapSecond;
    tm.clearDisplay();
    tm.setDisplayToString(formatTime(totalSecond),(dots * 80),pos);
  }
}


void buttonEvent(byte inp){
  switch (inp) {
  case 7:
    intensity = ( intensity + 1 ) % 8;
    tm.setupDisplay(true, intensity);
    tm.setLEDs((1 << (intensity + 1)) - 1);
    return;
  case 0:
    if (hour(gapSecond) != 23)    {  
      gapSecond += 3600    ; 
    }
    else      {
      gapSecond -= 82800;
    }
    break;
  case 1:
    if (hour(gapSecond) != 0){
      gapSecond -= 3600;
    }
    else{
      gapSecond += 82800;
    }
    break;
  case 2:
    if (minute(gapSecond) != 59){
      gapSecond += 60;
    }
    else{
      gapSecond -= 3540;
    }
    break;
  case 3:
    if (minute(gapSecond) != 0){
      gapSecond -= 60;
    }
    else{
      gapSecond += 3540;
    }
    break;
  case 4:
    gapSecond -= second(gapSecond + round(millis() / 1000)); 
    break;
  case 5:
    moduleOff = !moduleOff;
    if (moduleOff) tm.clearDisplay();
    break;
  case 6:
    pos = (pos + 1) % 3;
    break;
  }
  tm.setLED(TM1638_COLOR_GREEN + TM1638_COLOR_RED, inp);
}


String formatTime(unsigned long inp){
  return hourStr(inp) + minuteStr(inp) + secondStr(inp);
}

String formatNumber(String inp){
  if (inp.length()==1) {
    return "0" + inp;
  }
  else{
    return inp;
  }
}

String hourStr(unsigned long inp){
  return  formatNumber(String(hour(inp)));
}

String minuteStr(unsigned long inp){
  return  formatNumber(String(minute(inp)));
}

String secondStr(unsigned long inp){
  return  formatNumber(String(second(inp)));
}

byte hour(unsigned long inp){
  return  byte((inp/3600) % 24);
}

byte minute(unsigned long inp){
  return  byte((inp/60) % 60);
}

byte second(unsigned long inp){
  return  byte(inp % 60);
}









