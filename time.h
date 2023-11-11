#ifndef TIME_H
#define TIME_H

#include <Wire.h>
#include <DS3231.h>

//Clock object
DS3231 clock;
RTCDateTime rtcDateTime;

/**
  * This function gets the date and time from the Real-Time Clock Module
  * @return date/time string
*/
String getTimeString() 
{
  String timeStr = String(rtcDateTime.month) + "/" + String(rtcDateTime.day) + "/" + String(rtcDateTime.year) + "  " +
                   String(rtcDateTime.hour) + ":" + String(rtcDateTime.minute) + ":" + String(rtcDateTime.second);
  return timeStr;
}

#endif