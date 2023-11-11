#ifndef KEYS_H
#define KEYS_H

#include <Keypad.h>
#include <Wire.h>
#include "oled.h"
#include "time.h"

/**
  * This file contains all functions needed for the Keypad
*/

//Variables
double buttonTimer = 0; //Timer to avoid multiple key presses at a time
String fullString = ""; //concatenated string
String pressedCharacter = ""; //recently pressed character on the keypad
bool stringAdded = false; //keeps track of the concatenation
bool numberAdded=false;

/**
  *This function reads the input fron the Keypad and returns which key was selected
  * @return key entered from keypad
*/
String readKeyPad()
{
  if (buttonPressed == true)
  {
    if (millis() - buttonTimer > 300)
    {
      //The button pressed is only set back to false after 300 ms, so we cannot press a button twice quickly
      buttonTimer = millis();
      buttonPressed = false;
    }
    else
    {
      //do nothing
      return "";
    }
  }
  else
  {
    //B11101111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11101111);
    //[P7]B11101111[P0] -> [P7]1110[P4] - activates first row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11101110 - P0 pin went low after pressing the button -> 1 was pressed
      case 238: //Button 1
        Serial.print("1");
        pressedCharacter = "1";
        buttonPressed = true;
        numberAdded = true;
        break;

      //11101101 - P1 pin went low after pressing the button -> 2 was pressed
      case 237: //Button 4
        Serial.print("4");
        pressedCharacter = "4";
        buttonPressed = true;
        numberAdded = true;
        break;

      //11101011 - P2 pin went low after pressing the button -> 7 was pressed
      case 235: //Button 7
        Serial.print("7");
        pressedCharacter = "7";
        buttonPressed = true;
        numberAdded = true;
        break;

      //11100111 - P3 pin went low after pressing the button -> * was pressed
      case 231: //Button *
        Serial.println("*");
        pressedCharacter = "*";
        buttonPressed = true;
        stringAdded = true;
        break;
    }
    //-------------------------------------------
    //B11011111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11011111);
    //[P7]B11011111[P0] -> [P7]1101[P4] - activates second row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11011110 - P0 pin went low after pressing the button -> 2 was pressed
      case 222: //Button 2
        Serial.print("2");
        pressedCharacter = "2";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 221: //Button 5
        Serial.print("5");
        pressedCharacter = "5";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 219: //Button 8
        Serial.print("8");
        pressedCharacter = "8";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 215: //Button 0
        Serial.println("0");
        pressedCharacter = "0";
        buttonPressed = true;
        numberAdded = true;
        break;
    }
    //-------------------------------------------
    //B10111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B10111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 190: //Button 3
        Serial.print("3");
        pressedCharacter = "3";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 189: //Button 6
        Serial.print("6");
        pressedCharacter = "6";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 187: //Button 9
        Serial.print("9");
        pressedCharacter = "9";
        buttonPressed = true;
        numberAdded = true;
        break;

      case 183: //Button #
        Serial.println("#");
        pressedCharacter = "#";
        buttonPressed = true;
        stringAdded = true;
        break;
    }
    //-------------------------------------------
    //B01111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B01111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 126: //Button A
        Serial.print("A");
        pressedCharacter = "A";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 125: //Button B
        Serial.print("B");
        pressedCharacter = "B";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 123: //Button C
        Serial.print("C");
        pressedCharacter = "C";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 119: //Button D
        Serial.println("D");
        pressedCharacter = "D";
        buttonPressed = true;
        stringAdded = true;
        break;
    }
    buttonTimer = millis();
  }

  String returnValue=pressedCharacter;
  pressedCharacter="";
  return returnValue;
}

/** 
  *This function prints Enter Passcode: to OLED, then returns passcode entered from user input
  * @return passcode
*/
int password()
{
  String number;
  String newCode = "";
  int passcode = 0;

  while (newCode.length() < 4)
  {
    analogWrite(greenPin,0);
    analogWrite(redPin,255);
    alarmTone();
    blinking();
    number = readKeyPad();
    newCode += number;
    
    u8g.firstPage();
    do
    {
      drawClock();
      u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Enter Passcode:");
      u8g.drawStr(u8g.getWidth() / 2 - 12, u8g.getHeight() / 2 + 10, newCode.c_str());
    } while (u8g.nextPage());
    
  }

  passcode = newCode.toInt();

  u8g.firstPage();
  do
  {
    drawClock();
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Passcode Set!");
  } while (u8g.nextPage());

  delay(2000); // Display "Passcode Set!" for 2 seconds

  return passcode;
}

/**
  *This function changes the current passcode set for the security system
  *@return new passcode
*/
int changePassword()
{
  String number;
  String newCode = "";
  int password = 0;

  while (newCode.length() < 4)
  {
    number = readKeyPad();
    newCode += number;
    
    u8g.firstPage();
    do
    {
      drawClock();
      u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Enter Passcode:");
      u8g.drawStr(u8g.getWidth() / 2 - 12, u8g.getHeight() / 2 + 10, newCode.c_str());
    } while (u8g.nextPage());
    
  }

  password = newCode.toInt();

  u8g.firstPage();
  do
  {
    drawClock();
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Passcode Set!");
  } while (u8g.nextPage());

  delay(2000); // Display "Passcode Set!" for 2 seconds

  return password;
}

#endif
