/*
Name:    WeatherStation.ino
Created: 2/11/2018 2:33:43 PM
Author:  Farbod
*/

#include "SensorLib.h"
#include <SoftwareSerial.h>
#include <serLCD.h>
#include <Time.h>
#include <TimeLib.h>

//Declare Modules
Sensor lightSen(0, 1);
Sensor tempSen(1, 2);
Sensor windSen(2, 3);
serLCD lcd(50);

int lightLed = 41;
int tempLed = 42;
int windLed = 43;
int switch1 = 52;
int menuButton = 51;

int windLedState, tempLedState, lightLedState; //environmental state holders for leds

                         //Declare Variables
float data[3][3];


int ledBlink; //led on & off checker for blinking
int waitCounter; //counter for intial averaged data @  startup
int menuNum; //lcd menu state holder
int avgInterval;
int logInterval;
int testMode;
bool updateDisp;
int prevDispTime;
unsigned long currentTime, dispCurrentTime;
unsigned long prevLogTime, prevAvgTime, prevBlinkTime;
int currentButtonState, prevButtonState;


// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  prevLogTime = 0;
  prevAvgTime = 0;
  prevBlinkTime = 0;
  windLedState = 0;
  lightLedState = 0;
  tempLedState = 0;
  menuNum = 0;
  waitCounter = 0;
  updateDisp = true;
  
  logInterval = 1000;
  avgInterval = 6000;
  pinMode(switch1, INPUT);
  pinMode(menuButton, INPUT);
  pinMode(lightLed, OUTPUT);
  pinMode(windLed, OUTPUT);
  pinMode(tempLed, OUTPUT);
  startUp();
  for (int i = 0; i < 3; i++) {
    data[i][i] = 0;
    data[i][i] = 0;
    data[i][i] = 0;
  }

  setTime(2,51,0,21,2,2018); //Sets arduino to time (hour, minute, second, day, month, year)
}

// the loop function runs over and over again until power down or reset
void loop() {
  currentTime = millis();

  currentButtonState = digitalRead(menuButton);

 // if(digitalRead(switch1)){
//    logInterval = 10000;
//    avgInterval = 60000; 
//    testMode = 1; 
//  }

 // else {
 //   avgInterval = 600000;  
 //   testMode = 0;
 // }
  
  if (currentButtonState != prevButtonState) {
    if (currentButtonState == 0){
      menu();
      printToMonitor();
    }
  }


  if ((currentTime - prevLogTime) >= logInterval) {
    prevLogTime = currentTime;
    data[0][0] += lightSen.dataCollect();
    data[0][1] += tempSen.dataCollect();
    data[0][2] += windSen.dataCollect();
  }

  if ((currentTime - prevAvgTime) >= avgInterval) {
    prevAvgTime = currentTime;
    avgData();
  }

  if ((currentTime - prevDispTime) >= lcdDelay(5)) {
    updateDisp = true;
    prevDispTime = currentTime;
  }

  if ((currentTime - prevBlinkTime) >= 300) {
    prevBlinkTime = currentTime;
    if (ledBlink == 1)
      ledBlink = 0;
    else
      ledBlink = 1;
  }

  lcdDisp();
  ledAlarm();
 
  prevButtonState = currentButtonState;
}

