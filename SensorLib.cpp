/*
 Name:    SensorLib.cpp
 Created: 2/11/2018 2:30:37 PM
 Author:  Farbod
 Editor:  http://www.visualmicro.com
*/

#include "SensorLib.h"

Sensor::Sensor(int pin, int type)
{
  _pin = pin;
  _type = type; //type 1 - light, type 2  - temp , type 3 - wind
  pinMode(_pin, INPUT);
  
}

float Sensor::dataCollect() {

  if    (_type == 1) {

    _rawRead = analogRead(_pin);
    _adjRead = ((_rawRead * 5.0) / 1024.0) * 2000.0; //lux reading between 0 and  1000 based on a 10kOhm resistor
  }
  else if (_type == 2) {
    _rawRead = analogRead(_pin);
    _adjRead = ((((_rawRead * 5.0) / 1024.0) - 0.5) * 100) + 9;
    //celcius temp function
  }
  else if (_type == 3) {
    _rawRead = analogRead(_pin);
    _adjRead = _rawRead;
  }

  return _adjRead;
}

