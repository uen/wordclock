#define FASTLED_ESP8266_DMA
//#define FASTLED_RGBW
#include <FastLED.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ezTime.h>
#include <math.h>
#include "words.c"
#include "wordUtils.c"
#include "birthdayUtils.c"
#define CHIPSET       WS2812B
#define DATA_PIN      5

#define NUM_LEDS      172
CRGB leds[NUM_LEDS];

boolean topButtonPressed = false; // brightness
boolean bottomButtonPressed = false; 
boolean buttonsPressed = false;

double brightness = 1.0;

int topButton = 5; // d1;
int bottomButton = 4; // d2
int birth = 0;

const char* sensor_name = "wordclock";
const char* ota_password = "";

boolean isOff = false;

void fillWord(const int[],int, CRGB color = CRGB(255,255,255));
void clearWords(); 

Timezone UK;
void setup(){
  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS);

  // :)
  fillWord(word_manolis, sizeof(word_manolis)/sizeof(int));
  FastLED.show();
  
  // setup rainbow timer for birthdays
  rainbowTicker = millis();

  pinMode(topButton, INPUT_PULLUP);
  pinMode(bottomButton, INPUT_PULLUP);

  WiFiManager wifiManager;
  wifiManager.autoConnect("wordclock");
  setup_ota();
  
  UK.setLocation("Europe/London");
  UK.setDefault();
  setInterval(60);
  setDebug(INFO);
  delay(500);
  clearWords();
}

uint8_t curHour = 2;
uint8_t curMinute = 0;

CRGB overrideColor = CRGB::Black;

void loop() {  
  clearWords();

  curHour = hourFormat12();
  curMinute = minute();

  setBirthdays(UK.month(), day());
  fillTime((curHour%24)+1, curMinute);


  /*
   * 
   * Buttons
   * 
   */
   
   bool justPressedTopButton = false;

   int topButtonVal = digitalRead(topButton);
   if(!topButtonPressed && topButtonVal==0){
    topButtonPressed = true;
    brightness = fmod((brightness + 0.1), 1.0)+0.1;
    justPressedTopButton = true;
   } else if(topButtonVal==1 && topButtonPressed){
    topButtonPressed = false;
   }

   int bottomButtonVal = digitalRead(bottomButton);
   if(!bottomButtonPressed && bottomButtonVal==0){
    if(topButtonVal == 0 && topButtonPressed && justPressedTopButton){
      birthdayOverride = !birthdayOverride;
    } else{
      isOff = !isOff; 
    }    

    bottomButtonPressed = true;

    Serial.printf("Button button pressed. %d:%d", hour, minute);
   } else if(bottomButtonVal==1 && bottomButtonPressed){
    bottomButtonPressed = false;
   }


   if(birthdayOverride){
    birthdayShow = (birthdayShow + 1) % 5;
    resetBirthdays();
    switch(birthdayShow){
      case 1:
      sophiaBirthday = true;
      break;
      case 2:
      patrickBirthday = true;
      break;
      case 3:
      leanneBirthday = true;
      break;
      case 4:
      overrideColor = CRGB::Black;
      setBirthdays(month(), day());
      break;
      default:
       setBirthdays(month(), day());
    }
   }

   rainbow();

   ArduinoOTA.handle();
   if(isOff)
    clearWords();
    
   FastLED.show();
   delay(50);

   events();
}

