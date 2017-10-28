// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "FastLED.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

#define POT_PIN        2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      144

#define MAX_BRIGHTNESS 32


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.

//fastLED display
CRGB display[NUMPIXELS];
int huerange = 100; // our colour range
int t = 0; // our "time" input
void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(display, NUMPIXELS);
}



void loop() {

  static uint8_t hue = 0;

  // FastLED.showColor(CHSV(hue++, 255, MAX_BRIGHTNESS)); 

  int brightness = abs(analogRead(POT_PIN) / 4);

  t ++;

  if(t > huerange*2*4){
    t -= huerange*2*4;
  }
    
  for(int i=0;i<NUMPIXELS;i++){

    // float x = ((i/NUMPIXELS)*2)-1; //convert this range to -1 - 1 (simular to GLSL);

   
    int r = abs(((t/2+i)%huerange*2) - 100)+128;


    // float rf = (x * sin(t) +1)/2;
    // float gf = 0.0;
    // float bf = 0.0;

    display[i] = CHSV( r, 255, MAX_BRIGHTNESS);
    

  }
  FastLED.show(); 
}
