#include "WiFiSetup.h"
#include <Arduino.h>
#include <WiFi.h>

// --- Konfigurasi Access Point (AP) ---
// GANTI DENGAN NAMA JARINGAN DAN PASSWORD YANG ANDA INGINKAN
const char* ssid = "JawaraBot_AP";       
const char* password = "jawarabot123"; 

// IP Address AP: Default ESP32 adalah 192.168.4.1
IPAddress localIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress setupWiFi() {
    Serial.println("Starting ESP32 in Access Point (AP) Mode...");

    // Mengatur konfigurasi jaringan AP
    if (!WiFi.softAPConfig(localIP, gateway, subnet)) {
        Serial.println("AP Configuration Failed!");
    }

    // Memulai Access Point
    // Argumen 1: SSID, Argumen 2: Password, Argumen 3: Channel, Argumen 4: Hidden, Argumen 5: Max connections
    bool result = WiFi.softAP(ssid, password, 1, false, 4);

    if (result) {
        Serial.println("WiFi Access Point Started.");
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Connect to IP: ");
        Serial.println(WiFi.softAPIP()); // Menampilkan IP internal (192.168.4.1)
    } else {
        Serial.println("WiFi AP Failed to Start.");
    }
    
    return WiFi.softAPIP(); // Mengembalikan IP 192.168.4.1
}