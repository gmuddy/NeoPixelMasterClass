#include <Adafruit_NeoPixel.h>
#define LED_COUNT 16
#define LED_PIN 11
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


#include <QMC5883LCompass.h>
QMC5883LCompass compass;

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(115200);
  compass.init();
  Serial.println("Starting!!");
}


void loop() {
  int x;
  int y;
  float rads;
  compass.read();
  x=compass.getX();
  y=compass.getY();
  rads=atan2(x,y);
  rads+=PI;
  int angle=rads*57.325;
  Serial.println(angle);
  angle=359-angle;
  int pix=((LED_COUNT/360.00)*angle);
  setPixel(pix+LED_COUNT-2,170,255);
  //strip.setPixelColor(pix,255,0,0);
  strip.show();
  strip.setPixelColor((pix+LED_COUNT-2)%LED_COUNT,0,0,0);
  delay(100);
  
}

void setPixel(int pixelz, int colorz, long unsigned fadez){
  pixelz%=LED_COUNT;
  colorz%=1024;
  fadez*=fadez;
  fadez/=255;
  strip.setPixelColor(pixelz,strip.ColorHSV(colorz*64,255,fadez));
}
