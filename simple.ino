// Some simple 1D Animation tests for FastLED compatible devices.
// Author: Rich Searle - hypertrifle.com

#include "FastLED.h"      //http://fastled.io/
#include <Easing.h>       //https://github.com/tobiastoft/ArduinoEasing

// Neopixel / LED strip pin.
#define PIN            6

//for use of potentiometer input.
#define POT_PIN        2

#define NUMPIXELS     31 // 144
#define MAX_BRIGHTNESS 64
#define MIN_BRIGHTNESS 0

//fastLED display
CRGB display[NUMPIXELS];


float t = 0; // our "time" input
int spread = 4; //size of the lit area, this is still scaled by the max brightness and the easing function. // 35 works for all lit but pulsing.

int min_hue = 128;
int max_hue = 228;

int hue_sweep = 0;

int hue_change_interval = 1;
float loop_interval = 1; //

int delay_interval = 0;
int delay_amount = 0;

enum Animations {  WIPE, RANDOM, DEV, FULL, PULSE };
enum EaseType { Sine, Quint, Quad, Cubic, Quart, Expo, Circ, Bounce};

Animations mode = DEV;

void setup() {
  randomSeed(analogRead(0));
  FastLED.addLeds<NEOPIXEL, PIN>(display, NUMPIXELS);
  randomise();
}

//im rather embarased about this next bit, was written in a hurry.
int ease(int numi, int mini, int maxi,EaseType ease ){
  
  switch(ease){
    case Sine:
    return Easing::easeInOutSine( numi, mini,maxi,maxi);

    case Quint:
    return Easing::easeInOutQuint( numi, mini,maxi,maxi);

    case Quad:
    return Easing::easeInOutQuad( numi, mini,maxi,maxi);

    case Cubic:
    return Easing::easeInOutCubic( numi, mini,maxi,maxi);

    case Quart:
    return Easing::easeInOutQuart( numi, mini,maxi,maxi);

    case Circ:
    return Easing::easeInOutCirc( numi, mini,maxi,maxi);

    case Bounce:
    return Easing::easeInOutBounce( numi, mini,maxi,maxi);

    case Expo:
    return Easing::easeInOutExpo( numi, mini,maxi,maxi);
  }

}

int bounce(int t, int maxValue){

  int length = t % (maxValue*2);
  if(length >= maxValue){
    length = maxValue - (length - maxValue);
  }

  return length;

}

void randomise() {
 
  //lets choose an animation types

  int r = random(0,5);

  switch(r){
    case 1:
      mode = RANDOM;
      break;

    case 2:
      mode = FULL;
      break;

    case 3:
      mode = PULSE;
      break;

    default:
      mode = WIPE;
  }



  //we probably want to ease all of the values towards the lower values.
  spread = random(1,7); //Easing::easeInSine(random(1,36),1,36,36);
  switch(spread){
    case 4 :
      spread = (mode == RANDOM)? 3 : 35;
      break;
    case 5 :
      spread = (mode == RANDOM)? 2 : 10;
    break;
    case 6 :
      spread = (mode == RANDOM)? 1 : 80;
    break;

  }

  //not sure what to do to these yet, to keep as an int i can only slow it down from 1, 
  hue_change_interval = random(1,10);

  if(hue_change_interval > 7){
    hue_change_interval = hue_change_interval * 10;
  }

  hue_sweep = random(0,4);

  loop_interval = random(1,10);

  if(loop_interval > 7){
    loop_interval = loop_interval * 10;
  }



  //set the ease types?
  delay_interval = 1;
  delay_amount = 200 /delay_interval; // (int) random(4000,6000) / delay_interval;
  
  //lets generate a randome hue range
  min_hue = random(0,256);

  if(random(0,4) == 0){
    min_hue = 0;
    max_hue = 255;
  } else {
    max_hue = min_hue + min(80,random(0, 255 - min_hue));
    
  }


  t =  0;
  
  //switch between "step" itterations.

  for(int i=0;i<NUMPIXELS;i++){
      display[i] = CHSV( 0, 0, 0);
  }
  delay(100);

}



void loop() {

  t += 0.1;

  if(t > delay_amount){
    t = 0;
    randomise();
  }




  int step = 0;

  
  switch(mode){

    case DEV:

    break;

    case RANDOM:
    step = random(0,NUMPIXELS);
    break;

    case WIPE:
      // calculate the step of the display (twice the length)
      step =  (int)(t*loop_interval)%(NUMPIXELS*2);
      // apply our bounce effect.
      if(step >= NUMPIXELS){
        step = NUMPIXELS - (step - NUMPIXELS);
      }
    break;
  }
  



  // lets ease our display postition.
  // Sine Quint Quad Cubic Quart Expo Circ Elastic Back Bounce

  step =  Easing::easeInOutSine( step, 0,NUMPIXELS,NUMPIXELS+1);
   

   //now lets set our pixels
  for(int i=0;i<NUMPIXELS;i++){
    
    //ease this between a colour range based on the distance?.
    int hue = bounce(t*hue_change_interval + (i*hue_sweep), max_hue - min_hue);
    //offset
    hue += min_hue;


    
    int saturation = 255; //lower will be paler/whiter light.

    int dist = abs(step - i); //an absolute distance from the current 'step'

    int brightness = 0;

    switch(mode){

      
      case PULSE:
         brightness = bounce((int) ((t*loop_interval)), MAX_BRIGHTNESS);
        break;
      case FULL:
        brightness = MAX_BRIGHTNESS;
        break;
      default:
        brightness = Easing::easeInQuad( max(0, spread - dist) , 0,MAX_BRIGHTNESS, spread );
        break;
    }


    if(brightness > 0){
      brightness = max(MIN_BRIGHTNESS,brightness);
    }



    display[i] = CHSV( hue, saturation, brightness);


    

  }
  FastLED.show(); 
  delay(delay_interval);
}
