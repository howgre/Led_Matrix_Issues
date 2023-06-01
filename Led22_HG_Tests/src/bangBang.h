#include <Arduino.h>
#include <LEDMatrix_22.h>


void BangBangSquares()
{
  uint32_t tim = 0;
 
  leds.clear();
  int RandColour = random(255);
  int RandStartPos = random(160);
    int RandColour2 = random(255);
    int RandStartPos2 = random(160);
  //fadeToBlackBy(leds[0], NUM_LEDS, 220);
  tim = millis();
  for (int i = 0; i < 50; i++){
  while(millis()<tim+200){
    leds.drawFilledRectangle(RandStartPos-1,0,RandStartPos+8,7,CHSV(RandColour, 255,160)); 
    leds.drawFilledRectangle(RandStartPos2-1,0,RandStartPos2+8,7,CHSV(RandColour2, 255,160)); 
  leds.show();
  }
  
  }
  
}
