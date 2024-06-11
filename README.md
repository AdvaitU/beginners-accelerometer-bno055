# Beginners Accelerometer BNO055
Advait Ukidve - June 2023

## Description
Beginner friendly library for using the [Adafruit BNO055 9-Axis Absolute Orientation Sensor](https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview) compatible with Arduino. Contains easy to understand and documented functions to get sensor readings and translate them to the more understandable degrees unit.

## How to Use
Download or clone the repository and place it in the same folder as your Arduino sketch. Put the following in your main Arduino Sketch at the top (outside of setup() or loop())
> #include Accelerometer.h     // To include the library header file        
> Accelerometer acc;           // To create an object of the Accelerometer class // defines pins numbers

Example use case can be found [here](https://github.com/AdvaitU/endgame/tree/main/Arduino_Code). Check accelerometer_test.ino for implementation of the library in Arduiino code

## Prerequisites
Install the following libraries using your Arduino IDE:
- Wire.h *(Should already be installed)*
- [Adafruit Unified Sensor Library](https://www.arduino.cc/reference/en/libraries/adafruit-unified-sensor/)
- [Adafruit BNO055 Library](https://www.arduino.cc/reference/en/libraries/adafruit-bno055/)

## Wiring
All Arduinos and most standard microcontrollers allow for I2C communication using the 'SDA' and 'SCL' pins on board. Wire them up as follows:     
**BNO055 ---->  Arduino**     
Vin    ---->  5V     
GND    ---->  GND     
SDA    ---->  SDA     
SCL    ---->  SCL     

# Documentation of Functions
Download folder sgtarting with "V2" for latest implementation. Documentation is contained in the comments of the BeginnersBNO055.h Header file while the example shows implementation for Arduino .ino sketch using the library.

