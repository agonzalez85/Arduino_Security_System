#ifndef PIN_H
#define PIN_H

#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;

/**
  * This file defines all of the pin values for each component
*/

//rgb variables
const int redPin=5;
const int greenPin=7;

//buzzer variables
const int buzzer=2;
const int alarmFreq=500;
const int alarmDur=500;
const int confirmFreq=900;
const int confirmDur=100;
const int confirmPause=200;

//sonic sensor variables
const int triggerPin=3;
const int echoPin=4;
double travelTime;
int trigDelay=10;
double distanceTravelled; //inches
double targetDistance;
const double rate=0.013464; //in/us
int dt=500;
int detectDistance=0; //inches

//keypad
int securityCode=0;  //default security code

#endif
