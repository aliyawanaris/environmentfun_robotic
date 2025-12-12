#include "WaterControl.h"

// Pin sinyal (Direkomendasikan pindah dari GPIO 12 ke GPIO 18/23)
const int servoPin = 12; 
const int relayPumpPin = 13; 

Servo myservo;

void setupWaterControlPins() {
    pinMode(relayPumpPin, OUTPUT);
    digitalWrite(relayPumpPin, HIGH); // Relay OFF/Mati saat start

    myservo.setPeriodHertz(50); 
    // Optimasi: Mempersempit rentang pulsa (600µs - 2300µs) untuk stabilitas
    myservo.attach(servoPin, 600, 2300); 
    
    myservo.write(90); 

    Serial.println("Water Control Initialized.");
}

void setServoAngle(int angle) {
    // Optimasi: Batasi sudut (10° - 170°) untuk mencegah jitter di titik ekstrem
    angle = constrain(angle, 10, 170); 
    myservo.write(angle);
    Serial.print("Servo set to: ");
    Serial.println(angle);
}

void setPump(bool aktif) {
    if (aktif) {
        digitalWrite(relayPumpPin, LOW); 
        Serial.println("Motor Pump ON");
    } else {
        digitalWrite(relayPumpPin, HIGH);
        Serial.println("Motor Pump OFF");
    }
}