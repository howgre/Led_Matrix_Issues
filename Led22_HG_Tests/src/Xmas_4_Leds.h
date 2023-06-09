/* 
* Copyright (c) 2015-2017 Pololu Corporation.  For more information, see
 * http://www.pololu.com/
 * http://forum.pololu.com/
 *
 * support Pololu by frequenting their website
 *
 * modified for the LEDMatrix_22 library 2022

 * This example has a series of fun patterns for use with LED
 * strips set up as a Christmas lighting display.  You can see an
 * earlier version of this code running in this YouTube video:
 * https://www.youtube.com/watch?v=VZRN0UrQSlc
 *
 * You can optionally connect a switch between pin A0 and 5v
 * to control if the Arduino automatically cycles through the
 * different patterns.  When no switch is present or the switch
 * is open, the patterns will cycle.
 *
 * You can also optionally connect a button A1 and 5v
 * ground that displays the next pattern in the series each time
 * it is pushed. */

/* This example is meant for controlling large numbers of LEDs,
 * so it requires the FastGPIO library.  If you cannot use the
 * FastGPIO library, you can comment out the two lines below and
 * the example will still work, but it will be slow. */

/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>
#include <Arduino.h>
/*   Activation Switches
This example includes buttons to hold a pattern and a 2nd to switch patterns.
These use Arduino's analopg input
*/




#define NEXT_PATTERN_BUTTON_PIN  25  // button between this pin and ground
#define AUTOCYCLE_SWITCH_PIN  26  // switch between this pin and ground

// Create a buffer for holding LED_COUNT colors.
#define LED_COUNT MATRIX_WIDTH * MATRIX_HEIGHT

#define NUM_STATES  7  // number of patterns to cycle through

// system timer, incremented by one every time through the main loop
unsigned int loopCount = 0;

unsigned int seed = 0;  // used to initialize random number generator

// enumerate the possible patterns in the order they will cycle
enum Pattern {
  WarmWhiteShimmer = 0,
  RandomColorWalk = 1,
  TraditionalColors = 2,
  ColorExplosion = 3,
  Gradient = 4,
  BrightTwinkle = 5,
  Collision = 6,
  AllOff = 255
};
unsigned char pattern = AllOff;
unsigned int maxLoops;  // go to next state when loopCount >= maxLoops












/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/





// This function detects if the optional next pattern button is pressed
// (connecting the pin to ground) and advances to the next pattern
// in the cycle if so.  It also debounces the button.
void handleNextPatternButton()
{
  if (digitalRead(NEXT_PATTERN_BUTTON_PIN) == 0)
  {
    // if optional button is pressed
    while (digitalRead(NEXT_PATTERN_BUTTON_PIN) == 0)
    {
      // wait for button to be released
      while (digitalRead(NEXT_PATTERN_BUTTON_PIN) == 0);
      delay(10);  // debounce the button
    }
    loopCount = 0;  // reset timer
    pattern = ((unsigned char)(pattern+1))%NUM_STATES;  // advance to next pattern
  }
}


// This function applies a random walk to val by increasing or
// decreasing it by changeAmount or by leaving it unchanged.
// val is a pointer to the byte to be randomly changed.
// The new value of val will always be within [0, maxVal].
// A walk direction of 0 decreases val and a walk direction of 1
// increases val.  The directions argument specifies the number of
// possible walk directions to choose from, so when directions is 1, val
// will always decrease; when directions is 2, val will have a 50% chance
// of increasing and a 50% chance of decreasing; when directions is 3,
// val has an equal chance of increasing, decreasing, or staying the same.
void randomWalk(unsigned char *val, unsigned char maxVal, unsigned char changeAmount, unsigned char directions)
{
  unsigned char walk = random(directions);  // direction of random walk
  if (walk == 0)
  {
    // decrease val by changeAmount down to a min of 0
    if (*val >= changeAmount)
    {
      *val -= changeAmount;
    }
    else
    {
      *val = 0;
    }
  }
  else if (walk == 1)
  {
    // increase val by changeAmount up to a max of maxVal
    if (*val <= maxVal - changeAmount)
    {
      *val += changeAmount;
    }
    else
    {
      *val = maxVal;
    }
  }
}


