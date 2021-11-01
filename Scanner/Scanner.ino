#include <Adafruit_NeoPixel.h>
#define LED_COUNT 11
#define LED_PIN 11
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(115200);
}
int fade[LED_COUNT];
int pix;
int dir=1;

long unsigned timer;
long unsigned scanTimer;
long unsigned fadeTimer;

void loop() {
  timer=millis();
  if(timer>scanTimer){
    scanTimer+=80;
    pix+=dir;
    if(pix<=0){dir=1;}
    if(pix>=LED_COUNT-1){dir=-1;}
    fade[pix]=255;
  }
  if(timer>fadeTimer){
    fadeTimer+=7;
    for(int a=0;a<LED_COUNT;a++){
      long unsigned bright=fade[a]*fade[a];
      bright/=255;
      strip.setPixelColor(a,bright,0,0);
      fade[a]=max(0,fade[a]-4);;
    }
    strip.show();
  }
}
