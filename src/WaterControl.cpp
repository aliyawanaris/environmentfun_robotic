#include "WaterControl.h"

// --- 1. Definisi Konstanta Pin ---
const int servoPin = 18; // GPIO 12 untuk Servo MG996R
const int relayPumpPin = 13; // GPIO 13 untuk Relay Motor Pump

// --- 2. Definisi Objek Servo ---
Servo myservo;

// --- 3. Implementasi Fungsi ---

void setupWaterControlPins() {
    // Inisialisasi Pin Relay
    pinMode(relayPumpPin, OUTPUT);
    digitalWrite(relayPumpPin, HIGH); // Matikan pompa saat startup (HIGH untuk Active-LOW)

    // Inisialisasi Servo
    myservo.setPeriodHertz(50); // Servo standar 50Hz
    // Attach pin dengan pulsa min/max standar untuk MG996R
    myservo.attach(servoPin, 500, 2400); 
    
    // Atur posisi awal servo (misalnya, 90 derajat)
    setServoAngle(90); 

    Serial.println("Water Control (Pump & Servo) Initialized.");
}

void setServoAngle(int angle) {
    // Pastikan sudut antara 0 dan 180
    angle = constrain(angle, 0, 180);
    myservo.write(angle);
    Serial.print("Servo set to: ");
    Serial.println(angle);
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