// This function fades val by decreasing it by an amount proportional
// to its current value.  The fadeTime argument determines the
// how quickly the value fades.  The new value of val will be:
//   val = val - val*2^(-fadeTime)
// So a smaller fadeTime value leads to a quicker fade.
// If val is greater than zero, val will always be decreased by
// at least 1.
// val is a pointer to the byte to be faded.
void fade(unsigned char *val, unsigned char fadeTime)
{
  if (*val != 0)
  {
    unsigned char subAmt = *val >> fadeTime;  // val * 2^-fadeTime
    if (subAmt < 1)
      subAmt = 1;  // make sure we always decrease by at least 1
    *val -= subAmt;  // decrease value of byte pointed to by val
  }
}





// ***** PATTERN RandomColorWalk *****
// This function randomly changes the color of every seventh LED by
// randomly increasing or decreasing the red, green, and blue components
// by changeAmount (capped at maxBrightness) or leaving them unchanged.
// The two preceding and following LEDs are set to progressively dimmer
// versions of the central color.  The initializeColors argument
// determines how the colors are initialized:
//   0: randomly walk the existing colors
//   1: set the LEDs to alternating red and green segments
//   2: set the LEDs to random colors
// When true, the dimOnly argument changes the random walk into a 100%
// chance of LEDs getting dimmer by changeAmount; this can be used for
// a fade-out effect.
void randomColorWalk(unsigned char initializeColors, unsigned char dimOnly)
{
  const uint8_t maxBrightness = 255;  // cap on LED brightness
  const uint8_t changeAmount = 3;  // size of random walk step

  // pick a good starting point for our pattern so the entire strip
  // is lit well (if we pick wrong, the last four LEDs could be off)
  unsigned char start;
  switch (LED_COUNT % 7)
  {
    case 0:
      start = 3;
      break;
    case 1:
      start = 0;
      break;
    case 2:
      start = 1;
      break;
    default:
      start = 2;
  }

  for (int i = start; i < LED_COUNT; i+=7)
  {
    if (initializeColors == 0)
    {
      // randomly walk existing colors of every seventh LED
      // (neighboring LEDs to these will be dimmer versions of the same color)
      randomWalk(&leds.cLED[i].r, maxBrightness, changeAmount, dimOnly ? 1 : 3);
      randomWalk(&leds.cLED[i].g, maxBrightness, changeAmount, dimOnly ? 1 : 3);
      randomWalk(&leds.cLED[i].b, maxBrightness, changeAmount, dimOnly ? 1 : 3);
    }
    else if (initializeColors == 1)
    {
      // initialize LEDs to alternating red and green
      if (i % 2)
      {
        leds.cLED[i] = CRGB(maxBrightness, 0, 0);
      }
      else
      {
        leds.cLED[i] = CRGB(0, maxBrightness, 0);
      }
    }
    else
    {
      // initialize LEDs to a string of random colors
      leds.cLED[i] = CRGB(random(maxBrightness), random(maxBrightness), random(maxBrightness));
    }

    // set neighboring LEDs to be progressively dimmer versions of the color we just set
    if (i >= 1)
    {
      leds.cLED[i-1] = CRGB(leds.cLED[i].r >> 2, leds.cLED[i].g >> 2, leds.cLED[i].b >> 2);
    }
    if (i >= 2)
    {
      leds.cLED[i-2] = CRGB(leds.cLED[i].r >> 3, leds.cLED[i].g >> 3, leds.cLED[i].b >> 3);
    }
    if (i + 1 < LED_COUNT)
    {
      leds.cLED[i+1] = leds.cLED[i-1];
    }
    if (i + 2 < LED_COUNT)
    {
      leds.cLED[i+2] = leds.cLED[i-2];
    }
  }
}


