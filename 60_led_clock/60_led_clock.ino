// LED clock:
// 60xWS2812b
// RTS
// ESP8266 for server sync (or, later, reduce to only ESP8266)
// USB power (battery pack possible)

// features:
// * display clock
// * display rainbow
// * light up full
// * thermometer
// * (photoresistor) adjust brightness. Calibrate using a light level comparator against the LED itself.
// * keypad on back? And/or not buttons: instead, toch-sensitive pads or
// * compass? https://brainy-bits.com/tutorials/find-your-way-using-the-hmc5883l/
// * visual: smooth fade seconds. Additive color values.Pulsing clock face every second.
// * alarm clock
// put on github, publish

// Hardware:
// infinity mirror (buy: 35x35cm, 1 mirror, 1 glass)
// LEDS in a circle (Poxilina?
// if time, add diffusers (eppendorfs not good. Polynorph? Opaque white tape?
// hinges holding bottom part: hanging below, or serving as stand
// don't forget resistor and capacitor

// links: WS2812b
// https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library
// http://www.instructables.com/id/Bitbanging-step-by-step-Arduino-control-of-WS2811-/
// links: DS1307 RTC:
// http://www.instructables.com/id/Arduino-Real-Time-Clock-DS1307/?ALLSTEPS
// http://www.instructables.com/id/Arduino-Watch-Build-Instructions/step10/Soldering-the-Real-Time-Clock/
// links: ESP8266 with Arduino:
// https://petestechprojects.wordpress.com/2014/12/10/mini-server-on-arduino-esp8266/
// https://petestechprojects.wordpress.com/2014/12/08/leds-control-through-tty/
// https://petestechprojects.wordpress.com/2014/11/30/esp8266-4-wifi-module-bring-up/
// ... and timetnp example in time library
// links: keypad
// http://www.learningaboutelectronics.com/Articles/Arduino-keypad-circuit.php
// links: other:
// https://learn.adafruit.com/flora-geo-watch




// LEDs
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// timekeeping
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

// Arduino pin number (most are valid)
#define NEOPIXEL_PIN 9
#define LEDCOUNT 60

#define SHIFT_BLUE 0
#define SHIFT_GREEN 8
#define SHIFT_RED 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long timeLast, timeNow;
byte lastSecond = -1, s, m, h;
int ticksInLastSecond = 0; // benchmark

// Colors. TODO: read in terminal, change to defines
uint32_t faceOffColor, faceHourColor, faceQuarterColor, face12Color, handHColor, handMColor, handSColor;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.





void setup() {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  strip.begin();
  strip.setBrightness(64); // 1-255

  faceOffColor = strip.Color(0, 0, 0);
  faceQuarterColor = strip.Color(0x10, 0x10, 0x10);
  faceHourColor = strip.Color(0x08, 0x08, 0x08);
  face12Color = strip.Color(0x40, 0x40, 0x40);
  handHColor = strip.Color(0xFF, 0, 0);
  handMColor = strip.Color(0, 0xFF, 0);
  handSColor = strip.Color(0, 0, 0x90);

  //strip.show(); // Initialize all pixels to 'off'

  
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  while(timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
    rainbowCycle(10, 1);
  }
  digitalClockDisplay();
  timeLast = millis();
  rainbowCycle(2, 1); // kinda a bootup anim
}

void loop() {
  //rainbowCycle(5);
  
  tick();
}

void paintClockFace() {
  uint8_t strength = 0xFF - constrain((timeNow-timeLast) >> 1, 0, 0xFF);
  //Serial.print("\t");
  //Serial.println(strength);
  strip.setPixelColor(0, getFadedColor(face12Color, strength));
  for (int i = 1; i < LEDCOUNT; i++) {
    strip.setPixelColor(i, (i%5 == 0) ? // hour marks
          getFadedColor( ((i%15 == 0) ? faceQuarterColor : faceHourColor), strength) :
          faceOffColor);
  }
}

void paintClockhands() {
  stackColor(s, handSColor, 0xFF, 0xFF);
  stackColor(m, handMColor, 0xFF, 0xFF);
  stackColor(5*h, handHColor, 0xFF, 0xFF);
}

void updateClockVars() {
  timeNow = millis();
  s = second();
  if(s != lastSecond){
    lastSecond = s;
    Serial.print("ticksInLastSecond: ");
    Serial.println(ticksInLastSecond);
    ticksInLastSecond = 0;
    timeLast = timeNow;
    m = minute();
    h = hour()%12;
  } else {
    ticksInLastSecond++;
  }
}

uint8_t getColorComponent(uint32_t packedColor, short shift, uint8_t strength) {
  return (((uint8_t)(packedColor >> shift)) * strength) >> 8; // note: full strength is /255 *256 (always less!)
}

void stackColor(uint16_t led, uint32_t newColor, uint8_t oldStrength, uint8_t newStrength){
  uint32_t oldColor = strip.getPixelColor(led);
  uint8_t
      oldR = getColorComponent(oldColor, SHIFT_RED, oldStrength),
      oldG = getColorComponent(oldColor, SHIFT_GREEN, oldStrength),
      oldB = getColorComponent(oldColor, SHIFT_BLUE, oldStrength),
      newR = getColorComponent(newColor, SHIFT_RED, newStrength),
      newG = getColorComponent(newColor, SHIFT_GREEN, newStrength),
      newB = getColorComponent(newColor, SHIFT_BLUE, newStrength);
  strip.setPixelColor(led,
      constrain(oldR+newR, 0, 0xFF),
      constrain(oldG+newG, 0, 0xFF),
      constrain(oldB+newB, 0, 0xFF));
}
uint32_t getFadedColor(uint32_t packedColor, uint8_t strength){
  
  //Serial.print(getColorComponent(packedColor, SHIFT_RED, strength));
  //Serial.println("\t");
  return strip.Color(
      getColorComponent(packedColor, SHIFT_RED, strength),
      getColorComponent(packedColor, SHIFT_GREEN, strength),
      getColorComponent(packedColor, SHIFT_BLUE, strength));
}


void tick() {
  // TODO: store time of last change of seconds,
  // oldStrength dependent on time since then (like, constrain(diff > 1, 0, 0xFF)
  updateClockVars();
  paintClockFace();
  paintClockhands();
  strip.show();
}










void rainbowCycle(uint8_t wait, int cycles) {
  uint16_t i, j;

  for (j = 0; j < 256 * cycles; j++) {
    for (i = 0; i < LEDCOUNT; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / LEDCOUNT) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}




void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
