boolean leanneBirthday = false;
boolean patrickBirthday = false;
boolean sophiaBirthday = false;
boolean  birthdayOverride = false;

int birthdayShow = 0;
unsigned long rainbowTicker;
uint16_t rainbowState = 0;

void resetBirthdays(){
  leanneBirthday = false;
  patrickBirthday = false;
  sophiaBirthday = false;
}


void setBirthdays(uint8_t curMonth, uint8_t curDay){
  Serial.println("Current day / month");
  Serial.println(curMonth);
  Serial.println(curDay);
  if(curMonth == APRIL && curDay == 18){
    sophiaBirthday = true;
  } else if(curMonth == AUGUST && curDay == 16){
    leanneBirthday = true;
  } else if(curMonth == SEPTEMBER && curDay == 2){
    patrickBirthday = true;
  } else if(curMonth == OCTOBER && curDay == 31){
    overrideColor = CRGB::Orange;
  } else if(curMonth == DECEMBER && curDay == 25){
    overrideColor = CRGB::Red;
  } else if(curMonth == MARCH && curDay == 17){
    overrideColor = CRGB::Green;
  } else if(curMonth == FEBRUARI && curDay == 14){
    overrideColor = CRGB::Pink;
  } else{
    overrideColor = CRGB::Black;
  }
}
