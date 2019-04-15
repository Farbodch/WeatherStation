### WeatherStation: A CPSC-1490 Project, Langara College

**Farbod Chamanian**

**Submitted on 2/11/2018**



- Requirements: 
  - Incorporate Object-Oriented Programming concepts to integrate multiple sensors (Temperature, Wind, Light).
  - Display these values on a selectable 16x2 LCD Display (Menu-Like Selecation), along with a message describing the weather.
  - Alarm user via blinking RGB LEDs if severity of weather surpassed critical levels (eg. Stormy vs Breezy).
  
- Input: 
  - LDR Light Sensor Values
  - LMT84LP Temperature Sensor Values
  - Variable Potentiameter Values (Variation of which recorded movement of a low hanging thin metal sheet against wind)
  - Push Button Value (To scroll through Menu)

- Process:
  - Convert raw sensor values (temp & light) to usable data (degrees Celcius/Fahrenheit & Lumens).
  - Compare old/new average instance of the potentiameter location(value) to determine wind intensity.
  
- Output:
  - Display Temperature (degC & degF) and Light Intensity (lumens) along with a descriptive text (Freezing, Cool, Warm, Hot / Dark, Overcast, Bright, Sunny) and Wind Intensity (Still, Breezy, Windy, Gusty, Stormy), on a 16x2 LCD Display.
  - RGB LED Color/Blink to reflect weather condition.
  - (If connected to computer & Arduino IDE), display current date & time on serial monitor -> Can be incorporated into the LCD, given an internal passive clock is added to the circuitary of the project.
