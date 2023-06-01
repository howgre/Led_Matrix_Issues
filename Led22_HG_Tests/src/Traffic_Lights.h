/*
As the name suggests, this is a sketch for controlling the
leds as a series of traffic lights (U.K.) sequence.
It is taken from GitHub Gist simonarnell/trafficLight.ino
and modified for led matrices, rather than using individual leds.

It worked first time!!!

*/




#include <Arduino.h>
#include <LEDMatrix_22.h>

void replicate() { // copies the first panel to the next 4
    for (int i = 0; i<=192; i++ ){
      leds.cLED[i+192] = leds.cLED[i];
      leds.cLED[i+192*2] = leds.cLED[i];
      leds.cLED[i+192*3] = leds.cLED[i];
      leds.cLED[i+192*4] = leds.cLED[i];
      leds.cLED[i+192*5] = leds.cLED[i];
    }
}

void RedLightOn() {
    leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 100)); // 1 is Red
    leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 0)); // 50 is Amber
    leds.drawFilledCircle(20, 4, 3, CHSV(100, 255, 0)); // 100 is Green
    replicate();
    
    leds.show();
}

void RedLightOff() {
    //leds.clear();
    leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 0)); // 1 is Red
    replicate();
    leds.show();
}
void RedAmberLightOn(){
  leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 100)); // 1 is Red
  leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 100)); // 50 is Amber
  leds.drawFilledCircle(20, 4, 3, CHSV(100, 255, 0)); // 100 is Green
  replicate();
  leds.show();
}
void RedAmberLightOff(){
  leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 0)); // 1 is Red
  leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 0)); // 50 is Amber
  replicate();
  leds.show();
}

void GreenLightOn(){
  leds.drawFilledCircle(20, 4, 3, CHSV(100, 255, 100)); // 100 is Green
  leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 0)); // 1 is Red
  leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 0)); // 50 is Amber
  replicate();
  leds.show();
}
void GreenLightOff(){
  leds.drawFilledCircle(20, 4, 3, CHSV(100, 255, 0)); // 100 is Green
  replicate();
  leds.show();
}

void AmberLightOn(){
  leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 100)); // 50 is Amber
  leds.drawFilledCircle(4, 4, 3, CHSV(1, 255, 0)); // 1 is Red
  leds.drawFilledCircle(20, 4, 3, CHSV(100, 255, 0)); // 100 is Green
  replicate();
  leds.show();
}

void AmberLightOff(){
  leds.drawFilledCircle(12, 4, 3, CHSV(50, 255, 0)); // 50 is Amber
  replicate();
  leds.show();
}




enum phases {
 red, redChanging, green, amber
};

phases phase, nextPhase;
unsigned long lightsTimer = 0;




void traffic_Lights() {
  if(millis() >= lightsTimer) {
    phase = nextPhase;
    switch(phase) {
      case red:
        RedLightOn();
        nextPhase = redChanging;
        lightsTimer = millis() + 1000UL;
                                    
        break;
      case redChanging:
        RedAmberLightOn();
        nextPhase = green;
        lightsTimer = millis() + 300UL;
        break;
      case amber:
        AmberLightOn();
        nextPhase = red;
        lightsTimer = millis() + 500UL;
        break;
      case green:
        GreenLightOn();
        nextPhase = amber;
        lightsTimer = millis() + 1000UL;
        break;
      
      default:
        RedLightOn();
      break;
    }
  }
}









/*

enum phases {
 red, redChanging, green, amber
};

phases phase, nextPhase;
unsigned long lightsTimer = 0;




void traffic_Lights() {
  if(millis() >= lightsTimer) {
    phase = nextPhase;
    switch(phase) {
      case red:
        RedLightOn();
        nextPhase = redChanging;
        lightsTimer = millis() + 1000UL;
                                    
        break;
      case redChanging:
        RedAmberLightOn();
        nextPhase = green;
        lightsTimer = millis() + 300UL;
        break;
      case amber:
        AmberLightOn();
        nextPhase = red;
        lightsTimer = millis() + 500UL;
        break;
      case green:
        GreenLightOn();
        nextPhase = amber;
        lightsTimer = millis() + 1000UL;
        break;
      
      default:
        RedLightOn();
      break;
    }
  }
}


*/