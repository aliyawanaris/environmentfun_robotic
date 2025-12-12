#ifndef WATERCONTROL_H
#define WATERCONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h> 

extern const int servoPin;
extern const int relayPumpPin; 
extern Servo myservo;

void setupWaterControlPins();
void setServoAngle(int angle);
void setPump(bool aktif);

#endif // WATERCONTROL_H