//-------------------------------------------------------
void startUp() {
  lcd.clear();
  delay(10);
  lcd.print("Hello...");
  delay(1000);
}
//-------------------------------------------------------
void menu() {
  if (menuNum == 2)
    menuNum = 0;
  else
    menuNum++;
}
//-------------------------------------------------------
void lcdDisp() {
  if (updateDisp) {
    if (currentTime <= (avgInterval + 1000)) {
      pleaseWait();
    }
    else {
      switch (menuNum) {
      case 0:
        dispTemp();
        break;
      case 1:
        dispLight();
        break;
      case 2:
        dispWind();
        break;
           
      default:
        break;
      }
    }
    updateDisp = false;
  }
}
//------------------------------------------------------
void dispLight() {
  int lux = data[1][0];
  lcd.clear();
  lcd.print("Light Level:    ");
  lcd.print(lux);
  lcd.print(" Lux ");
  if (lux < 100) {
    lcd.print("Dark");
    lightLedState = 0;
  }
  else if (lux > 100 && lux <= 1000) {
    lcd.print("Overcast");
    lightLedState = 0;
  }
  else if (lux > 1000 && lux <= 10000) {
    lcd.print("Bright");
    lightLedState = 0;
  }
  else if (lux > 10000) {
    lcd.print("Sunny");
    lightLedState = 1;
  }
  delay(100);
}
//-------------------------------------------------------
void dispTemp() {
  int tempC = (int)data[1][1];
  lcd.clear();
  lcd.print("Temperature:    ");
  lcd.print(tempC);
  lcd.print(" C ");
  lcd.print((int)((tempC * 9.0 / 5.0) + 32.0));
  lcd.print(" F ");
  if (tempC <= 0) {
    lcd.print("Freezing");
    tempLedState = 0;
  }
  else if (tempC > 0 && tempC < 15) {
    lcd.print("Cool");
    tempLedState = 0;
  }
  else if (tempC >= 15 && tempC <= 24) {
    lcd.print("Warm");
    tempLedState = 1;
  }
  else if (tempC > 24) {
    lcd.print("Hot");
    tempLedState = 1;
  }
  delay(100);
}
//-------------------------------------------------------
void dispWind() {
  int avgPotLoc = (int)data[1][2];
  int prevAvgPotLoc = (int)data[2][2];
  lcd.clear();
  lcd.print("Wind:    ");
  if ((502 <= avgPotLoc) && (avgPotLoc <= 522) && ((prevAvgPotLoc - 30) <= avgPotLoc) && (avgPotLoc <= (prevAvgPotLoc + 30))) {
    lcd.print("Still");
    windLedState = 0;
  }
  else if ((402 <= avgPotLoc) && (avgPotLoc <= 622) && !(((prevAvgPotLoc - 50) <= avgPotLoc) && (avgPotLoc <= (prevAvgPotLoc + 50)))) {
    lcd.print("Breezy");
    windLedState = 1;
  }
  else if ((((100 <= avgPotLoc) && (avgPotLoc < 502)) || ((522 < avgPotLoc) && (avgPotLoc <= 924))) && ((prevAvgPotLoc - 50) <= avgPotLoc) && (avgPotLoc <= (prevAvgPotLoc + 50))) {
    lcd.print("Windy");
    windLedState = 1;
  }
  else if (((((100 <= avgPotLoc) && (avgPotLoc < 502)) && ((100 <= prevAvgPotLoc) && (prevAvgPotLoc < 502))) || (((522 < avgPotLoc) && (avgPotLoc <= 924)) && ((522 < avgPotLoc) && (avgPotLoc <= 924)))) && !(((prevAvgPotLoc - 20) <= avgPotLoc) && (avgPotLoc <= (prevAvgPotLoc + 20)))) {
    lcd.print("Gusty");
    windLedState = 1;
  }
  else if ((avgPotLoc <= 100) || (924 <= avgPotLoc)) {
    lcd.print("Stormy");
    windLedState = 2;
  }
  delay(100);
}
//-------------------------------------------------------
void avgData() {
  for (int i = 0; i < 3; i++) {
    data[2][i] = data[1][i];
    data[1][i] = (data[0][i] / (avgInterval / 1000));
  }
//  if(testMode == 0) {
//    data[2][2] = data[1][2];
//    data[1][2] = (data[0][2] / (avgInterval / 10000));
//    data[0][2] = 0;
//  }
//  else if(testMode == 1) {
//    data[2][2] = data[1][2];
//    data[1][2] = (data[0][2] / (avgInterval / 1000));
//    data[0][2] = 0;
//  }
//  
  for (int i = 0; i < 3; i++) {
    data[0][i] = 0;
  }
}
//-------------------------------------------------------
void printToMonitor()
{
  Serial.println("\n------------------------------------------------------------\n");
  Serial.print("Mount Lake Resort Date: ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(monthShortStr(month()));
  Serial.print(" ");
  Serial.print(year());
  Serial.print("\nTime: ");
  digitalClockDisplay();
}
//-------------------------------------------------------
void digitalClockDisplay()    // digital clock display of the time
{
  Serial.print(hour());
  Serial.print(":");
  Serial.print((int)minute());
  Serial.print(":");
  Serial.print((int)second());
  
}
//-------------------------------------------------------
int lcdDelay(int intervalMult) {
  return intervalMult * 100;
}
//-------------------------------------------------------
void pleaseWait() {
  waitCounter++;
  lcd.clear();
  lcd.print("Averaging");
  if (waitCounter == 4)
    waitCounter = 0;
  for (int j = 0; j < waitCounter; j++)
    lcd.print(".");
  delay(100);
}
//-------------------------------------------------------
void ledAlarm() {
  switch (lightLedState) {

  case 0:
    digitalWrite(lightLed, LOW);
    break;
  case 1:
    digitalWrite(lightLed, HIGH);
    break;

  }
  switch (windLedState) {

  case 0:
    digitalWrite(windLed, LOW);
    break;
  case 1:
    if (ledBlink == 0)
      digitalWrite(windLed, HIGH);
    else if (ledBlink == 1)
      digitalWrite(windLed, LOW);
    break;
  case 2:
    digitalWrite(windLed, HIGH);
    break;

  }
  switch (tempLedState) {

  case 0:
    digitalWrite(tempLed, LOW);
    break;
  case 1:
    digitalWrite(tempLed, HIGH);
    break;

  }
}

