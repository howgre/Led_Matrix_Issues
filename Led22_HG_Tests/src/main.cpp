#include <Arduino.h>
#include <LEDMatrix_22.h>

#include <LEDText.h>
#include <FontMatrise.h>
#include <FontRobotron.h>


/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/


cLEDMatrix leds;

//lets simply the code a bit with these substitutions
//cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

CRGB* cleds = leds.cLED;   //pointer to your leds[] array to access directly
#define XY(x, y) leds.mXY(x, y) //this sketch adresses all leds directly
#include <Text_Routine.h>
#include <One_Sine_Pal.h>
#include <AA_lines.h>
#include <Traffic_Lights.h>
#include <AaronTextTest.h>
#include <bangBang.h>
#include <Curve_Canvas.h>
#include <Sinusoid.h>
#include <Xmas_4_Leds.h>



void routineChange(){
  uint32_t tim = 0;
  
  tim = millis();
  while(millis()<tim+10000){
    aaron();
  }
  leds.clear();
  
 
 
 
 
  tim = millis();
  while(millis()<tim+10000){
    differentMessage();
  }
  leds.clear();


  /*
  tim = millis();
  while(millis()<tim+10000){
    traditionalColors();
  }
  leds.clear();
  */

  tim = millis();
  while(millis()<tim+10000){
  traffic_Lights();
  }
  leds.clear();
  
  tim = millis();
  while(millis()<tim+45000){
    xmasLoop();
    
  }
  leds.clear();


  tim = millis();
  while(millis()<tim+5000){
    Sinusoidal();
    
  }
  leds.clear();


  tim = millis();
  while(millis()<tim+5000){
    draw();
    leds.show();
  }
  leds.clear();

  tim = millis();
  while(millis()<tim+15000){
    BangBangSquares();
    
  }
  leds.clear();
  
  tim = millis();
  while(millis()<tim+15000){
    One_Sine_Routine();
  }
  leds.clear();
  
  tim = millis();
  while(millis()<tim+10000){
    AA_Lines();
  }
  fadeToBlackBy(cleds, NUM_LEDS, 100);
  leds.clear();
  
  /*
  tim=millis();
  while(millis()<tim+15000){
    aaron();
  }
  leds.clear();
  */
  
}


void setup() {

    //Serial.begin(115200);
    

    leds.addLeds();       //initiate LEDMatrix
    
    leds.clear();
    delay(30);
    
    ScrollingMsg.SetFont(MatriseFontData);
    ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
    ScrollingMsg.SetText((unsigned char *)TxtDemo2, sizeof(TxtDemo2) - 1);
    ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0x66);
        

     // initialize the random number generator with a seed obtained by
  // summing the voltages on the disconnected analog inputs
  for (int i = 0; i < 8; i++)
  {
    seed += analogRead(i);
  }

  randomSeed(seed);
  // save a random number in EEPROM to be used for random seed
  // generation the next time the program runs
 
  // optionally connect a switch between this pin and ground
  // when the input is low, freeze the cycle at the current pattern
  pinMode(AUTOCYCLE_SWITCH_PIN, INPUT_PULLUP);

  // optionally connect a button between this pin and ground
  // when the input goes low, advance to the next pattern in cycle
  pinMode(NEXT_PATTERN_BUTTON_PIN, INPUT_PULLUP);


}




void loop(){
  
  routineChange();
  
 
  
  //Diamondhole();
  //leds.show();
  
  //traffic_Lights();
  
  //HelloFiona();
  
  //brightNewWorld();
 
 
}


