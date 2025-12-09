#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "WiFiSetup.h"    
#include "MotorControl.h" 
#include "WaterControl.h" 
#include "htmlContent.h" // Berisi konstanta INDEX_HTML

// --- 1. Konfigurasi Server dan WebSocket ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // WebSocket endpoint

// --- 2. Penanganan WebSocket ---

/**
 * @brief Fungsi callback untuk menangani event (data, koneksi, diskoneksi) WebSocket.
 */
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("WebSocket Client Connected");
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("WebSocket Client Disconnected");
        // Matikan semua fungsi saat koneksi terputus
        stopMobil();
        setPump(false); 
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String message = (char*)data;
            
            // Ambil kecepatan global dari MotorControl.cpp (extern int globalSpeed)
            int speed = globalSpeed; 

            // --- Logika Kontrol Motor Drive ---
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
            
            // --- Logika Kontrol Pompa dan Servo (Water Control) ---
            else if (message == "PUMP_ON") {
                setPump(true);
            } else if (message == "PUMP_OFF") {
                setPump(false);
            } 
            else if (message.startsWith("SERVO:")) {
                String angleStr = message.substring(6);
                int angle = angleStr.toInt();
                setServoAngle(angle);
            }
            
            // --- Logika Kontrol Kecepatan ---
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

    // 1. Inisialisasi Pin Motor Drive
    // Fungsi ini didefinisikan di MotorControl.cpp
    setupMotorPins(); 
    
    // 2. Inisialisasi Pin Relay dan Servo
    // Fungsi ini didefinisikan di WaterControl.cpp
    setupWaterControlPins(); 
    
    // 3. Panggil Fungsi Setup WiFi
    // Fungsi ini didefinisikan di WiFiSetup.cpp
    setupWiFi(); 

    // 4. Inisialisasi WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // 5. Routing Web Server untuk Halaman Utama
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        // Mengirim konten HTML langsung dari HtmlContent.cpp
        request->send_P(200, "text/html", INDEX_HTML); 
    });

    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    // Membersihkan klien WebSocket yang terputus secara berkala
    ws.cleanupClients();
}