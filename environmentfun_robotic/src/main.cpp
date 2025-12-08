#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h> // Library untuk membaca index.html dari Flash
#include "WiFiSetup.h"    
#include "MotorControl.h" 

// --- 1. Konfigurasi Server dan WebSocket ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // WebSocket endpoint

// --- 2. Penanganan WebSocket ---

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("WebSocket Client Connected");
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("WebSocket Client Disconnected");
        stopMobil();
        setPump(false); // Matikan Pump jika koneksi terputus
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String message = (char*)data;
            // Ambil kecepatan global dari MotorControl.cpp
            int speed = globalSpeed; 

            // Logika Kontrol Motor
            if (message == "FORWARD") {
                bergerak(speed, true, speed, true);
            } else if (message == "BACKWARD") {
                bergerak(speed, false, speed, false);
            } else if (message == "LEFT") {
                bergerak(speed, false, speed, true); 
            } else if (message == "RIGHT") {
                bergerak(speed, true, speed, false); 
            } else if (message == "STOP") {
                stopMobil();
            } 
            // Logika Kontrol Pompa
            else if (message == "PUMP_ON") {
                setPump(true);
            } else if (message == "PUMP_OFF") {
                setPump(false);
            } 
            // Logika Kontrol Kecepatan
            else if (message.startsWith("SPEED:")) {
                String speedStr = message.substring(6);
                globalSpeed = speedStr.toInt();
            }
        }
    }
}

// --- 3. SETUP dan LOOP ---
void setup() {
    Serial.begin(115200);

    // 1. Inisialisasi Sistem File
    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    Serial.println("LittleFS mounted successfully."); 

    // 2. Inisialisasi Pin Motor DC dan Relay
    // Fungsi ini didefinisikan di MotorControl.cpp
    setupMotorPins(); 
    
    // 3. Panggil Fungsi Setup WiFi
    // Fungsi ini didefinisikan di WiFiSetup.cpp
    setupWiFi(); 

    // 4. Inisialisasi WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // 5. Routing Web Server untuk Halaman Utama (Membaca dari LittleFS)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        // Menyajikan file index.html dari LittleFS
        request->send(LittleFS, "/index.html", "text/html"); 
    });
//
    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    // Tugas membersihkan klien WebSocket yang terputus
    ws.cleanupClients();
}