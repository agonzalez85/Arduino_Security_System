#ifndef SENSOR_H
#define SENSOR_H

#include "pin.h"
#include "oled.h"
#include "keys.h"

/**
  * This file contains all functions related to the sensor distance
*/

/**
  * This function prints the current sensor distance to OLED
*/
void drawCurrentDistance()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, String(detectDistance).c_str());
  } while (u8g.nextPage());
}

/**
  * This function prints menu option for user on OLED
*/
void drawNewDistance()
{
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(0,15,"Enter New Distance:");
  u8g.drawStr(0,30,"Use Keypad to input:");
  u8g.drawStr(0,45,"and press * to confirm");
}

/**
  *This function sets the sensor distance for the security system
  * @return sensor distance
*/
int setSensorDistance()
{
  int distance=0;
  String distanceStr="";
  
  while (true) 
  {
    String input = readKeyPad();
    if (input == "#") 
    {
      return distanceStr.toInt();
    } 
    else 
    {
      distanceStr += input;
      distance = distanceStr.toInt();

      // Update the display to show the user's input
      u8g.firstPage();
      do 
      {
        drawNewDistance();
        u8g.drawStr(0, 60, distanceStr.c_str());
      } while (u8g.nextPage());
    }
  }
}

#endif