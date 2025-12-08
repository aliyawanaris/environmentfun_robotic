#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

// --- 1. Konfigurasi Pin Motor DC (L298N) ---
// Motor Kiri
extern const int motorKiri_A; 
extern const int motorKiri_B; 
extern const int enableKiri; 

// Motor Kanan
extern const int motorKanan_A;
extern const int motorKanan_B; 
extern const int enableKanan; 

// --- 2. Konfigurasi Motor Pump (Relay 1 Channel) ---
extern const int relayPumpPin; 

// Konfigurasi PWM
extern const int freq;
extern const int res; 
extern const int chKiri;
extern const int chKanan;

// Variabel Kontrol Global
extern int globalSpeed; 
extern const int maxSpeed;    

// --- 3. Deklarasi Fungsi Kontrol ---

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