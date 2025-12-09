#include "WiFiSetup.h"
#include <Arduino.h> // Diperlukan untuk fungsi delay dan Serial

// --- DEFINISI KONSTANTA (HARUS DIUBAH) ---
const char* ssid = "Pikan Miku";       // <<== GANTI!
const char* password = "jasamaru123"; // <<== GANTI!

/**
 * @brief Implementasi fungsi untuk inisialisasi koneksi WiFi.
 */
IPAddress setupWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    // Tunggu hingga terhubung
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    return WiFi.localIP();
}