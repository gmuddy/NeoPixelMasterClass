#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define pix 45

#define NEOspeed 8         //milliseconds until the next neopixel is in front. (lower is faster)
#define NEOtail  32         //length of fading tail (must be below pix count)
//#define NEOtail  (analogRead(1)/(1024/pix))         //length of fading tail (must be below pix count)
#define NEOcolor 0          //0-1023
#define NEOcolorChange 10   //milliseconds until color change increment (lower is faster)
#define NEOcolorIncrement 1 //color change increment (set it to 0 for no color change)
#define NEOcolorFade 44     //color difference along the tail

#define NEObrightness 255      //0-255
//#define NEObrightness (analogRead(0)/4)  //or you can use a knob to control brightness

#define NEOdirection 1   // -1 for other direction

Adafruit_NeoPixel strip(pix, LED_PIN, NEO_GRB + NEO_KHZ800);

int tail;
int dir;

void setup() {
  dir=NEOdirection;
  strip.begin();
  strip.show();
  tail=min(NEOtail,pix)-1;
}

long unsigned timer;
long unsigned speedTimer;
long unsigned colorFadeTimer;
long unsigned dirTimer;

int color=NEOcolor;
int pixel=0;

void loop() {
  timer=millis();
  
  if(timer>speedTimer){
    pixel=(pixel+pix+dir);
    pixel%=pix;
    for(int a=0;a<tail;a++){
      setPixel((pixel+pix)+(a*dir),color+(a*NEOcolorFade),(((255/tail)*a)*NEObrightness)/255);
    }
    strip.show();
    for(int a=0;a<pix;a++){
      strip.setPixelColor(a,0,0,0);
    }
    speedTimer+=NEOspeed;
  }

  if(timer>colorFadeTimer){
    color+=NEOcolorIncrement;
    color%=1024;
    colorFadeTimer+=NEOcolorChange;
  }
  if(timer>dirTimer){
    dirTimer+=random(5000)+1000;
    dir=0-dir;
  }
  
}


void setPixel(int pixelz,double colorz, long unsigned fadez){
  fadez*=fadez;  //these two lines turn the dimming 
  fadez/=255;    //from linear to logarithmic
  pixelz%=pix;
  strip.setPixelColor(pixelz,strip.ColorHSV(colorz*64,255,fadez));
}
