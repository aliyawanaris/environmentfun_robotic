#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

extern int globalSpeed; 
extern const int maxSpeed;    

void setupMotorPins();
void setMotor(int channel, int pinA, int pinB, int speed, bool maju);
void stopMobil();
void bergerak(int speedKiri, bool majuKiri, int speedKanan, bool majuKanan);

#endif // MOTORCONTROL_H