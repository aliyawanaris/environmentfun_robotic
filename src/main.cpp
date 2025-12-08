#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// --- 1. Konfigurasi Jaringan & Server ---
const char* ssid = "Pikan Miku";       // Ganti dengan SSID WiFi Anda
const char* password = "jasamaru123"; // Ganti dengan Password WiFi Anda

AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // Endpoint WebSocket

// --- 2. Konfigurasi Pin Motor dan PWM ---
// Pin Motor Kiri (L298N IN1/IN2 & ENA)
const int motorKiri_A = 27; 
const int motorKiri_B = 26; 
const int enableKiri = 14;  // PWM Channel 0

// Pin Motor Kanan (L298N IN3/IN4 & ENB)
const int motorKanan_A = 25;
const int motorKanan_B = 33; // CATATAN: Pastikan GPIO33 tidak konflik dengan Selenoid Kopi Anda.
const int enableKanan = 32;  // PWM Channel 1

// Konfigurasi PWM
const int freq = 5000;
const int res = 8; // Resolusi 8 bit (0-255)
const int chKiri = 0;
const int chKanan = 1;

// Variabel Kontrol Global
int globalSpeed = 150; // Kecepatan default
const int maxSpeed = 255;    

// --- 3. Fungsi Kontrol Motor ---

/**
 * @brief Mengatur kecepatan dan arah motor menggunakan PWM.
 */
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

/**
 * @brief Menghentikan kedua motor.
 */
void stopMobil() {
    setMotor(chKiri, motorKiri_A, motorKiri_B, 0, true);
    setMotor(chKanan, motorKanan_A, motorKanan_B, 0, true);
}

/**
 * @brief Menggerakkan mobil dengan kecepatan dan arah yang berbeda (Differential Drive).
 */
void bergerak(int speedKiri, bool majuKiri, int speedKanan, bool majuKanan) {
    setMotor(chKiri, motorKiri_A, motorKiri_B, speedKiri, majuKiri);
    setMotor(chKanan, motorKanan_A, motorKanan_B, speedKanan, majuKanan);
}

// --- 4. Penanganan WebSocket ---

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("WebSocket Client Connected");
        client->text("Connected");
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("WebSocket Client Disconnected");
        stopMobil();
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            String message = (char*)data;
            // Serial.println(message); // Uncomment untuk debug

            int speed = globalSpeed;

            if (message == "FORWARD") {
                bergerak(speed, true, speed, true);
            } else if (message == "BACKWARD") {
                bergerak(speed, false, speed, false);
            } else if (message == "LEFT") {
                // Berputar kiri (Kiri Mundur, Kanan Maju)
                bergerak(speed, false, speed, true); 
            } else if (message == "RIGHT") {
                // Berputar kanan (Kiri Maju, Kanan Mundur)
                bergerak(speed, true, speed, false); 
            } else if (message == "STOP") {
                stopMobil();
            } else if (message.startsWith("SPEED:")) {
                // Mengatur kecepatan dari slider
                String speedStr = message.substring(6);
                globalSpeed = speedStr.toInt();
            }
        }
    }
}

// --- 5. Halaman HTML dengan WebSocket dan Slider ---

// HTML/CSS/JS di dalam R"rawliteral()" disimpan di Flash Memory (PROGMEM)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Kontrol Tank WebSocket</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin: auto; padding-top: 20px; }
    .btn { padding: 20px; font-size: 24px; margin: 5px; width: 100px; border-radius: 10px; cursor: pointer; user-select: none; }
    #forward { background-color: #4CAF50; color: white; }
    #backward { background-color: #2196F3; color: white; }
    #left, #right { background-color: #ff9800; color: white; }
    #stop { background-color: #f44336; color: white; width: 110px; }
    .control-grid { 
        display: grid; 
        grid-template-areas: ". forward ." "left stop right" ". backward ."; 
        width: 350px; margin: 20px auto; 
    }
    #forward { grid-area: forward; }
    #backward { grid-area: backward; }
    #left { grid-area: left; }
    #right { grid-area: right; }
    #stop { grid-area: stop; }
  </style>
</head>
<body>
  <h1>Kontrol Mobil Rantai ESP32</h1>
  
  <p>Status Koneksi: <span id="wsState" style="color:red;">DISCONNECTED</span></p>

  <div style="margin: 20px;">
    <strong>Speed: </strong> <span id="speedVal">150</span> / 255
    <input type="range" min="0" max="255" value="150" id="speedSlider" onchange="sendSpeed(this.value)">
  </div>

  <div class="control-grid">
    <button class="btn" id="forward" ontouchstart="sendCmd('FORWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('FORWARD')" onmouseup="sendCmd('STOP')">▲</button>
    <button class="btn" id="left" ontouchstart="sendCmd('LEFT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('LEFT')" onmouseup="sendCmd('STOP')">◀</button>
    <button class="btn" id="stop" ontouchstart="sendCmd('STOP')" onmouseup="sendCmd('STOP')" onmousedown="sendCmd('STOP')" onmouseup="sendCmd('STOP')">STOP</button>
    <button class="btn" id="right" ontouchstart="sendCmd('RIGHT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('RIGHT')" onmouseup="sendCmd('STOP')">▶</button>
    <button class="btn" id="backward" ontouchstart="sendCmd('BACKWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('BACKWARD')" onmouseup="sendCmd('STOP')">▼</button>
  </div>

  <script>
    var connection = new WebSocket('ws://' + location.hostname + '/ws');
    var speedSlider = document.getElementById('speedSlider');
    var speedVal = document.getElementById('speedVal');
    var wsState = document.getElementById('wsState');

    connection.onopen = function () {
      wsState.innerHTML = "CONNECTED";
      wsState.style.color = "green";
      console.log('WebSocket connection opened');
      sendSpeed(speedSlider.value); // Kirim speed awal ke ESP32
    };

    connection.onclose = function () {
      wsState.innerHTML = "DISCONNECTED";
      wsState.style.color = "red";
      console.log('WebSocket connection closed');
    };

    function sendCmd(cmd) {
      if (connection.readyState === 1) { // 1 = OPEN
        connection.send(cmd);
      }
    }

    function sendSpeed(speed) {
      speedVal.innerHTML = speed;
      if (connection.readyState === 1) {
        connection.send("SPEED:" + speed);
      }
    }
  </script>
</body>
</html>
)rawliteral";

// --- 6. SETUP dan LOOP ---
void setup() {
    Serial.begin(115200);

    // Inisialisasi Pin Motor dan PWM
    pinMode(motorKiri_A, OUTPUT);
    pinMode(motorKiri_B, OUTPUT);
    pinMode(motorKanan_A, OUTPUT);
    pinMode(motorKanan_B, OUTPUT);
    
    // Setup PWM channels
    ledcSetup(chKiri, freq, res);
    ledcAttachPin(enableKiri, chKiri);
    ledcSetup(chKanan, freq, res);
    ledcAttachPin(enableKanan, chKanan);

    stopMobil(); // Pastikan motor berhenti saat booting

    // Koneksi ke WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 

    // Inisialisasi WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // Routing Web Server untuk Halaman Utama
    // Menggunakan send() untuk mengirim HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });

    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    // Tugas membersihkan klien WebSocket yang terputus
    ws.cleanupClients();
    // Loop kosong karena AsyncWebServer menangani permintaan di latar belakang
}