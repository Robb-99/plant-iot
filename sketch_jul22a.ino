#include <FastLED.h>

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

const int soilThreeshold = 25;


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
  Serial.println(ledPercentValue);
}

void loop() {
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  FastLED.clear(true);
  int ledsPercentActive = 0;
  for (int i = 0; i <= NUM_LEDS; i++) {
    ledsPercentActive += ledPercentValue;
    leds[i] = CRGB(soilmoisturepercent >= soilThreeshold ? 0 : 255, soilmoisturepercent >= soilThreeshold ? 255 : 0, 0);
    if(ledsPercentActive > soilmoisturepercent){
      break;
    }
  }
  Serial.println();
  FastLED.show();
  if(soilmoisturepercent < soilThreeshold){
    Serial.println("Sound");
    alertSound();
  }
  delay(250);
}
