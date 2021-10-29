#include <Adafruit_NeoPixel.h>
#define LED_PIN    14
#define LED_COUNT 42
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

bool dice[8][7]={{0,0,0,1,0,0,0},
                 {1,0,0,0,0,0,1},
                 {0,0,1,1,1,0,0},
                 {1,1,0,0,0,1,1},
                 {1,1,0,1,0,1,1},
                 {1,1,1,0,1,1,1},
                 {1,1,1,1,1,1,1},
                 {0,0,0,0,0,0,0}};

int pinputs[7]={7,6,5,4,3,2,8};

void setup() {
  strip.begin();
  Serial.begin(115200);
  strip.begin();
  for(int a=0;a<5;a++){
    for(int b=0;b<8;b++){
      setDice(a,b,random(65536));
      strip.show();
      delay(40);
    }
  }
  strip.show();
  for(int a=0;a<7;a++){
    pinMode(pinputs[a],INPUT_PULLUP);
  }
}

long unsigned timer;
long unsigned latchTimer[7];
long unsigned rollTimer;
long unsigned pulseTimer;
byte fade;
long unsigned fadeval;
bool latch[7];
bool hold[5];
int  rollVal[5];
int rollCount;
bool roll;
int reroll;

void loop() {
  timer=millis();
  if(timer>pulseTimer){
    pulseTimer+10;
    fade+=1;
    if(fade<128){
      fadeval=255-fade;
    } else {
      fadeval=fade;
    }
    fadeval=fadeval*fadeval;
    fadeval/=255;
    for(int a=0;a<5;a++){
      if(hold[a]){setDice(a,rollVal[a],0);}
    }
    strip.show();
  }
  for(int a=0;a<7;a++){
    if(!digitalRead(pinputs[a])&&!latch[a]&&timer>latchTimer[a]){
      latch[a]=1;
      latchTimer[a]=timer+20;
      if(a<5){
        hold[a]=!hold[a];
        if(hold[a]){setDice(a,rollVal[a],0);}
        else{       setDice(a,rollVal[a],random(65535)+1);}
        strip.show();
      }
      if(a==5){
        roll=1;
      }
      if(a==6){
        clearScreen();
        rollCount=0;
        for(int b=0;b<5;b++){
          hold[b]=0;
          rollVal[b]=7;
        }
      }
    }
    if(digitalRead(pinputs[a])&&latch[a]&&timer>latchTimer[a]){
      latch[a]=0;
      latchTimer[a]=timer+20;
      if(a==5){
        roll=0;
        reroll=6;
        rollCount++;
        for(int b=0;b<min(3,rollCount);b++){
          if(rollCount<=3){
            strip.setPixelColor(39+b,0,255,255);
          } else {
            strip.setPixelColor(39+b,255,0,0);
          }
          strip.show();
        }
      }
    }
  }
  if(roll||reroll){
    if(timer>rollTimer){
      rollTimer=timer+50+(reroll*10);
      reroll--;
      if(reroll<0){reroll=0;}
      for(int a=0;a<5;a++){
        if(!hold[a]){
          rollVal[a]=random(6);
          setDice(a,rollVal[a],random(65535)+1);
        }
      }
      strip.show();
    }
  }
}


void setDice(int dicez,int rollz,word colorz){
  for(int a=0;a<7;a++){
    int poz;
    if(a<2){
      poz=(dicez*2)+a;
    }
    if(a>=2&&a<5){
      poz=28-((dicez*4)+(a-2));
    }
    if(a>=5){
      poz=29+((a-5)+(dicez*2));
    }
    if(dice[rollz][a]){
      if(!colorz){strip.setPixelColor(poz,fadeval,fadeval,fadeval);}
      else{       strip.setPixelColor(poz,strip.ColorHSV(colorz));}
    } else {
      strip.setPixelColor(poz,0,0,0);
    }
  }
}
void clearScreen(){
  for(int a=0;a<LED_COUNT;a++){
    strip.setPixelColor(a,0,0,0);
  }
}
