#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "WiFiSetup.h"    
#include "MotorControl.h" 
#include "WaterControl.h" 
#include "HtmlContent.h" 

// --- 1. Konfigurasi Server dan WebSocket ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); 

// --- 2. Penanganan WebSocket (Logic Rotasi Diperbaiki) ---

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("WebSocket Client Connected");
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("WebSocket Client Disconnected");
        stopMobil();
        setPump(false); 
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String message = (char*)data;
            int speed = globalSpeed; 

            // Logika Kontrol Motor Drive
            if (message == "FORWARD") {
                bergerak(speed, true, speed, true);
            } else if (message == "BACKWARD") {
                bergerak(speed, false, speed, false);
            } 
            // ROTASI DIPERBAIKI: Jika KIRI terbalik, buat roda kanan maju dan roda kiri mundur
            else if (message == "LEFT") {
                // Motor Kiri Mundur, Motor Kanan Maju
                bergerak(speed, false, speed, true); 
            } 
            // ROTASI DIPERBAIKI: Jika KANAN terbalik, buat roda kiri maju dan roda kanan mundur
            else if (message == "RIGHT") {
                // Motor Kiri Maju, Motor Kanan Mundur
                bergerak(speed, true, speed, false); 
            } 
            else if (message == "STOP") {
                stopMobil();
            } 
            // Logika Kontrol Pompa
            else if (message == "PUMP_ON") {
                setPump(true);
            } else if (message == "PUMP_OFF") {
                setPump(false);
            } 
            // Logika Kontrol Servo
            else if (message.startsWith("SERVO:")) {
                String angleStr = message.substring(6);
                int angle = angleStr.toInt();
                setServoAngle(angle);
            }
            // Logika Kontrol Kecepatan
            else if (message.startsWith("SPEED:")) {
                String speedStr = message.substring(6);
                globalSpeed = speedStr.toInt();
            }
        }
    }
}

// --- 3. SETUP dan LOOP (Tidak Berubah) ---

void setup() {
    Serial.begin(115200);

    setupMotorPins(); 
    setupWaterControlPins(); 
    setupWiFi(); 

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", INDEX_HTML); 
    });

    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    ws.cleanupClients();
}