// ***** PATTERN TraditionalColors *****
// This function creates a repeating patern of traditional Christmas
// light colors: red, green, orange, blue, magenta.
// Every fourth LED is colored, and the pattern slowly moves by fading
// out the current set of lit LEDs while gradually brightening a new
// set shifted over one LED.
void traditionalColors()
{
  // loop counts to leave strip initially dark
  const unsigned char initialDarkCycles = 10;
  // loop counts it takes to go from full off to fully bright
  const unsigned char brighteningCycles = 20;

  if (loopCount < initialDarkCycles)  // leave strip fully off for 20 cycles
  {
    return;
  }

  // if LED_COUNT is not an exact multiple of our repeating pattern size,
  // it will not wrap around properly, so we pick the closest LED count
  // that is an exact multiple of the pattern period (20) and is not smaller
  // than the actual LED count.
  unsigned int extendedLEDCount = (((LED_COUNT-1)/20)+1)*20;

  for (unsigned int i = 0; i < extendedLEDCount; i++)
  {
    unsigned char brightness = (loopCount - initialDarkCycles)%brighteningCycles + 1;
    unsigned char cycle = (loopCount - initialDarkCycles)/brighteningCycles;

    // transform i into a moving idx space that translates one step per
    // brightening cycle and wraps around
    unsigned int idx = (i + cycle)%extendedLEDCount;
    if (idx < LED_COUNT)  // if our transformed index exists
    {
      if (i % 4 == 0)
      {
        // if this is an LED that we are coloring, set the color based
        // on the LED and the brightness based on where we are in the
        // brightening cycle
        switch ((i/4)%5)
        {
           case 0:  // red
             leds.cLED[idx].r = 200 * brightness/brighteningCycles;
             leds.cLED[idx].g = 10 * brightness/brighteningCycles;
             leds.cLED[idx].b = 10 * brightness/brighteningCycles;
             break;
           case 1:  // green
             leds.cLED[idx].r = 10 * brightness/brighteningCycles;
             leds.cLED[idx].g = 200 * brightness/brighteningCycles;
             leds.cLED[idx].b = 10 * brightness/brighteningCycles;
             break;
           case 2:  // orange
             leds.cLED[idx].r = 200 * brightness/brighteningCycles;
             leds.cLED[idx].g = 120 * brightness/brighteningCycles;
             leds.cLED[idx].b = 0 * brightness/brighteningCycles;
             break;
           case 3:  // blue
             leds.cLED[idx].r = 10 * brightness/brighteningCycles;
             leds.cLED[idx].g = 10 * brightness/brighteningCycles;
             leds.cLED[idx].b = 200 * brightness/brighteningCycles;
             break;
           case 4:  // magenta
             leds.cLED[idx].r = 200 * brightness/brighteningCycles;
             leds.cLED[idx].g = 64 * brightness/brighteningCycles;
             leds.cLED[idx].b = 145 * brightness/brighteningCycles;
             break;
        }
      }
      else
      {
        // fade the 3/4 of LEDs that we are not currently brightening
        fade(&leds.cLED[idx].r, 3);
        fade(&leds.cLED[idx].g, 3);
        fade(&leds.cLED[idx].b, 3);
      }
    }
  }
  
}


// Helper function for adjusting the colors for the BrightTwinkle
// and ColorExplosion patterns.  Odd colors get brighter and even
// colors get dimmer.
void brightTwinkleColorAdjust(unsigned char *color)
{
  if (*color == 255)
  {
    // if reached max brightness, set to an even value to start fade
    *color = 254;
  }
  else if (*color % 2)
  {
    // if odd, approximately double the brightness
    // you should only use odd values that are of the form 2^n-1,
    // which then gets a new value of 2^(n+1)-1
    // using other odd values will break things
    *color = *color * 2 + 1;
  }
  else if (*color > 0)
  {
    fade(color, 4);
    if (*color % 2)
    {
      (*color)--;  // if faded color is odd, subtract one to keep it even
    }
  }
}


// Helper function for adjusting the colors for the ColorExplosion
// pattern.  Odd colors get brighter and even colors get dimmer.
// The propChance argument determines the likelihood that neighboring
// LEDs are put into the brightening stage when the central LED color
// is 31 (chance is: 1 - 1/(propChance+1)).  The neighboring LED colors
// are pointed to by leftColor and rightColor (it is not important that
// the leftColor LED actually be on the "left" in your setup).
void colorExplosionColorAdjust(unsigned char *color, unsigned char propChance,
 unsigned char *leftColor, unsigned char *rightColor)
{
  if (*color == 31 && random(propChance+1) != 0)
  {
    if (leftColor != 0 && *leftColor == 0)
    {
      *leftColor = 1;  // if left LED exists and color is zero, propagate
    }
    if (rightColor != 0 && *rightColor == 0)
    {
      *rightColor = 1;  // if right LED exists and color is zero, propagate
    }
  }
  brightTwinkleColorAdjust(color);
}


