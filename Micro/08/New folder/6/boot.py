# ============================================================
#  ESP32 MicroPython — DHT22 + TM1638 (8-digit) + Web Monitor
#  สำหรับ: นาย
#  Hardware: ESP32, DHT22 (GPIO4), TM1638 8-digit LED&Key module
#  TM1638 Pins: STB=GPIO5  CLK=GPIO18  DIO=GPIO23
# ============================================================
#
#  วิธีต่อสาย:
#  ┌──────────────┬─────────────┐
#  │  TM1638 Pin  │  ESP32 Pin  │
#  ├──────────────┼─────────────┤
#  │  VCC         │  5V         │
#  │  GND         │  GND        │
#  │  STB         │  GPIO5      │
#  │  CLK         │  GPIO18     │
#  │  DIO         │  GPIO23     │
#  └──────────────┴─────────────┘
#  ┌──────────────┬─────────────┐
#  │  DHT22 Pin   │  ESP32 Pin  │
#  ├──────────────┼─────────────┤
#  │  VCC (+)     │  3V3        │
#  │  GND (-)     │  GND        │
#  │  DATA (out)  │  GPIO4      │
#  └──────────────┴─────────────┘
#
#  แสดงผลบน 7-Segment 8 หลัก:
#  [_][28][.5][C]  [_][65][.3][H]
#   ←─ Temperature ─→  ←─ Humidity ─→
#
# ============================================================

import machine, time, dht, network, socket

# ─────────────── WiFi Config ──────────────────────────────────
WIFI_SSID     = "Tay2548"
WIFI_PASSWORD = "2548254500"

# ─────────────── Pin Config ───────────────────────────────────
DHT_PIN = 4
TM_STB  = 5
TM_CLK  = 18
TM_DIO  = 23

# ─────────────── Segment Encoding ────────────────────────────
#  Bit:  g f e d c b a  (a=bit0, g=bit6, dp=bit7)
#  7-segment ตัวเลข 0-9
SEG = [
    0x3F,  # 0  → abcdef
    0x06,  # 1  → bc
    0x5B,  # 2  → abdeg
    0x4F,  # 3  → abcdg
    0x66,  # 4  → bcfg
    0x6D,  # 5  → acdfg
    0x7D,  # 6  → acdefg
    0x07,  # 7  → abc
    0x7F,  # 8  → abcdefg
    0x6F,  # 9  → abcdfg
]
SEG_MINUS = 0x40   # g เท่านั้น  ( - )
SEG_C     = 0x39   # a d e f     ( C )
SEG_H     = 0x76   # b c e f g   ( H )
SEG_BLANK = 0x00
SEG_DP    = 0x80   # decimal point (OR กับค่า segment ตัวอื่น)


