#include "WiFiSetup.h"
#include <Arduino.h> 

const char* ssid = "Pikan Miku";       // <<== HARUS DIGANTI!
const char* password = "jasamaru123"; // <<== HARUS DIGANTI!

IPAddress setupWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    return WiFi.localIP();
}