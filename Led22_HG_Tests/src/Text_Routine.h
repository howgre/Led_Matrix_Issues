



#include <Arduino.h>
#include <LEDMatrix_22.h>


char  text1[]=" Hello Fiona  ";
char text2[] = "A bright new world"; 
void HelloFiona(){
   leds.clear();
  
   
  
  leds.setTextColor(CHSV(50,30,50));
  leds.setTextWrap(1);
  leds.setTextSize(1);
  leds.setCursor(10,0);
  leds.print(text1);
  leds.show();
 // delay(1000); 

  
}

void brightNewWorld(){
    leds.clear();

    leds.setTextWrap(1);
    leds.setTextSize(1);
    for (int i= MATRIX_WIDTH-1; i >= 0; i --){
            leds.setTextColor(CHSV(150,230,50));
            leds.setCursor(i,0);
            leds.print(text2);
            leds.show();
            delay(2);
            //if (i=0) {delay(10000);} else {  delay(2);}
            //Serial.print("Text preceding i = ");
            //Serial.println(i);

            
            
            leds.clear();
            //leds.setCursor(i,0);
            //leds.setTextColor(CHSV(150,0,0));
            //leds.print(text2);
            //leds.show();
            //delay(50);

    }
    
    
    
   // delay(1000); 
}