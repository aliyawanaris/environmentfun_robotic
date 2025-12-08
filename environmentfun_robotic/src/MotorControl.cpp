#include "MotorControl.h"

// --- 1. Definisi Konstanta Pin Motor DC (L298N) ---
// Motor Kiri
const int motorKiri_A = 27; 
const int motorKiri_B = 26; 
const int enableKiri = 14;  // PWM Channel 0

// Motor Kanan
const int motorKanan_A = 25;
const int motorKanan_B = 33; 
const int enableKanan = 32;  // PWM Channel 1

// --- 2. Definisi Konstanta Pin Motor Pump (Relay 1 Channel) ---
const int relayPumpPin = 13; 

// Konfigurasi PWM
const int freq = 5000;
const int res = 8; // Resolusi 8 bit (0-255)
const int chKiri = 0;
const int chKanan = 1;

// Variabel Kontrol Global
int globalSpeed = 150; 
const int maxSpeed = 255;    

// --- 3. Implementasi Fungsi ---

void setupMotorPins() {
    // Inisialisasi Pin Motor DC dan PWM
    pinMode(motorKiri_A, OUTPUT);
    pinMode(motorKiri_B, OUTPUT);
    pinMode(motorKanan_A, OUTPUT);
    pinMode(motorKanan_B, OUTPUT);
    
    ledcSetup(chKiri, freq, res);
    ledcAttachPin(enableKiri, chKiri);
    ledcSetup(chKanan, freq, res);
    ledcAttachPin(enableKanan, chKanan);

    // Inisialisasi Pin Relay
    pinMode(relayPumpPin, OUTPUT);
    // Pastikan pompa mati saat startup (HIGH untuk Active-LOW)
    digitalWrite(relayPumpPin, HIGH); 

    stopMobil(); 
}

void setMotor(int channel, int pinA, int pinB, int speed, bool maju) {
    speed = constrain(speed, 0, maxSpeed);
    ledcWrite(channel, speed);

    if (speed == 0) {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, LOW);
    } else if (maju) {
        digitalWrite(pinA, HIGH);
        digitalWrite(pinB, LOW);
    } else {
        digitalWrite(pinA, LOW);
        digitalWrite(pinB, HIGH);
    }
}

void stopMobil() {
    setMotor(chKiri, motorKiri_A, motorKiri_B, 0, true);
    setMotor(chKanan, motorKanan_A, motorKanan_B, 0, true);
}

void bergerak(int speedKiri, bool majuKiri, int speedKanan, bool majuKanan) {
    setMotor(chKiri, motorKiri_A, motorKiri_B, speedKiri, majuKanan); // Perhatikan: Motor Kanan biasanya diputar terbalik untuk maju
    setMotor(chKanan, motorKanan_A, motorKanan_B, speedKanan, majuKiri); // Perhatikan: Motor Kiri biasanya diputar normal untuk maju
}

void setPump(bool aktif) {
    // Relay Active-LOW: LOW = ON, HIGH = OFF
    if (aktif) {
        digitalWrite(relayPumpPin, LOW); 
        Serial.println("Motor Pump ON");
    } else {
        digitalWrite(relayPumpPin, HIGH);
        Serial.println("Motor Pump OFF");
    }
}