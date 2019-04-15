/*
 Name:    SensorLib.h
 Created: 2/11/2018 2:30:37 PM
 Author:  Farbod
 Editor:  http://www.visualmicro.com
*/

#ifndef _SensorLib_h
#define _SensorLib_h

//---------------------------------------------
#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif
//---------------------------------------------


class Sensor {

public:
  Sensor(int pin, int type);
  float dataCollect();

private:
  int _pin;
  int _type;
  float _rawRead;
  float _adjRead;
};


#endif