void rainbow(){
  if(leanneBirthday || patrickBirthday || sophiaBirthday){

    rainbowState = rainbowState % 768;
    if(millis()-rainbowTicker > 200){

      for(int i=0; i < (sizeof(word_happy) / sizeof(int)); i++){
        leds[word_happy[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
      }


      for(int i=0; i < (sizeof(word_birth) / sizeof(int)); i++){
        leds[word_birth[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
      }


      for(int i=0; i < (sizeof(word_day) / sizeof(int)); i++){
        leds[word_day[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
      }

      if(leanneBirthday){
        for(int i=0; i < (sizeof(word_leanne) / sizeof(int)); i++){
          leds[word_leanne[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
        }
      } else{
        clearPatrickBirthday();
        clearSophiaBirthday();
      }


      if(patrickBirthday){
        for(int i=0; i < (sizeof(word_patrick) / sizeof(int)); i++){
          leds[word_patrick[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
        }
      } else{
        clearLeanneBirthday();
        clearSophiaBirthday();
      }


      if(sophiaBirthday){
        for(int i=0; i < (sizeof(word_sophia) / sizeof(int)); i++){
          leds[word_sophia[i]] = Wheel(((i * 50 / 13) + rainbowState) & 255);
        }
      } else{
        clearPatrickBirthday();
        clearLeanneBirthday();
      }


      rainbowState++;
      rainbowTicker = millis();
    }
  }
  else{
    fillWord(word_happy, sizeof(word_happy)/sizeof(int), CRGB::Black);
    fillWord(word_birth, sizeof(word_birth)/sizeof(int), CRGB::Black);
    fillWord(word_day, sizeof(word_day)/sizeof(int), CRGB::Black);
    fillWord(word_leanne, sizeof(word_leanne)/sizeof(int), CRGB::Black);
    fillWord(word_patrick, sizeof(word_patrick)/sizeof(int), CRGB::Black);
    fillWord(word_sophia, sizeof(word_sophia)/sizeof(int), CRGB::Black);
  } 
}

bool colorEquals(CRGB one, CRGB two){
  return ((one.r == two.r) && (one.g == two.g) && (one.b == two.b));
}

void fillWord(const int word[], int len, CRGB color){
  if((!colorEquals(CRGB::Black, color)) && (!colorEquals(overrideColor, CRGB::Black))){
    color = overrideColor;
  }
  
  color.r *= brightness;
  color.g *= brightness;
  color.b *= brightness;

  for(int letter = 0; letter < len; letter++){
    leds[word[letter]] = color;
  }  
}

void fillTime(uint8_t hours, uint8_t minutes){
  fillWord(word_it, sizeof(word_it)/sizeof(int));
  fillWord(word_is, sizeof(word_is)/sizeof(int));

  if(minutes >= 5 && minutes < 10) {
    fillWord(word_five,sizeof(word_five)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
  } else if(minutes >= 10 && minutes < 15) { 
    fillWord(word_ten,sizeof(word_ten)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
  } else if(minutes >= 15 && minutes < 20) {
    fillWord(word_quarter,sizeof(word_quarter)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
  } else if(minutes >= 20 && minutes < 25) {
    fillWord(word_twenty,sizeof(word_twenty)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
  } else if(minutes >= 25 && minutes < 30) { 
    fillWord(word_twenty, sizeof(word_twenty)/sizeof(int)); 
    fillWord(word_five,sizeof(word_five)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
  } else if(minutes >= 30 && minutes < 35) { 
    fillWord(word_half,sizeof(word_half)/sizeof(int));
    fillWord(word_past,sizeof(word_past)/sizeof(int));
  } else if(minutes >= 35 && minutes < 40) {
    fillWord(word_twenty,sizeof(word_twenty)/sizeof(int));
    fillWord(word_five,sizeof(word_five)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_to,sizeof(word_to)/sizeof(int));
  } else if(minutes >= 40 && minutes < 45) {
    fillWord(word_twenty,sizeof(word_twenty)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_to,sizeof(word_to)/sizeof(int));
  } else if(minutes >= 45 && minutes < 50) {
    fillWord(word_quarter,sizeof(word_quarter)/sizeof(int));
    fillWord(word_to,sizeof(word_to)/sizeof(int));
  } else if(minutes >= 50 && minutes < 55) {
    fillWord(word_ten,sizeof(word_ten)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_to,sizeof(word_to)/sizeof(int));
  } else if(minutes >= 55 && minutes < 60) {
    fillWord(word_five,sizeof(word_five)/sizeof(int));
    fillWord(word_minutes, sizeof(word_minutes)/sizeof(int));
    fillWord(word_to,sizeof(word_to)/sizeof(int));
  }

  if(minutes >= 35) {
    hours++;
  }

  if(hours >= 12) {
    hours -= 12;
  }
  
  switch(hours) {
    case 0:
    fillWord(word_twelve,sizeof(word_twelve)/sizeof(int));
    break;
    case 1:
    fillWord(word_one,sizeof(word_one)/sizeof(int));
    break;
    case 2:
    fillWord(word_two,sizeof(word_two)/sizeof(int));
    break;
    case 3:
    fillWord(word_three,sizeof(word_three)/sizeof(int));
    break;
    case 4:
    fillWord(word_four,sizeof(word_four)/sizeof(int));
    break;
    case 5:
    fillWord(word_fiveh,sizeof(word_five)/sizeof(int));
    break;
    case 6:
    fillWord(word_six,sizeof(word_six)/sizeof(int));
    break;
    case 7:
    fillWord(word_seven,sizeof(word_seven)/sizeof(int));
    break;
    case 8:
    fillWord(word_eight, sizeof(word_eight)/sizeof(int));
    break;
    case 9:
    fillWord(word_nine,sizeof(word_nine)/sizeof(int));
    break;
    case 10:
    fillWord(word_tenh,sizeof(word_ten)/sizeof(int));
    break;
    case 11:
    fillWord(word_eleven,sizeof(word_eleven)/sizeof(int));
    break;
  }

  if(minutes < 5){
    fillWord(word_oclock, sizeof(word_oclock)/sizeof(int));
  }
}

void setup_ota() {
  ArduinoOTA.setHostname(sensor_name);
  ArduinoOTA.setPassword(ota_password);

  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA..");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA ended..");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}


CRGB Wheel(byte WheelPos) {

  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos*3, 255 - WheelPos * 3);
  }
}