// ***** PATTERN ColorExplosion *****
// This function creates bursts of expanding, overlapping colors by
// randomly picking LEDs to brighten and then fade away.  As these LEDs
// brighten, they have a chance to trigger the same process in
// neighboring LEDs.  The color of the burst is randomly chosen from
// among red, green, blue, and white.  If a red burst meets a green
// burst, for example, the overlapping portion will be a shade of yellow
// or orange.
// When true, the noNewBursts argument changes prevents the generation
// of new bursts; this can be used for a fade-out effect.
// This function uses a very similar algorithm to the BrightTwinkle
// pattern.  The main difference is that the random twinkling LEDs of
// the BrightTwinkle pattern do not propagate to neighboring LEDs.
void colorExplosion(unsigned char noNewBursts)
{
  // adjust the colors of the first LED
  colorExplosionColorAdjust(&leds.cLED[0].r, 9, (unsigned char*)0, &leds.cLED[1].r);
  colorExplosionColorAdjust(&leds.cLED[0].g, 9, (unsigned char*)0, &leds.cLED[1].g);
  colorExplosionColorAdjust(&leds.cLED[0].b, 9, (unsigned char*)0, &leds.cLED[1].b);

  for (int i = 1; i < LED_COUNT - 1; i++)
  {
    // adjust the colors of second through second-to-last LEDs
    colorExplosionColorAdjust(&leds.cLED[i].r, 9, &leds.cLED[i-1].r, &leds.cLED[i+1].r);
    colorExplosionColorAdjust(&leds.cLED[i].g, 9, &leds.cLED[i-1].g, &leds.cLED[i+1].g);
    colorExplosionColorAdjust(&leds.cLED[i].b, 9, &leds.cLED[i-1].b, &leds.cLED[i+1].b);
  }

  // adjust the colors of the last LED
  colorExplosionColorAdjust(&leds.cLED[LED_COUNT-1].r, 9, &leds.cLED[LED_COUNT-2].r, (unsigned char*)0);
  colorExplosionColorAdjust(&leds.cLED[LED_COUNT-1].g, 9, &leds.cLED[LED_COUNT-2].g, (unsigned char*)0);
  colorExplosionColorAdjust(&leds.cLED[LED_COUNT-1].b, 9, &leds.cLED[LED_COUNT-2].b, (unsigned char*)0);

  if (!noNewBursts)
  {
    // if we are generating new bursts, randomly pick one new LED
    // to light up
    for (int i = 0; i < 1; i++)
    {
      int j = random(LED_COUNT);  // randomly pick an LED

      switch(random(7))  // randomly pick a color
      {
        // 2/7 chance we will spawn a red burst here (if LED has no red component)
        case 0:
        case 1:
          if (leds.cLED[j].r == 0)
          {
            leds.cLED[j].r = 1;
          }
          break;

        // 2/7 chance we will spawn a green burst here (if LED has no green component)
        case 2:
        case 3:
          if (leds.cLED[j].g == 0)
          {
            leds.cLED[j].g = 1;
          }
          break;

        // 2/7 chance we will spawn a white burst here (if LED is all off)
        case 4:
        case 5:
          if ((leds.cLED[j].r == 0) && (leds.cLED[j].g == 0) && (leds.cLED[j].b == 0))
          {
            leds.cLED[j] = CRGB(1, 1, 1);
          }
          break;

        // 1/7 chance we will spawn a blue burst here (if LED has no blue component)
        case 6:
          if (leds.cLED[j].b == 0)
          {
            leds.cLED[j].b = 1;
          }
          break;

        default:
          break;
      }
    }
  }
}