# ═══════════════════════════════════════════════════════════
#  TM1638 Driver
#  Protocol: 3-wire (STB/CLK/DIO) LSB-first
#  Memory map: address 0..15  (even = segment, odd = LED)
# ═══════════════════════════════════════════════════════════
class TM1638:

    def __init__(self, stb, clk, dio, brightness=4):
        self.stb = machine.Pin(stb, machine.Pin.OUT, value=1)
        self.clk = machine.Pin(clk, machine.Pin.OUT, value=1)
        self.dio = machine.Pin(dio, machine.Pin.OUT, value=1)
        # brightness: 0-7  (0x88 | brightness)
        b = max(0, min(7, brightness))
        self._cmd(0x40)                  # data command: auto-increment, write
        self._cmd(0x88 | b)              # display ON + brightness
        self.clear()

    # ── low-level ──────────────────────────────────────────

    def _cmd(self, cmd):
        """Send a single-byte command (STB low → byte → STB high)"""
        self.stb.value(0)
        self._write_byte(cmd)
        self.stb.value(1)

    def _write_byte(self, data):
        """Write one byte LSB-first on the CLK/DIO bus"""
        for _ in range(8):
            self.clk.value(0)
            self.dio.value(data & 1)
            data >>= 1
            self.clk.value(1)

    def _write_data(self, addr, buf):
        """
        TM1638 protocol — ต้องแยกเป็น 2 STB transaction:
          TX1: STB=0 → 0x40 (data cmd) → STB=1
          TX2: STB=0 → 0xC0|addr → data bytes → STB=1
        ถ้าใช้ self._cmd(0x40) แล้วต่อด้วย STB=0 ทันที
        จะทำให้ TM1638 ไม่รับ address ใหม่ → เลขค้าง
        """
        # TX1: data command (auto-increment, write)
        self.stb.value(0)
        self._write_byte(0x40)
        self.stb.value(1)
        # TX2: start address + data burst
        self.stb.value(0)
        self._write_byte(0xC0 | addr)
        for b in buf:
            self._write_byte(b)
        self.stb.value(1)

    # ── public API ─────────────────────────────────────────

    def clear(self):
        """ล้างหน้าจอทั้งหมด (segment + LED)"""
        self._write_data(0, [0x00] * 16)

    def set_brightness(self, level):
        """0 = min, 7 = max"""
        self._cmd(0x88 | max(0, min(7, level)))

    def show_temp_hum(self, temp, hum):
        """
        แสดงอุณหภูมิ (digit 0-3) และความชื้น (digit 4-7)

        ตัวอย่าง temp=28.5, hum=65.3:
          digit: [_][2][8.][C]  [_][6][5.][H]
                  0  1  2   3    4  5  6   7

        TM1638 memory layout (auto-increment from addr 0):
          addr 0 → seg[0] (digit 0)
          addr 1 → LED[0]  = 0x00
          addr 2 → seg[1] (digit 1)
          addr 3 → LED[1]  = 0x00
          ...
        ทั้งหมด 16 bytes สำหรับ 8 digits + 8 LEDs
        """

        # ── อุณหภูมิ ──────────────────────────────────────
        t_raw = round(abs(temp) * 10)   # 28.5 → 285, 9.5 → 95
        t_neg = temp < 0

        t_hundreds = t_raw // 100       # 2  (หรือ 0 ถ้า < 10°C)
        t_tens     = (t_raw // 10) % 10 # 8
        t_ones     = t_raw % 10         # 5

        # digit 0: ขีดลบ หรือ หลักร้อย/สิบ หรือ ว่าง
        if t_neg:
            d0 = SEG_MINUS
        elif t_hundreds > 0:
            d0 = SEG[t_hundreds]
        else:
            d0 = SEG_BLANK

        # จุดทศนิยมอยู่ที่ digit 1 (หลักหน่วย) → แสดงเป็น "28." ก่อน C
        d1 = SEG[t_tens] | SEG_DP      # digit 1: หลักสิบ + จุด  เช่น "8."
        d2 = SEG[t_ones]               # digit 2: หลักทศนิยม     เช่น "5"
        d3 = SEG_C                     # digit 3: C

        # ── ความชื้น ──────────────────────────────────────
        h_raw = round(abs(hum) * 10)   # 65.3 → 653
        h_hundreds = h_raw // 100
        h_tens     = (h_raw // 10) % 10
        h_ones     = h_raw % 10

        d4 = SEG[h_hundreds] if h_hundreds > 0 else SEG_BLANK
        d5 = SEG[h_tens] | SEG_DP     # digit 5: หลักสิบ + จุด  เช่น "5."
        d6 = SEG[h_ones]              # digit 6: หลักทศนิยม     เช่น "3"
        d7 = SEG_H

        # ── เขียนลง TM1638 ─────────────────────────────────
        # สลับ segment / LED ทุก address (16 bytes รวม)
        buf = [
            d0, 0,  # addr 0,1  (digit0, LED0)
            d1, 0,  # addr 2,3
            d2, 0,  # addr 4,5
            d3, 0,  # addr 6,7
            d4, 0,  # addr 8,9
            d5, 0,  # addr 10,11
            d6, 0,  # addr 12,13
            d7, 0,  # addr 14,15
        ]
        self._write_data(0, buf)


# ─────────────── Init Hardware ────────────────────────────────
display = TM1638(stb=TM_STB, clk=TM_CLK, dio=TM_DIO, brightness=4)
sensor  = dht.DHT22(machine.Pin(DHT_PIN))

# ─────────────── Global State ─────────────────────────────────
temperature = 0.0
humidity    = 0.0


# ─────────────── Read DHT22 ───────────────────────────────────
def read_dht():
    global temperature, humidity
    try:
        sensor.measure()
        temperature = sensor.temperature()
        humidity    = sensor.humidity()
        print(f"Temp={temperature:.1f}C  Hum={humidity:.1f}%")
        display.show_temp_hum(temperature, humidity)
    except OSError as e:
        print("DHT22 error:", e)


# ─────────────── WiFi ─────────────────────────────────────────
def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    # Reset WiFi state ก่อนเสมอ เพื่อล้าง internal state เก่า
    wlan.active(False)
    time.sleep(0.5)
    wlan.active(True)
    time.sleep(0.5)
    print(f"Connecting to '{WIFI_SSID}' ", end="")
    wlan.connect(WIFI_SSID, WIFI_PASSWORD)
    for _ in range(20):
        if wlan.isconnected():
            break
        time.sleep(1)
        print(".", end="")
    print()
    if wlan.isconnected():
        ip = wlan.ifconfig()[0]
        print(f"Connected!  http://{ip}/")
        return ip
    print("WiFi failed — offline mode")
    return None


# ─────────────── HTML (static — JS fetch real-time) ──────────
HTML = """<!DOCTYPE html>
<html lang="th">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32 Monitor (TM1638)</title>
<style>
*,*::before,*::after{box-sizing:border-box;margin:0;padding:0}
body{
  font-family:'Segoe UI',sans-serif;
  background:#0d1117;color:#c9d1d9;
  min-height:100vh;display:flex;flex-direction:column;
  align-items:center;justify-content:center;padding:24px;gap:18px;
}
h1{font-size:1.25rem;color:#58a6ff;text-align:center}
.badge{
  background:#161b22;border:1px solid #30363d;
  border-radius:20px;padding:3px 14px;
  font-size:0.72rem;color:#8b949e;letter-spacing:.04em;
}
.grid{
  display:grid;grid-template-columns:1fr 1fr;
  gap:14px;width:100%;max-width:400px;
}
.card{
  background:#161b22;border:1px solid #30363d;
  border-radius:14px;padding:24px 12px 18px;text-align:center;
}
.icon{font-size:2rem;margin-bottom:8px}
.val{
  font-size:2.8rem;font-weight:700;line-height:1;
  transition:opacity .15s;
}
.val.fade{opacity:.25}
.unit{font-size:1rem;font-weight:400;opacity:.65}
.lbl{font-size:0.78rem;color:#8b949e;margin-top:6px}
.temp .val{color:#ff7b72}
.hum  .val{color:#79c0ff}
.seg-box{
  background:#111;border:1px solid #238636;
  border-radius:10px;padding:14px 18px;
  width:100%;max-width:400px;text-align:center;
}
.seg-label{font-size:0.72rem;color:#3fb950;letter-spacing:.08em;margin-bottom:6px}
.seg-digits{
  font-family:'Courier New',monospace;font-size:1.5rem;font-weight:700;
  color:#3fb950;letter-spacing:.2em;text-shadow:0 0 8px #3fb95066;
  transition:opacity .15s;
}
.seg-digits.fade{opacity:.25}
.status{font-size:.75rem;color:#555;display:flex;align-items:center;gap:6px}
.dot{width:7px;height:7px;border-radius:50%;background:#3fb950}
.dot.live{animation:blink 1.5s ease-in-out infinite}
.dot.err{background:#ff7b72;animation:none}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.15}}
</style>
</head>
<body>
<h1>&#127777; ESP32 Weather Monitor</h1>
<span class="badge">TM1638 &mdash; Real-time</span>
<div class="grid">
  <div class="card temp">
    <div class="icon">&#127777;</div>
    <div class="val" id="temp">--<span class="unit">&#176;C</span></div>
    <div class="lbl">&#xE2D;&#xE38;&#xE13;&#xE2B;&#xE20;&#xE39;&#xE21;&#xE34;</div>
  </div>
  <div class="card hum">
    <div class="icon">&#128167;</div>
    <div class="val" id="hum">--<span class="unit">%</span></div>
    <div class="lbl">&#xE04;&#xE27;&#xE32;&#xE21;&#xE0A;&#xE37;&#xE49;&#xE19;</div>
  </div>
</div>
<div class="seg-box">
  <div class="seg-label">TM1638 OUTPUT</div>
  <div class="seg-digits" id="seg">--------</div>
</div>
<div class="status">
  <span class="dot" id="dot"></span>
  <span id="status">กำลังเชื่อมต่อ...</span>
</div>
<script>
function segText(t,h){
  var s=t<0?"-":" ",a=Math.abs(t),i=Math.floor(a);
  var ts=s+Math.floor(i/10)+(i%10)+"."+Math.round(a*10)%10+"C";
  var b=Math.abs(h),j=Math.floor(b);
  var hs=" "+Math.floor(j/10)+(j%10)+"."+Math.round(b*10)%10+"H";
  return ts+"  "+hs;
}
function flash(id){
  var e=document.getElementById(id);
  e.classList.add("fade");
  return e;
}
function update(){
  fetch("/data")
    .then(function(r){return r.json();})
    .then(function(d){
      var te=flash("temp"), he=flash("hum"), se=flash("seg");
      setTimeout(function(){
        te.innerHTML=d.temp.toFixed(1)+'<span class="unit">&#176;C</span>';
        he.innerHTML=d.hum.toFixed(1)+'<span class="unit">%</span>';
        se.textContent=segText(d.temp,d.hum);
        te.classList.remove("fade");
        he.classList.remove("fade");
        se.classList.remove("fade");
        document.getElementById("dot").className="dot live";
        document.getElementById("status").textContent=
          "Live — "+new Date().toLocaleTimeString("th-TH");
      },150);
    })
    .catch(function(){
      document.getElementById("dot").className="dot err";
      document.getElementById("status").textContent="เชื่อมต่อไม่ได้ — กำลังลองใหม่...";
    });
}
update();
setInterval(update, 2000);
</script>
</body>
</html>"""


def make_json():
    return '{{"temp":{:.1f},"hum":{:.1f}}}'.format(temperature, humidity)


# ─────────────── Web Server ───────────────────────────────────
def start_server(ip):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((ip, 80))
    s.listen(3)
    s.settimeout(0.05)   # non-blocking: หมดเวลาก็วน loop ต่อได้
    return s


def handle_request(srv):
    try:
        conn, _ = srv.accept()
        req = conn.recv(512)
        if b"GET /data" in req:
            # JSON endpoint สำหรับ real-time fetch
            body = make_json().encode("utf-8")
            header = (
                b"HTTP/1.1 200 OK\r\n"
                b"Content-Type: application/json\r\n"
                b"Access-Control-Allow-Origin: *\r\n"
                b"Connection: close\r\n\r\n"
            )
            conn.sendall(header + body)
        elif b"GET /" in req:
            # หน้า HTML หลัก (โหลดครั้งเดียว JS จัดการต่อ)
            body = HTML.encode("utf-8")
            header = (
                b"HTTP/1.1 200 OK\r\n"
                b"Content-Type: text/html; charset=utf-8\r\n"
                b"Connection: close\r\n\r\n"
            )
            conn.sendall(header + body)
        conn.close()
    except OSError:
        pass   # timeout → ไม่มี client ก็ผ่านไป


# ─────────────── Main ─────────────────────────────────────────
def main():
    ip  = connect_wifi()
    srv = start_server(ip) if ip else None

    last_read = -999   # อ่านทันทีรอบแรก

    while True:
        now = time.time()

        # อ่าน sensor ทุก 3 วินาที
        if now - last_read >= 3:
            read_dht()
            last_read = now

        # รับ HTTP request (non-blocking)
        if srv:
            handle_request(srv)

main()
