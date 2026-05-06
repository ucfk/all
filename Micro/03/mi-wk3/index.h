const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body { 
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
            background-color: #e9ecef; 
            margin: 0; 
            display: flex; 
            justify-content: center; 
            align-items: center; 
            min-height: 100vh;
        }
        .container { 
            background: white; 
            padding: 40px; 
            border-radius: 25px; 
            box-shadow: 0 15px 35px rgba(0,0,0,0.15); 
            text-align: center; 
            width: 95%; 
            max-width: 500px;
        }
        h1 { color: #2d3436; font-size: 24px; margin-bottom: 30px; line-height: 1.4; }
        .btn-group { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom: 25px; }
        button { 
            border: none; 
            color: white; 
            padding: 18px; 
            font-size: 16px; 
            font-weight: bold; 
            border-radius: 15px; 
            cursor: pointer; 
            transition: all 0.2s ease; 
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        button:active { transform: scale(0.92); }
        .btn-on { background-color: #ff5252; }
        .btn-on:hover { background-color: #ff1744; filter: brightness(1.1); }
        .btn-off { background-color: #448aff; }
        .btn-off:hover { background-color: #2979ff; filter: brightness(1.1); }
        
        .status-box { 
            background: #f8f9fa; 
            padding: 20px; 
            border-radius: 15px; 
            margin-top: 25px; 
            border-left: 6px solid #00b894;
            text-align: left;
        }
        .label { font-weight: bold; color: #636e72; font-size: 15px; }
        .value { color: #00b894; font-weight: bold; font-size: 16px; }

        /* --- ปรับแต่งปุ่ม BY KIT CHANCHUANG --- */
        .footer-link { 
            display: inline-block;
            margin-top: 35px; 
            padding: 15px 30px;
            background: linear-gradient(45deg, #0984e3, #6c5ce7); /* สี Gradient น้ำเงิน-ม่วง */
            color: white !important;
            text-decoration: underline; /* มีเส้นใต้ */
            font-weight: bold; 
            font-size: 18px; 
            border-radius: 50px; 
            transition: all 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275);
            box-shadow: 0 6px 15px rgba(108, 92, 231, 0.3);
            border: 2px solid rgba(255,255,255,0.2);
        }

        .footer-link:hover { 
            transform: translateY(-5px) scale(1.05); /* ลอยขึ้นและขยาย */
            box-shadow: 0 10px 20px rgba(108, 92, 231, 0.4);
            filter: brightness(1.2);
            text-decoration: none; /* เอาเส้นใต้ออกตอนชี้ให้ดูว้าว */
        }
        
        .course-info {
            display: block;
            margin-top: 15px;
            font-size: 12px;
            color: #b2bec3;
            font-style: italic;
        }
    </style>
</head>
<body>

<div class="container">
    <h1>The ESP-32 Update web page without refresh</h1>

    <div class="btn-group">
        <button type="button" class="btn-on" onclick="sendData(11)">LED1 ON</button>
        <button type="button" class="btn-off" onclick="sendData(10)">LED1 OFF</button>
        
        <button type="button" class="btn-on" onclick="sendData(21)">LED2 ON</button>
        <button type="button" class="btn-off" onclick="sendData(20)">LED2 OFF</button>
        
        <button type="button" class="btn-on" onclick="sendData(31)">LED3 ON</button>
        <button type="button" class="btn-off" onclick="sendData(30)">LED3 OFF</button>
        
        <button type="button" class="btn-on" onclick="sendData(41)">LED4 ON</button>
        <button type="button" class="btn-off" onclick="sendData(40)">LED4 OFF</button>
    </div>

    <div class="status-box">
        <div><span class="label">State:</span> [ <span id="LEDState" class="value">NA</span> ]</div>
        <div style="margin-top:12px;"><span class="label">DHT-22:</span> <span id="DHT22_Value" class="value">0</span></div>
    </div>

    <a href="https://www.facebook.com/kit02some/" class="footer-link">
       BY KIT CHANCHUANG
    </a>
<script>
function sendData(led) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("LEDState").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "setLED?LEDstate=" + led, true);
    xhttp.send();
}

setInterval(function() {
    getData();
}, 2000);

function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("DHT22_Value").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "read_DHT22", true);
    xhttp.send();
}
</script>

</body>
</html>
)=====";