// ***** PATTERN Gradient *****
// This function creates a scrolling color gradient that smoothly
// transforms from red to white to green back to white back to red.
// This pattern is overlaid with waves of brightness and dimness that
// scroll at twice the speed of the color gradient.
void gradient()
{
  unsigned int j = 0;

  // populate colors array with full-brightness gradient colors
  // (since the array indices are a function of loopCount, the gradient
  // colors scroll over time)
  while (j < LED_COUNT)
  {
    // transition from red to green over 8 LEDs
    for (int i = 0; i < 8; i++)
    {
      if (j >= LED_COUNT){ break; }
      leds.cLED[(loopCount/2 + j + LED_COUNT)%LED_COUNT] = CRGB(160 - 20*i, 20*i, (160 - 20*i)*20*i/160);
      j++;
    }
    // transition from green to red over 8 LEDs
    for (int i = 0; i < 8; i++)
    {
      if (j >= LED_COUNT){ break; }
      leds.cLED[(loopCount/2 + j + LED_COUNT)%LED_COUNT] = CRGB(20*i, 160 - 20*i, (160 - 20*i)*20*i/160);
      j++;
    }
  }

  // modify the colors array to overlay the waves of dimness
  // (since the array indices are a function of loopCount, the waves
  // of dimness scroll over time)
  const unsigned char fullDarkLEDs = 10;  // number of LEDs to leave fully off
  const unsigned char fullBrightLEDs = 5;  // number of LEDs to leave fully bright
  const unsigned char cyclePeriod = 14 + fullDarkLEDs + fullBrightLEDs;

  // if LED_COUNT is not an exact multiple of our repeating pattern size,
  // it will not wrap around properly, so we pick the closest LED count
  // that is an exact multiple of the pattern period (cyclePeriod) and is not
  // smaller than the actual LED count.
  unsigned int extendedLEDCount = (((LED_COUNT-1)/cyclePeriod)+1)*cyclePeriod;

  j = 0;
  while (j < extendedLEDCount)
  {
    unsigned int idx;

    // progressively dim the LEDs
    for (int i = 1; i < 8; i++)
    {
      idx = (j + loopCount) % extendedLEDCount;
      if (j++ >= extendedLEDCount){ return; }
      if (idx >= LED_COUNT){ continue; }

      leds.cLED[idx].r >>= i;
      leds.cLED[idx].g >>= i;
      leds.cLED[idx].b >>= i;
    }

    // turn off these LEDs
    for (int i = 0; i < fullDarkLEDs; i++)
    {
      idx = (j + loopCount) % extendedLEDCount;
      if (j++ >= extendedLEDCount){ return; }
      if (idx >= LED_COUNT){ continue; }

      leds.cLED[idx].r = 0;
      leds.cLED[idx].g = 0;
      leds.cLED[idx].b = 0;
    }

    // progressively bring these LEDs back
    for (int i = 0; i < 7; i++)
    {
      idx = (j + loopCount) % extendedLEDCount;
      if (j++ >= extendedLEDCount){ return; }
      if (idx >= LED_COUNT){ continue; }

      leds.cLED[idx].r >>= (7 - i);
      leds.cLED[idx].g >>= (7 - i);
      leds.cLED[idx].b >>= (7 - i);
    }

    // skip over these LEDs to leave them at full brightness
    j += fullBrightLEDs;
  }
}


