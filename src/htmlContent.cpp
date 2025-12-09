#include "htmlContent.h"

// Definisi Konstanta HTML (Ganti seluruh konten ini dengan kode HTML yang sudah dirapikan)
const char* INDEX_HTML = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Kontrol Robot Modular</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
        text-align: center; 
        background-color: #f4f7f6; 
        padding: 10px;
        margin: 0;
    }
    h1 { color: #333; font-size: 1.5em; }
    .container { 
        max-width: 400px; 
        margin: 0 auto;
    }
    .card {
        background-color: #fff;
        border-radius: 12px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
        padding: 15px;
        margin-bottom: 20px;
    }
    .status-badge {
        font-weight: bold;
        padding: 5px 10px;
        border-radius: 8px;
        display: inline-block;
        margin-bottom: 10px;
    }
    #wsState[data-connected="true"] { background-color: #4CAF50; color: white; }
    #wsState[data-connected="false"] { background-color: #f44336; color: white; }
    
    /* Grid Kontrol Drive */
    .control-grid {
        display: grid;
        grid-template-areas: ". forward ." "left stop right" ". backward .";
        gap: 10px;
        width: 100%;
        margin: 10px 0;
    }
    .btn {
        padding: 20px 10px;
        font-size: 1.2em;
        font-weight: bold;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        user-select: none;
        transition: background-color 0.1s;
    }
    .btn:active { transform: scale(0.98); }

    #forward { grid-area: forward; background-color: #4CAF50; color: white; }
    #backward { grid-area: backward; background-color: #2196F3; color: white; }
    #left { grid-area: left; background-color: #ff9800; color: white; }
    #right { grid-area: right; background-color: #ff9800; color: white; }
    #stop { grid-area: stop; background-color: #f44336; color: white; }
    
    /* Kontrol Aksesori dan Slider */
    .slider-group {
        display: flex;
        align-items: center;
        margin-top: 10px;
    }
    .slider-group span { width: 60px; text-align: left; }
    input[type=range] { flex-grow: 1; margin: 0 10px; }

    .pump-btn, .servo-btn {
        background-color: #9C27B0;
        color: white;
        padding: 10px 15px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        margin: 5px;
    }
    .servo-btn-group {
        display: flex;
        justify-content: space-around;
        margin-bottom: 10px;
    }
    .servo-input-group {
        display: flex;
        align-items: center;
        gap: 5px;
        margin-top: 10px;
    }
    .servo-input-group input[type="number"] {
        width: 80px;
        padding: 5px;
        border: 1px solid #ccc;
        border-radius: 4px;
        text-align: center;
    }
    .servo-input-group button {
        background-color: #007bff;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Kontrol Robot Tank ESP32</h1>
    
    <div class="card status-card">
        <h3>Status & Kecepatan</h3>
        <p>Koneksi: 
            <span id="wsState" class="status-badge" data-connected="false">DISCONNECTED</span>
        </p>
        <div class="slider-group">
            <span>Speed: </span>
            <input type="range" min="0" max="255" value="150" id="speedSlider" oninput="updateSpeedVal(this.value)" onchange="sendSpeed(this.value)">
            <span id="speedVal">150</span> / 255
        </div>
    </div>

    <div class="card">
        <h3>Kontrol Drive Rantai</h3>
        <div class="control-grid">
            <button class="btn" id="forward" ontouchstart="sendCmd('FORWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('FORWARD')" onmouseup="sendCmd('STOP')">MAJU ▲</button>
            <button class="btn" id="left" ontouchstart="sendCmd('LEFT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('LEFT')" onmouseup="sendCmd('STOP')">KIRI ◀</button>
            <button class="btn" id="stop" ontouchstart="sendCmd('STOP')" onmouseup="sendCmd('STOP')" onmousedown="sendCmd('STOP')" onmouseup="sendCmd('STOP')">STOP</button>
            <button class="btn" id="right" ontouchstart="sendCmd('RIGHT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('RIGHT')" onmouseup="sendCmd('STOP')">KANAN ▶</button>
            <button class="btn" id="backward" ontouchstart="sendCmd('BACKWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('BACKWARD')" onmouseup="sendCmd('STOP')">MUNDUR ▼</button>
        </div>
    </div>

    <div class="card">
        <h3>Kontrol Aksesori (Water Control)</h3>
        
        <h4>Motor Pump (12V)</h4>
        <button class="pump-btn" onclick="sendCmd('PUMP_ON')">NYALAKAN PUMP</button>
        <button class="pump-btn" onclick="sendCmd('PUMP_OFF')">MATIKAN PUMP</button>

        <h4>Servo (MG996R)</h4>
        <p>Posisi Saat Ini: <span id="servoVal">90</span>°</p>

        <div class="servo-btn-group">
            <button class="servo-btn" onclick="sendServoAngle(0)">KIRI (0°)</button>
            <button class="servo-btn" onclick="sendServoAngle(90)">TENGAH (90°)</button>
            <button class="servo-btn" onclick="sendServoAngle(180)">KANAN (180°)</button>
        </div>

        <div class="servo-input-group">
            <span>Sudut Kustom (0-180):</span>
            <input type="number" min="0" max="180" value="90" id="servoInput">
            <button class="servo-btn" onclick="sendCustomServo()">SET</button>
        </div>

        <div class="slider-group">
            <span>Fine Tune: </span>
            <input type="range" min="0" max="180" value="90" id="servoSlider" oninput="updateServoVal(this.value)" onchange="sendServoAngle(this.value)">
        </div>
    </div>
  </div>

  <script>
    var connection = new WebSocket('ws://' + location.hostname + '/ws');
    var speedSlider = document.getElementById('speedSlider');
    var servoSlider = document.getElementById('servoSlider'); 
    var servoInput = document.getElementById('servoInput'); 
    var wsState = document.getElementById('wsState');

    connection.onopen = function () {
      wsState.innerHTML = "CONNECTED";
      wsState.setAttribute('data-connected', 'true');
      sendSpeed(speedSlider.value); 
      sendServoAngle(90); 
    };

    connection.onclose = function () {
      wsState.innerHTML = "DISCONNECTED";
      wsState.setAttribute('data-connected', 'false');
    };

    function sendCmd(cmd) {
      if (connection.readyState === 1) { 
        connection.send(cmd);
      }
    }

    function updateSpeedVal(speed) {
        document.getElementById('speedVal').innerHTML = speed;
    }

    function sendSpeed(speed) {
      if (connection.readyState === 1) {
        connection.send("SPEED:" + speed);
      }
    }

    function updateServoVal(angle) {
        // Update tampilan posisi dan nilai input
        document.getElementById('servoVal').innerHTML = angle;
        servoInput.value = angle;
        servoSlider.value = angle;
    }
    
    function sendServoAngle(angle) {
      angle = parseInt(angle);
      if (isNaN(angle) || angle < 0 || angle > 180) {
          alert("Sudut harus antara 0 dan 180.");
          return;
      }
      updateServoVal(angle); 
      if (connection.readyState === 1) {
        connection.send("SERVO:" + angle);
      }
    }
    
    function sendCustomServo() {
        var angle = servoInput.value;
        sendServoAngle(angle);
    }
    
    servoSlider.onchange = function() {
        sendServoAngle(this.value);
    };

  </script>
</body>
</html>
)rawliteral";