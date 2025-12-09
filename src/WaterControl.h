#ifndef WATERCONTROL_H
#define WATERCONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h> // Membutuhkan library Servo

// --- 1. Konfigurasi Pin ---
extern const int servoPin;
extern const int relayPumpPin; 

// --- 2. Objek & Variabel ---
extern Servo myservo;

// --- 3. Deklarasi Fungsi Kontrol Air ---

/**
 * @brief Menginisialisasi pin relay dan mengaitkan objek servo ke pin.
 */
void setupWaterControlPins();

/**
 * @brief Mengatur sudut putaran servo (0-180 derajat).
 * @param angle Sudut target.
 */
void setServoAngle(int angle);

/**
 * @brief Mengontrol status ON/OFF Motor Pump 12V melalui Relay.
 * @param aktif True untuk ON (Low), False untuk OFF (High).
 */
void setPump(bool aktif);

#endif // WATERCONTROL_H