// ***** PATTERN BrightTwinkle *****
// This function creates a sparkling/twinkling effect by randomly
// picking LEDs to brighten and then fade away.  Possible colors are:
//   white, red, green, blue, yellow, cyan, and magenta
// numColors is the number of colors to generate, and minColor
// indicates the starting point (white is 0, red is 1, ..., and
// magenta is 6), so colors generated are all of those from minColor
// to minColor+numColors-1.  For example, calling brightTwinkle(2, 2, 0)
// will produce green and blue twinkles only.
// When true, the noNewBursts argument changes prevents the generation
// of new twinkles; this can be used for a fade-out effect.
// This function uses a very similar algorithm to the ColorExplosion
// pattern.  The main difference is that the random twinkling LEDs of
// this BrightTwinkle pattern do not propagate to neighboring LEDs.
void brightTwinkle(unsigned char minColor, unsigned char numColors, unsigned char noNewBursts)
{
  // Note: the colors themselves are used to encode additional state
  // information.  If the color is one less than a power of two
  // (but not 255), the color will get approximately twice as bright.
  // If the color is even, it will fade.  The sequence goes as follows:
  // * Randomly pick an LED.
  // * Set the color(s) you want to flash to 1.
  // * It will automatically grow through 3, 7, 15, 31, 63, 127, 255.
  // * When it reaches 255, it gets set to 254, which starts the fade
  //   (the fade process always keeps the color even).
  for (int i = 0; i < LED_COUNT; i++)
  {
    brightTwinkleColorAdjust(&leds.cLED[i].r);
    brightTwinkleColorAdjust(&leds.cLED[i].g);
    brightTwinkleColorAdjust(&leds.cLED[i].b);
  }

  if (!noNewBursts)
  {
    // if we are generating new twinkles, randomly pick four new LEDs
    // to light up
    for (int i = 0; i < 4; i++)
    {
      int j = random(LED_COUNT);
      if (leds.cLED[j].r == 0 && leds.cLED[j].g == 0 && leds.cLED[j].b == 0)
      {
        // if the LED we picked is not already lit, pick a random
        // color for it and seed it so that it will start getting
        // brighter in that color
        switch (random(numColors) + minColor)
        {
          case 0:
            leds.cLED[j] = CRGB(1, 1, 1);  // white
            break;
          case 1:
            leds.cLED[j] = CRGB(1, 0, 0);  // red
            break;
          case 2:
            leds.cLED[j] = CRGB(0, 1, 0);  // green
            break;
          case 3:
            leds.cLED[j] = CRGB(0, 0, 1);  // blue
            break;
          case 4:
            leds.cLED[j] = CRGB(1, 1, 0);  // yellow
            break;
          case 5:
            leds.cLED[j] = CRGB(0, 1, 1);  // cyan
            break;
          case 6:
            leds.cLED[j] = CRGB(1, 0, 1);  // magenta
            break;
          default:
            leds.cLED[j] = CRGB(1, 1, 1);  // white
        }
      }
    }
  }
}


