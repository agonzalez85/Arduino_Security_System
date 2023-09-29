#ifndef TONE_H
#define TONE_H
#include "pin.h"

/**
  * This file defines all functions for the buzzer
*/

/**
  * This function produces an alarm-like sound
*/
void alarmTone()
{
  tone(buzzer,alarmFreq,alarmDur);
  delay(dt);
  noTone(buzzer);
}

/**
  * This function provides a confirmation sound, a high pitch beep beep
*/
void confirmationTone()
{
  for(int i=0; i<2;i++)
  {
    digitalWrite(greenPin,HIGH);
    digitalWrite(redPin,LOW);
    tone(buzzer,confirmFreq,confirmDur);
    delay(confirmPause);
    noTone(buzzer); 
    digitalWrite(greenPin,LOW);
  }
}

/**
  * This function continuously blinks the red LED
*/
void blinking()
{
  digitalWrite(redPin,HIGH);
  delay(75);
  digitalWrite(redPin,LOW);
  delay(75);
}

#endif