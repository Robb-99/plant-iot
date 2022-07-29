#include <FastLED.h>

#define IMDVerse

#define PIN_SOUND 12
int freq[4] = {440, 580, 650, 720};

#define LED_PIN     7
#define NUM_LEDS    80
CRGB leds[NUM_LEDS];

const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
int ledPercentValue = 0;

const int soilUrgent = 10;
const int soilDry = 20;
const int soilTooWet = 70;

/*
 * Reprents led indices to draw the pil pattern
 */
const int pilLEDPattern[28] = 
{
  6,
  9,
  16,
  17,
  18,
  19,
  20,
  21,
  22,
  32,
  33,
  34,
  35,
  36,
  37,
  38,
  50,
  60,
  62,
  64,
  68,
  73,
  74,
  75,
  76,
  77,
  78,
  79
};

/*
 * Draws the PIL led pattern onto the matrix
 */
void drawPIL()
{
   for(int i = 0; i < sizeof(pilLEDPattern) / sizeof(pilLEDPattern[0]); i++) leds[pilLEDPattern[i]] = CRGB(0, 255, 0);
}


void alertSound(){
   playTone(freq[0], 150, 30);
   playTone(freq[1], 150, 30);
   playTone(freq[3], 150, 30);
   playTone(freq[1], 150, 30);
   playTone(freq[0], 150, 30);
   playTone(freq[1], 150, 30);
   playTone(freq[3], 300, 30);
   playTone(freq[1], 300, 30);
   playTone(freq[1], 300, 30);
   delay(3000);
}

void playTone(int toneFrequency, int delayTime, int pauseTime)
{
    tone(PIN_SOUND, toneFrequency);
    delay(delayTime);
    noTone(PIN_SOUND);
    delay(pauseTime);
}

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  ledPercentValue = 100 / NUM_LEDS;
  //Serial.println(ledPercentValue);
}

void loop() {
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  //Serial.print(soilmoisturepercent);
  //Serial.println("%");
  FastLED.clear(true);
  int ledsPercentActive = 0;
  //Zustände: Rot soilThreeshold <35, Grün  35-52, Blau >52
  if (soilmoisturepercent <= soilDry){
    for (int i = 0; i <= NUM_LEDS; i++) {
      ledsPercentActive += ledPercentValue;
      leds[i] = CRGB(255,  0,   0); 
      if(ledsPercentActive > soilmoisturepercent){
        break;
      }
    }
  }
  #ifdef IMDVerse
  else if (soilmoisturepercent > soilDry && soilmoisturepercent <= soilTooWet) drawPIL();
  
  #else
  else if (soilmoisturepercent > soilDry && soilmoisturepercent <= soilTooWet){
    for (int i = 0; i <= NUM_LEDS; i++) {
      ledsPercentActive += ledPercentValue;
      leds[i] = CRGB(0,  255,   0); 
      if(ledsPercentActive > soilmoisturepercent){
        break;
      }
    }
  }
  #endif
  else if (soilmoisturepercent > soilTooWet){
    for (int i = 0; i <= NUM_LEDS; i++) {
      ledsPercentActive += ledPercentValue;
      leds[i] = CRGB(0,  0,   255); 
      if(ledsPercentActive > soilmoisturepercent){
        break;
      }
    }
  }
  //SSerial.println();
  FastLED.show();
  if(soilmoisturepercent < soilUrgent){
 //Serial.println("Sound");
    alertSound();
  }
  delay(20000);
}