// ***** PATTERN Collision *****
// This function spawns streams of color from each end of the strip
// that collide, at which point the entire strip flashes bright white
// briefly and then fades.  Unlike the other patterns, this function
// maintains a lot of complicated state data and tells the main loop
// when it is done by returning 1 (a return value of 0 means it is
// still in progress).
unsigned char collision()
{
  const uint8_t maxBrightness = 160;  // max brightness for the colors
  const uint8_t numCollisions = 5;  // # of collisions before pattern ends
  static uint8_t state = 0;  // pattern state
  static uint8_t count = 0;  // counter used by pattern

  if (loopCount == 0)
  {
    state = 0;
  }

  if (state % 3 == 0)
  {
    // initialization state
    switch (state/3)
    {
      case 0:  // first collision: red streams
        leds.cLED[0] = CRGB(maxBrightness, 0, 0);
        break;
      case 1:  // second collision: green streams
        leds.cLED[0] = CRGB(0, maxBrightness, 0);
        break;
      case 2:  // third collision: blue streams
        leds.cLED[0] = CRGB(0, 0, maxBrightness);
        break;
      case 3:  // fourth collision: warm white streams
        leds.cLED[0] = CRGB(maxBrightness, maxBrightness*4/5, maxBrightness>>3);
        break;
      default:  // fifth collision and beyond: random-color streams
        leds.cLED[0] = CRGB(random(maxBrightness), random(maxBrightness), random(maxBrightness));
    }

    // stream is led by two full-white LEDs
    leds.cLED[1] = leds.cLED[2] = CRGB(255, 255, 255);
    // make other side of the strip a mirror image of this side
    leds.cLED[LED_COUNT - 1] = leds.cLED[0];
    leds.cLED[LED_COUNT - 2] = leds.cLED[1];
    leds.cLED[LED_COUNT - 3] = leds.cLED[2];

    state++;  // advance to next state
    count = 8;  // pick the first value of count that results in a startIdx of 1 (see below)
    return 0;
  }

  if (state % 3 == 1)
  {
    // stream-generation state; streams accelerate towards each other
    unsigned int startIdx = count*(count + 1) >> 6;
    unsigned int stopIdx = startIdx + (count >> 5);
    count++;
    if (startIdx < (LED_COUNT + 1)/2)
    {
      // if streams have not crossed the half-way point, keep them growing
      for (unsigned int i = 0; i < startIdx-1; i++)
      {
        // start fading previously generated parts of the stream
        fade(&leds.cLED[i].r, 5);
        fade(&leds.cLED[i].g, 5);
        fade(&leds.cLED[i].b, 5);
        fade(&leds.cLED[LED_COUNT - i - 1].r, 5);
        fade(&leds.cLED[LED_COUNT - i - 1].g, 5);
        fade(&leds.cLED[LED_COUNT - i - 1].b, 5);
      }
      for (unsigned int i = startIdx; i <= stopIdx; i++)
      {
        // generate new parts of the stream
        if (i >= (LED_COUNT + 1) / 2)
        {
          // anything past the halfway point is white
          leds.cLED[i] = CRGB(255, 255, 255);
        }
        else
        {
          leds.cLED[i] = leds.cLED[i-1];
        }
        // make other side of the strip a mirror image of this side
        leds.cLED[LED_COUNT - i - 1] = leds.cLED[i];
      }
      // stream is led by two full-white LEDs
      leds.cLED[stopIdx + 1] = leds.cLED[stopIdx + 2] = CRGB(255, 255, 255);
      // make other side of the strip a mirror image of this side
      leds.cLED[LED_COUNT - stopIdx - 2] = leds.cLED[stopIdx + 1];
      leds.cLED[LED_COUNT - stopIdx - 3] = leds.cLED[stopIdx + 2];
    }
    else
    {
      // streams have crossed the half-way point of the strip;
      // flash the entire strip full-brightness white (ignores maxBrightness limits)
      for (int i = 0; i < LED_COUNT; i++)
      {
        leds.cLED[i] = CRGB(255, 255, 255);
      }
      state++;  // advance to next state
    }
    return 0;
  }

  if (state % 3 == 2)
  {
    // fade state
    if (leds.cLED[0].r == 0 && leds.cLED[0].g == 0 && leds.cLED[0].b == 0)
    {
      // if first LED is fully off, advance to next state
      state++;

      // after numCollisions collisions, this pattern is done
      return state >= 3*numCollisions;
    }

    // fade the LEDs at different rates based on the state
    for (int i = 0; i < LED_COUNT; i++)
    {
      switch (state/3)
      {
        case 0:  // fade through green
          fade(&leds.cLED[i].r, 3);
          fade(&leds.cLED[i].g, 4);
          fade(&leds.cLED[i].b, 2);
          break;
        case 1:  // fade through red
          fade(&leds.cLED[i].r, 4);
          fade(&leds.cLED[i].g, 3);
          fade(&leds.cLED[i].b, 2);
          break;
        case 2:  // fade through yellow
          fade(&leds.cLED[i].r, 4);
          fade(&leds.cLED[i].g, 4);
          fade(&leds.cLED[i].b, 3);
          break;
        case 3:  // fade through blue
          fade(&leds.cLED[i].r, 3);
          fade(&leds.cLED[i].g, 2);
          fade(&leds.cLED[i].b, 4);
          break;
        default:  // stay white through entire fade
          fade(&leds.cLED[i].r, 4);
          fade(&leds.cLED[i].g, 4);
          fade(&leds.cLED[i].b, 4);
      }
    }
  }

  return 0;
}

// ***** PATTERN WarmWhiteShimmer *****
// This function randomly increases or decreases the brightness of the
// even red LEDs by changeAmount, capped at maxBrightness.  The green
// and blue LED values are set proportional to the red value so that
// the LED color is warm white.  Each odd LED is set to a quarter the
// brightness of the preceding even LEDs.  The dimOnly argument
// disables the random increase option when it is true, causing
// all the LEDs to get dimmer by changeAmount; this can be used for a
// fade-out effect.
void warmWhiteShimmer(unsigned char dimOnly)
{
    for (int i = 0; i < LED_COUNT; i += 2)
    {

        const uint8_t maxBrightness = 255;
        const uint8_t changeAmount = 3;  // size of random walk step

        // randomly walk the brightness of every even LED
        randomWalk(&leds.cLED[i].r, maxBrightness, changeAmount, dimOnly ? 1 : 2);

         // warm white: red = x, green = 0.8x, blue = 0.125x
        leds.cLED[i].g = leds.cLED[i].r * 4 / 5;  // green = 80% of red
        leds.cLED[i].b = leds.cLED[i].r >> 3;  // blue = red/8

        // every odd LED gets set to a quarter the brighness of the preceding even LED
        if (i + 1 < LED_COUNT)
        {
            leds.cLED[i + 1] = CRGB(leds.cLED[i].r >> 2, leds.cLED[i].g >> 2, leds.cLED[i].b >> 2);
        }
    }
}

