#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

// Variabel Kontrol Global
extern int globalSpeed; 
extern const int maxSpeed;    

/**
 * @brief Menginisialisasi semua pin GPIO motor dan relay.
 */
void setupMotorPins();

/**
 * @brief Mengontrol kecepatan dan arah satu motor (kiri atau kanan).
 */
void setMotor(int channel, int pinA, int pinB, int speed, bool maju);

/**
 * @brief Menghentikan pergerakan kedua motor.
 */
void stopMobil();

/**
 * @brief Mengatur pergerakan mobil secara keseluruhan (maju, mundur, belok).
 */
void bergerak(int speedKiri, bool majuKiri, int speedKanan, bool majuKanan);

/**
 * @brief Mengontrol status ON/OFF Motor Pump melalui Relay.
 * @param aktif True untuk ON, False untuk OFF.
 */
void setPump(bool aktif);

#endif // MOTORCONTROL_H