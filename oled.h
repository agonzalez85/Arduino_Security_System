#ifndef OLED_H
#define OLED_H

#include "U8glib.h"

/**
 * This file contains all functions for the OLED display, as well as the variables for the joystick pins
*/

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

const int joyPinX = A0;      // Analog input pin for X-axis
const int joyPinY = A1;      // Analog input pin for Y-axis
const int buttonPin = 6;     // Digital input pin for the joystick button

int xPosition = 0;    // Current X-axis position
int yPosition = 0;    // Current Y-axis position
bool buttonPressed = false;  // Flag to indicate if the button is pressed

const int numOptions=3; //Number of Menu Options
int selectedOption=1; //Current Option

/**
  * This function prepares the OLED to print 
*/
void prepare() 
{
  u8g.begin();
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

/**
  *This function displays 'Hello World' to the OLED
*/
void drawHelloWorld()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Hello, World!");
  } while (u8g.nextPage());
}

/**
  * This function prints the menu options to the OLED
*/
void drawMenu(int selectedOption) 
{
  u8g.firstPage();
  do 
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 15, selectedOption == 1 ? "> Set Alarm" : "  Set Alarm");
    u8g.drawStr(0, 30, selectedOption == 2 ? "> Change Passcode" : "  Change Passcode");
    u8g.drawStr(0, 45, selectedOption == 3 ? "> Set Sensor Distance" : "  Set Sensor Distance");
  } while (u8g.nextPage());
}

/**
  * This function prints 'Alarm is Active' to the OLED
*/
void drawAlarmSet()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Alarm is Active!");
    delay(2000);
  } while (u8g.nextPage());
}

/**
  * This function displays 'Intruder Alert!' to the OLED
*/
void drawIntruder()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Intruder Alert!");
    delay(2000);
  } while (u8g.nextPage()); 
}

/**
  * This function prints 'Accepted!' to the OLED
*/
void drawAccepted()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Accepted!");
    delay(2000);
  } while (u8g.nextPage()); 
}

/**
  * This function displays 'Invalid!' to the OLED
*/
void drawInvalid()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Invalid!");
    delay(2000);
  } while (u8g.nextPage()); 
}

/**
  * This function displays 'Passcode Set!' to the OLED
*/
void drawPassword()
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(u8g.getWidth() / 2 - 36, u8g.getHeight() / 2 - 5, "Passcode Set!");
    delay(2000);
  } while (u8g.nextPage());
}

/**
  * This function clears the OLED to a blank screen
*/
void clear()
{
  u8g.firstPage();
  do
  {
    u8g.setDefaultBackgroundColor(); //set background to blank
    u8g.drawBox(0,0,u8g.getWidth(),u8g.getHeight()); //draw box to clear display
  } while(u8g.nextPage());
}

/**
  * This function reads the value from the joystick for the menu display
  * @return joystick position for menu option
*/
int getSelectedOption()
{
  // Read the joystick inputs
  xPosition = analogRead(joyPinX);
  yPosition = analogRead(joyPinY);

  // Determine the selected menu option based on joystick inputs
  if (yPosition < 100) 
  {
    selectedOption--;
    if(selectedOption==0)
    {
      selectedOption=3;
    }
  } 
  else if (yPosition > 900) 
  {
    selectedOption++;
    if(selectedOption>3)
    {
      selectedOption=1;
    }
  }

  return selectedOption;
}

/**
  * This function displays 'Enter Distance then #' to the OLED
*/
void drawDistancePrompt() 
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0,30, "Enter Distance then #");
  } while (u8g.nextPage());
}

/**
  * This function displays the entered distance from the user
*/
void drawDistanceConfirmation(int distance) 
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do 
  {
    u8g.setFont(u8g_font_6x10);
    u8g.setCursorPos(0,0);
    u8g.print(distance); // Print the entered distance
    u8g.drawStr(20,0, "inches");
  } while (u8g.nextPage());
}

/**
  * This function displays options to deactivate the alarm, 'Enter Passcode or Scan Tag'
*/
void drawAlarmPrompt() 
{
  u8g.setDefaultForegroundColor();
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0,15, "> Enter Passcode?");
    u8g.drawStr(0,30, "Or Scan Tag");
  } while (u8g.nextPage());
}


#endif
