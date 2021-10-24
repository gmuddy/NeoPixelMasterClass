#include <Adafruit_NeoPixel.h>
#define LED_PIN    14
#define LED_COUNT 16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

byte simon[60];
int mem;
int current;

int pinputs[4]={5,4,3,2};
int clrs[4][3]={{155,100,0},
                {0,255,0},
                {0,0,255},
                {255,0,0}};

void setup() {
  Serial.begin(115200);
  strip.begin();
  for(int a=0;a<LED_COUNT;a++){
    strip.setPixelColor(a,strip.ColorHSV(random(65536)));
    strip.show();
    delay(50);
    strip.setPixelColor(a,0,0,0);
  }
  strip.show();
  for(int a=0;a<4;a++){
    pinMode(pinputs[a],INPUT_PULLUP);
  }
  randomSeed(analogRead(2));
  clearPixels();
}

bool guess;
bool win=1;
bool latch;

void loop() {
  randomSeed(analogRead(random(5)+1));
  simon[mem]=random(4);
  Serial.println(simon[mem]);
  mem++;
  for(int a=0;a<mem;a++){
    setSimon(simon[a]);
    strip.show();
    delay(400);
    clearPixels();
    strip.show();
    delay(100);
  }
  for(int a=0;a<mem;a++){
    guess=0;
    current=a;
    while(!guess){
      for(int b=0;b<4;b++){
        if(!digitalRead(pinputs[b])&&!latch){
          guess=1;
          latch=1;
          if(b==simon[a]){
            setSimon(b);
            strip.show();
            delay(150);
            clearPixels();
            strip.show();
          } else {
            win=0;
            a=60;
          }
        }
        if(digitalRead(pinputs[b])&&latch){
          latch=0;
          delay(50);
        }
      }
    }
  }
  if(win){
    Serial.print("hooray");
    for(int a=0;a<50;a++){
      strip.setPixelColor(random(LED_COUNT),85,85,85);
      strip.show();
      delay(20);
      clearPixels();
    }
    strip.show();
    delay(500);
  }
  if(!win){
    Serial.println("loser!");
    Serial.print("memory:");
    Serial.println(mem);
    
    for(int a=0;a<10;a++){
      setSimon(simon[current]);
      strip.show();
      delay(50);
      clearPixels();
      strip.show();
      delay(50);
    }
    mem=0;
    win=1;
    delay(1000);
  }
}

void setSimon(int secz){
  for(int a=0;a<4;a++){
    strip.setPixelColor((secz*4)+a,clrs[secz][0],clrs[secz][1],clrs[secz][2]);
  }
}
void clearPixels(){
  for(int a=0;a<4;a++){
    strip.setPixelColor((a*4)+0,clrs[a][0]/64,clrs[a][1]/64,clrs[a][2]/64);
    strip.setPixelColor((a*4)+1,clrs[a][0]/64,clrs[a][1]/64,clrs[a][2]/64);
    strip.setPixelColor((a*4)+2,clrs[a][0]/64,clrs[a][1]/64,clrs[a][2]/64);
    strip.setPixelColor((a*4)+3,clrs[a][0]/64,clrs[a][1]/64,clrs[a][2]/64);
  }
}
