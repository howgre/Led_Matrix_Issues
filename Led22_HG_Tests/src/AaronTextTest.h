#include <Arduino.h>
#include <LEDMatrix_22.h>
#include <LEDText.h>





cLEDText ScrollingMsg;

const unsigned char TxtDemo[] = { EFFECT_SCROLL_LEFT "            Left SCROLL "
                                  EFFECT_SCROLL_RIGHT "            LLORCS THGIR"
                                  EFFECT_SCROLL_DOWN "            SCROLL DOWN             SCROLL DOWN            " EFFECT_FRAME_RATE "\x04" " SCROLL DOWN            " EFFECT_FRAME_RATE "\x00" " "
                                  EFFECT_SCROLL_UP "             SCROLL UP               SCROLL UP             " EFFECT_FRAME_RATE "\x04" "  SCROLL UP             " EFFECT_FRAME_RATE "\x00" " "
                                  EFFECT_CHAR_UP EFFECT_SCROLL_LEFT "            UP"
                                  EFFECT_CHAR_RIGHT "  RIGHT"
                                  EFFECT_CHAR_DOWN "  DOWN"
                                  EFFECT_CHAR_LEFT "  LEFT"
                                  EFFECT_HSV_CV "\x00\xff\xff\x40\xff\xff" EFFECT_CHAR_UP "           HSV_CV 00-40"
                                  EFFECT_HSV_CH "\x00\xff\xff\x40\xff\xff" "    HSV_CH 00-40"
                                  EFFECT_HSV_AV "\x00\xff\xff\x40\xff\xff" "    HSV_AV 00-40"
                                  EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "    HSV_AH 00-FF"
                                  "           " EFFECT_HSV "\x00\xff\xff" "R" EFFECT_HSV "\x20\xff\xff" "A" EFFECT_HSV "\x40\xff\xff" "I" EFFECT_HSV "\x60\xff\xff" "N" EFFECT_HSV "\xe0\xff\xff" "B" EFFECT_HSV "\xc0\xff\xff" "O"
                                  EFFECT_HSV "\xa0\xff\xff" "W" EFFECT_HSV "\x80\xff\xff" "S " EFFECT_DELAY_FRAMES "\x00\x96" EFFECT_RGB "\xff\xff\xff" };



const unsigned char TxtDemo2[] = {
      EFFECT_SCROLL_LEFT  EFFECT_HSV_AV "\x02\xff\x88\xf5\xff\x99"  "                               Hello Fiona, am I the right way up?  "
};


const unsigned char TxtDemo3[] = {
      EFFECT_SCROLL_LEFT  EFFECT_HSV_AV "\x44\xff\x88\xe4\xff\x99"  "                     This is a different message "
};



void aaron(){
        
  ScrollingMsg.SetFont(MatriseFontData);
  {
  if (ScrollingMsg.UpdateText() == -1)
      ScrollingMsg.SetText((unsigned char *)TxtDemo2, sizeof(TxtDemo2) - 1);
    
  else
    leds.show();
  
  }
}


void differentMessage(){
        
  ScrollingMsg.SetFont(MatriseFontData);
  {
  if (ScrollingMsg.UpdateText() == -1)
      ScrollingMsg.SetText((unsigned char *)TxtDemo3, sizeof(TxtDemo3) - 1);
    
  else
    leds.show();
  
  }
}

