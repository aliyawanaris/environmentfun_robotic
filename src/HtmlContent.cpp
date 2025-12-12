#include "HtmlContent.h"

// Definisi Konstanta HTML (Ganti seluruh konten ini dengan kode HTML yang sudah dirapikan)
const char* INDEX_HTML = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Kontrol Robot Modular</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* 1. Global & Typography */
    body { 
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
        text-align: center; 
        background-color: #f4f7f6; 
        padding: 10px;
        margin: 0;
    }
    h1 { color: #333; font-size: 1.6em; margin-bottom: 5px; }
    h4 { margin: 10px 0 5px 0; color: #555; }
    
    .container { 
        max-width: 400px; 
        margin: 0 auto;
    }
    
    /* 2. Card Styling */
    .card {
        background-color: #fff;
        border-radius: 12px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
        padding: 15px;
        margin-bottom: 18px;
    }
    
    /* 3. Status Badge */
    .status-badge {
        font-weight: bold;
        padding: 6px 12px;
        border-radius: 8px;
        display: inline-block;
        font-size: 0.9em;
    }
    #wsState[data-connected="true"] { background-color: #4CAF50; color: white; }
    #wsState[data-connected="false"] { background-color: #f44336; color: white; }
    
    /* 4. Kontrol Drive (Grid) */
    .control-grid {
        /* TATA LETAK: Baris 1: Maju | Baris 2: Kiri - Kosong - Kanan | Baris 3: Mundur */
        display: grid;
        grid-template-areas: 
            "forward forward forward"
            "left . right" 
            "backward backward backward";
        gap: 12px;
        width: 100%;
        margin: 15px 0 10px 0;
    }
    .btn {
        padding: 18px 10px;
        font-size: 1.1em;
        font-weight: 600;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        user-select: none;
        transition: background-color 0.1s;
    }
    .btn:active { transform: scale(0.97); }

    /* Drive Colors */
    #forward, #backward { background-color: #007bff; color: white; }
    #left, #right { background-color: #ffc107; color: #333; }
    
    /* GAYA STOP DARURAT */
    #emergencyStop {
        background-color: #dc3545; /* Merah Menyala */
        color: white;
        padding: 15px 20px;
        font-size: 1.5em;
        font-weight: bold;
        width: 100%;
        border: none;
        border-radius: 12px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);
        margin-bottom: 20px;
        cursor: pointer;
    }

    /* 5. Slider Group (Digunakan juga untuk Servo) */
    .slider-group {
        display: flex;
        align-items: center;
        margin: 10px 0;
        padding: 0 5px;
    }
    .slider-group span { min-width: 50px; text-align: left; font-weight: 600; }
    input[type=range] { flex-grow: 1; margin: 0 10px; }
    
    /* 6. Aksesori & Presets */
    .pump-btn, .servo-btn {
        background-color: #6f42c1; 
        color: white;
        padding: 10px 15px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        margin: 4px;
        font-weight: 500;
        min-width: 80px;
    }
    .pump-btn.danger {
        background-color: #dc3545;
    }
    .servo-btn-group {
        display: flex;
        justify-content: space-around;
        margin: 8px 0;
    }
    /* Sinkronisasi UI */
    .current-angle {
        font-size: 1.1em;
        font-weight: bold;
        color: #007bff;
        margin-top: 5px;
    }
    .slider-info {
        display: flex;
        justify-content: space-between;
        font-size: 0.8em;
        color: #777;
        margin: -5px 10px 5px 10px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Jawara Bot</h1>
    
    <div class="card status-card">
        <h4>Status & Kecepatan</h4>
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
        <h4>Kontrol Transportasi</h4>
        <div class="control-grid">
            <button class="btn" id="forward" ontouchstart="sendCmd('FORWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('FORWARD')" onmouseup="sendCmd('STOP')" style="grid-area: forward;">MAJU</button>

            <button class="btn" id="left" ontouchstart="sendCmd('LEFT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('LEFT')" onmouseup="sendCmd('STOP')" style="grid-area: left;">KIRI</button>
            <button class="btn" id="right" ontouchstart="sendCmd('RIGHT')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('RIGHT')" onmouseup="sendCmd('STOP')" style="grid-area: right;">KANAN</button>
            
            <button class="btn" id="backward" ontouchstart="sendCmd('BACKWARD')" ontouchend="sendCmd('STOP')" onmousedown="sendCmd('BACKWARD')" onmouseup="sendCmd('STOP')" style="grid-area: backward;">MUNDUR</button>
        </div>
        
        <button id="emergencyStop" onclick="sendCmd('STOP')">STOP DARURAT</button>
        
    </div>

    <div class="card">
        <h4>Kontrol Siram Air</h4>
        
        <h5>Pompa Air</h5>
        <button class="pump-btn" onclick="sendCmd('PUMP_ON')">On</button>
        <button class="pump-btn danger" onclick="sendCmd('PUMP_OFF')">Off</button>

        <h5>Rotasi Servo</h5>
        <div class="current-angle">Sudut: <span id="servoVal">90</span>°</div>
        
        <div class="slider-group">
            <input type="range" min="10" max="170" value="90" id="servoSlider" oninput="updateServoVal(this.value)" onchange="sendServoAngle(this.value)">
        </div>
        <div class="slider-info">
            <span>Kiri (10°)</span>
            <span>Tengah (90°)</span>
            <span>Kanan (170°)</span>
        </div>
        
        </div>
  </div>

  <script>
    var connection = new WebSocket('ws://' + location.hostname + '/ws');
    var speedSlider = document.getElementById('speedSlider');
    var servoSlider = document.getElementById('servoSlider'); // Ambil slider servo baru
    var wsState = document.getElementById('wsState');

    connection.onopen = function () {
      wsState.innerHTML = "CONNECTED";
      wsState.setAttribute('data-connected', 'true');
      sendSpeed(speedSlider.value); 
      sendServoAngle(servoSlider.value); // Set posisi awal servo dari slider (90)
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
        // Update tampilan sudut saat slider digerakkan (oninput)
        document.getElementById('servoVal').innerHTML = angle;
    }
    
    function sendServoAngle(angle) {
      // Fungsi ini dipanggil saat slider diubah (onchange)
      angle = parseInt(angle);
      // Batas Aman (Sesuai dengan kode C++: 10° hingga 170°)
      if (isNaN(angle) || angle < 10 || angle > 170) { 
          return;
      }
      updateServoVal(angle); 
      if (connection.readyState === 1) {
        connection.send("SERVO:" + angle);
      }
    }

  </script>
</body>
</html>
)rawliteral";