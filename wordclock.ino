#define FASTLED_ESP8266_DMA
//#define FASTLED_RGBW
#include <FastLED.h>

#include "words.c"

#define CHIPSET       WS2812B
#define DATA_PIN      5

#define NUM_LEDS      172
CRGB leds[NUM_LEDS];

const int AMOUNT_WORDS = 3;
const int* words[AMOUNT_WORDS] = {word_it, word_is, word_ten};

int DELETE_WORD = 0;


boolean leanneBirthday = false;
boolean patrickBirthday = false;
boolean sophiaBirthday = false;

unsigned long rainbowTicker;
uint16_t rainbowState = 0;


void fillWord(const int[],int, CRGB color = CRGB(255,255,255));

void setup() {
  Serial.begin(115200);
  Serial.println("Initialized");

  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS);

  fillWord(word_oclock, sizeof(word_oclock)/sizeof(int));

  fillWord(word_twelve, sizeof(word_twelve)/sizeof(int));

  leanneBirthday = true;
  rainbowTicker = millis();




}

void clearWords(){
  fillWord(word_it, sizeof(word_it)/sizeof(int), CRGB::Black);
  fillWord(word_is, sizeof(word_is)/sizeof(int), CRGB::Black);
  fillWord(word_ten, sizeof(word_ten)/sizeof(int), CRGB::Black);
  fillWord(word_half, sizeof(word_half)/sizeof(int), CRGB::Black);

  fillWord(word_quarter, sizeof(word_quarter)/sizeof(int), CRGB::Black);
  fillWord(word_twenty, sizeof(word_twenty)/sizeof(int), CRGB::Black);

  fillWord(word_five, sizeof(word_five)/sizeof(int), CRGB::Black);
  fillWord(word_minutes, sizeof(word_minutes)/sizeof(int), CRGB::Black);
  fillWord(word_past, sizeof(word_past)/sizeof(int), CRGB::Black);

  fillWord(word_to, sizeof(word_to)/sizeof(int), CRGB::Black);
  fillWord(word_one, sizeof(word_one)/sizeof(int), CRGB::Black);

  fillWord(word_three, sizeof(word_three)/sizeof(int), CRGB::Black);
  fillWord(word_eleven, sizeof(word_eleven)/sizeof(int), CRGB::Black);

  fillWord(word_two, sizeof(word_two)/sizeof(int), CRGB::Black);

  fillWord(word_four, sizeof(word_four)/sizeof(int), CRGB::Black);
  fillWord(word_nine, sizeof(word_nine)/sizeof(int), CRGB::Black);
  fillWord(word_seven, sizeof(word_seven)/sizeof(int), CRGB::Black);

  fillWord(word_fiveh, sizeof(word_fiveh)/sizeof(int), CRGB::Black);
  fillWord(word_six, sizeof(word_six)/sizeof(int), CRGB::Black);


  fillWord(word_eight, sizeof(word_eight)/sizeof(int), CRGB::Black);
  fillWord(word_tenh, sizeof(word_tenh)/sizeof(int), CRGB::Black);


  fillWord(word_twelve, sizeof(word_twelve)/sizeof(int), CRGB::Black);
  fillWord(word_oclock, sizeof(word_oclock)/sizeof(int), CRGB::Black);
}

uint8_t hour = 0;
uint8_t minute = 0;
void loop() {
  clearWords();
  //fillWord(words[DELETE_WORD % AMOUNT_WORDS], sizeof(words[DELETE_WORD % AMOUNT_WORDS])/sizeof(int));

  DELETE_WORD++;

  minute+=4;

  if(minute>60){
    minute = 0;
    hour++;
  }

  fillTime(hour%24, minute);
  rainbow();
  FastLED.show();

  delay(1000);
  

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
      }

      
      rainbowState++;
      rainbowTicker = millis();
    }
  }
  else{

    fillWord(word_happy, sizeof(word_happy)/sizeof(int));
    fillWord(word_birth, sizeof(word_birth)/sizeof(int));
    fillWord(word_day, sizeof(word_day)/sizeof(int));
    fillWord(word_leanne, sizeof(word_leanne)/sizeof(int));
    fillWord(word_patrick, sizeof(word_patrick)/sizeof(int));
    fillWord(word_sophia, sizeof(word_sophia)/sizeof(int));
  }
  FastLED.show();   
}


void fillWord(const int word[], int len, CRGB color){

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



