#include <SPI.h> //rfid library
#include <RFID.h> //rfid library
#include "U8glib.h" //oled library
#include "tone.h"
#include "pin.h"
#include "oled.h"
#include "sensor.h"
#include "keys.h"
//#include "time.h"
#include <Wire.h>
#include <DS3231.h>

/**
  * Security System Program with Arduino Nano
  *
  * The purpose of this program is to implement a security system using the Arduino Nano and various components including:
  * Keypad for user input, OLED display for user interface, Joystick for menu navigation, Ultrasonic sensor for detection,
  * RFID reader for alarm deactivation, Buzzer and RGB LED for alamrm sound and status indication
  *
  * The main features of the system are as follows:
  * User can set a personal passcode to deactivate the alarm
  * User can change the senor's detection distance
  * User can activate the alarm
  * Alarm can be deactivated using a valid RFID tag or with a valid passcode
  * The alarm is triggered when an intruder enters the sensor distance, indicated by a red LED and alarm sound
  * 
  * @author Andy Gonzalez
  * @date July 23, 2023
*/

//Variables to control system state
bool alarmSet=false;
bool tagScanned=false;
bool correctCode=false;


void setup() 
{
  //Initializing Variables
  //RFID
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();

  //RGB
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);

  //Buzzer
  pinMode(buzzer,OUTPUT);

  //Ultrasonic sensor
  pinMode(triggerPin,OUTPUT);
  pinMode(echoPin,INPUT);

  //OLED display
  prepare();
  drawMenu(1); //Initially select the first menu option
  pinMode(buttonPin, INPUT_PULLUP); //Enable the internal pull-up resistor for the button pin

  //Keypad
  Wire.begin(); //initalize i2c communication
  Wire.beginTransmission(0x20); //addres of PCF8574T expander module
  Wire.write(0xFF); //sets all pins high (pull-up resistor enabled)
  Wire.endTransmission();

  //Clock
  Serial.println("Initialize RTC Module");
  clock.begin(); //initialize clock
  clock.setDateTime(__DATE__, __TIME__);  //Manuel (YYYY, MM, DD, HH, II, SS)

}

void loop() 
{
  //gets menu option from joystick
  int selectedOption=getSelectedOption();
  buttonPressed = digitalRead(buttonPin) == LOW;

  // Update the menu display
  drawMenu(selectedOption);

  // Check if the button is pressed and perform the corresponding action
  if (buttonPressed) 
  {
    buttonPressed=false;
    switch (selectedOption) 
    {
      case 1: //Sets Alarm
      {
        if(securityCode==0) //default code set to 2580
        {
          securityCode=2580;
        }
        if(detectDistance==0) //default distance set to 10 inches
        {
          detectDistance=10;
        }

        //Set Alarm Function
        alarmSet=true;
        bool tagScanned=false;
        bool correctCode=false;

        //Shows User Alarm is set
        clear();
        drawAlarmSet();
        confirmationTone();

        while(alarmSet)
        {
          //Sends Ultrasonic pulse
          digitalWrite(triggerPin,LOW);
          delayMicroseconds(trigDelay);
          digitalWrite(triggerPin,HIGH);
          delayMicroseconds(trigDelay);
          digitalWrite(triggerPin,LOW);

          //Calculates distance in inches 
          travelTime=pulseIn(echoPin,HIGH);
          distanceTravelled=travelTime*rate;
          targetDistance=distanceTravelled/2;
          delay(25);

          Serial.println(targetDistance); //prints distances
 
          //Checks if object is within sensor radius
          if((targetDistance<detectDistance) && (alarmSet=true))
          {
            Serial.println(detectDistance);

            clear();

            while(!tagScanned && !correctCode && alarmSet) //if the tag isn't scanned or incorrect code is entered, flash red
            { 
              rfidCard=""; 

              //Alarm Activates
              analogWrite(greenPin,0);
              analogWrite(redPin,255);
              Serial.println("Intrudor Alert!");
              alarmTone();
              blinking();

              drawAlarmPrompt();

              //Asks User if they would like to deactivate alarm with passcode
              int enteredCode=-1;
              buttonPressed = digitalRead(buttonPin) == LOW; 
              if(buttonPressed)
              {
                enteredCode=password();
              }
              if(enteredCode==securityCode)
              {
                correctCode==true;
                confirmationTone();
                Serial.println("Valid");
                alarmSet=false;
                break;
                break;    
              } 
              else if (rfid.isCard()) //reads rfid card
              {
                if (rfid.readCardSerial()) 
                {
                  rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
                  Serial.println(rfidCard);
                  rfid.halt();
                }
              }

              if(rfidCard=="243 116 8 158" || correctCode==true) //blue tag, accepted card flashes green Led with confirmation noise
              {
                rfidCard=""; 
                Serial.println("Accepted!");
                Serial.println(rfidCard);
                confirmationTone();
                tagScanned=true; 
                alarmSet=false;
                break;
                break;
              }
              else if(rfidCard=="67 214 14 17" || correctCode==false) //white card, incorrect card continues flashing red with alarm noise
              {
                Serial.println("Invalid!");
              }        
            }
          }
        }
        break;
      }

      case 2: //Change Passcode function

        securityCode=changePassword();
        break;
        
      case 3: //Set Sensor Distance function
        drawDistancePrompt(); // Displays "Enter Distance" on OLED

        String distanceString = ""; // Stores distance thats entered
        bool distanceEntered = false; // Checks if distance has been entered
        while (!distanceEntered) 
        {
          String input = readKeyPad(); //stores key from keypad
          if (input == "#") //stores passcode before # was entered
          {
            int distance = distanceString.toInt();
            Serial.println("Distance entered: " + distance);
            detectDistance=distance;
            clear();
            drawDistanceConfirmation(detectDistance); // Display's distance
            delay(3000); // Show the entered distance for 3 seconds
            distanceEntered = true; // Breaks out the while loop
            distanceString="";
          } 
          else 
          {
            distanceString += input; 
          }
        }
        break;
    }
    delay(200); // Delay to debounce the button
  }
}