// main loop
void xmasLoop()
{
  uint8_t startTime = millis();

  handleNextPatternButton();                    //check buttons
 
  if (loopCount == 0)
  {
    // whenever timer resets, clear the LED colors array (all off)
      leds.clear();
  }

  if (pattern == WarmWhiteShimmer || pattern == RandomColorWalk)
  {
    // for these two patterns, we want to make sure we get the same
    // random sequence six times in a row (this provides smoother
    // random fluctuations in brightness/color)
    if (loopCount % 6 == 0)
    {
      seed = random(30000);
    }
    randomSeed(seed);
  }

  // call the appropriate pattern routine based on state; these
  // routines just set the colors in the colors array
  switch (pattern)
  {
    case WarmWhiteShimmer:
      // warm white shimmer for 300 loopCounts, fading over last 70
      maxLoops = 300;
      warmWhiteShimmer(loopCount > maxLoops - 70);
      break;

    case RandomColorWalk:
      // start with alternating red and green colors that randomly walk
      // to other colors for 400 loopCounts, fading over last 80
      maxLoops = 400;
      randomColorWalk(loopCount == 0 ? 1 : 0, loopCount > maxLoops - 80);
      break;

    case TraditionalColors:
      // repeating pattern of red, green, orange, blue, magenta that
      // slowly moves for 400 loopCounts
      maxLoops = 400;
      traditionalColors();
      break;

    case ColorExplosion:
      // bursts of random color that radiate outwards from random points
      // for 630 loop counts; no burst generation for the last 70 counts
      // of every 200 count cycle or over the over final 100 counts
      // (this creates a repeating bloom/decay effect)
      maxLoops = 630;
      colorExplosion((loopCount % 200 > 130) || (loopCount > maxLoops - 100));
      break;

    case Gradient:
      // red -> white -> green -> white -> red ... gradiant that scrolls
      // across the strips for 250 counts; this pattern is overlaid with
      // waves of dimness that also scroll (at twice the speed)
      maxLoops = 250;
      gradient();
      delay(6);  // add an extra 6ms delay to slow things down
      break;

    case BrightTwinkle:
      // random LEDs light up brightly and fade away; it is a very similar
      // algorithm to colorExplosion (just no radiating outward from the
      // LEDs that light up); as time goes on, allow progressively more
      // colors, halting generation of new twinkles for last 100 counts.
      maxLoops = 1200;
      if (loopCount < 400)
      {
        brightTwinkle(0, 1, 0);  // only white for first 400 loopCounts
      }
      else if (loopCount < 650)
      {
        brightTwinkle(0, 2, 0);  // white and red for next 250 counts
      }
      else if (loopCount < 900)
      {
        brightTwinkle(1, 2, 0);  // red, and green for next 250 counts
      }
      else
      {
        // red, green, blue, cyan, magenta, yellow for the rest of the time
        brightTwinkle(1, 6, loopCount > maxLoops - 100);
      }
      break;

    case Collision:
      // colors grow towards each other from the two ends of the strips,
      // accelerating until they collide and the whole strip flashes
      // white and fades; this repeats until the function indicates it
      // is done by returning 1, at which point we stop keeping maxLoops
      // just ahead of loopCount
      if (!collision())
      {
        maxLoops = loopCount + 2;
      }
      break;
  }
 
  // update the LED strips with the colors in the colors array
  leds.show();

  // Make sure that loops are not too fast there are a small number
  // of LEDs and the colors are calculated quickly.
  while((uint8_t)(millis() - startTime) < 20) { }
  loopCount++;  // increment our loop counter/timer.

  if (loopCount >= maxLoops && digitalRead(AUTOCYCLE_SWITCH_PIN))
  {
    // if the time is up for the current pattern and the optional hold
    // switch is not grounding the AUTOCYCLE_SWITCH_PIN, clear the
    // loop counter and advance to the next pattern in the cycle
    loopCount = 0;  // reset timer
    pattern = ((unsigned char)(pattern+1))%NUM_STATES;  // advance to next pattern
  }

}