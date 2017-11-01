// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "FastLED.h"
#include <Easing.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

#define POT_PIN        2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     31 // 144

#define MAX_BRIGHTNESS 64
#define MIN_BRIGHTNESS 0



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.

//fastLED display
CRGB display[NUMPIXELS];
int t = 0; // our "time" input
int spread = 4; //size of the lit area, this is still scaled by the max brightness and the easing function. // 35 works for all lit but pulsing.

int min_hue = 128;
int max_hue = 228;

int hue_change_interval = 1;
float loop_interval = 1; //

int delay_interval = 0;
int delay_amount = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(display, NUMPIXELS);
  randomise();
}

void randomise() {
  randomSeed(analogRead(0));

  //we probably want to ease all of the values towards the lower values.
  spread = random(1,36); //Easing::easeInSine(random(1,36),1,36,36);

  // loopLength = random(50,300) / 100;

  //set t as to start in a random location in the coulr cycle.
  
  hue_change_interval = Easing::easeInSine(random(1,30),1,30,30);
  loop_interval = 1;

  //set the ease types?

  delay_interval = max(1,random(0,5)*10);


  delay_amount = (int) random(4000,6000) / delay_interval;


  int colour_range_type = random(0,3);

  switch(colour_range_type){
    case 0:
      min_hue = 0; //full coulor spectrum.
      max_hue = 255;
      break;

    default:
      min_hue = 128; //this is our ASTHETIC
      max_hue = 228;
      break;
  }


  t =  random(0,delay_amount);
  
  //switch between "step" itterations.

}



void loop() {

  t ++;

  if(t > delay_amount){
    t = 0;
    randomise();
  }


  int step = 0;

  
  switch(0){

    default:
      // calculate the step of the display (twice the length)
      step =  (int)(t*loop_interval)%(NUMPIXELS*2);
      // apply our bounce effect.
      if(step >= NUMPIXELS){
        step = NUMPIXELS - (step - NUMPIXELS);
      }
      break;
  }
  



  // lets ease our left to right position.
  // Sine Quint Quad Cubic Quart Expo Circ Elastic Back Bounce
  step =  Easing::easeInOutSine( step, 0,NUMPIXELS,NUMPIXELS+1);

   //now lets set our pixels
  for(int i=0;i<NUMPIXELS;i++){

    int dist = abs(step - i);
    
    //ease this between a colour range based on the distance?.
    int hue = min_hue + (t*hue_change_interval) % (max_hue - min_hue); //TODO: we need to bounce this to make it smooth.

    int saturation = 255; //lower will be paler - cooler white toward the center?.

    int brightness = Easing::easeInQuad( max(0, spread - dist) , 0,MAX_BRIGHTNESS, spread );

    if(brightness > 0){
      brightness = max(MIN_BRIGHTNESS,brightness);
    }



    display[i] = CHSV( hue, saturation, brightness);


    

  }
  FastLED.show(); 
  delay(delay_interval);
}
