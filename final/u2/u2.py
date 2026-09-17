import machine
import network
import socket
import time

# ==========================================
# 1. คลาสควบคุม TM1638 (เพิ่มฟังก์ชัน 7-Segment)
# ==========================================
class TM1638:
    def __init__(self, stb_pin, clk_pin, dio_pin):
        self.stb = machine.Pin(stb_pin, machine.Pin.OUT)
        self.clk = machine.Pin(clk_pin, machine.Pin.OUT)
        self.dio = machine.Pin(dio_pin, machine.Pin.OUT)
        self.stb.value(1)
        self.clk.value(1)
        self.send_command(0x8f) # เปิดจอ ความสว่างสูงสุด
        self.clear()

    def send_byte(self, data):
        for i in range(8):
            self.clk.value(0)
            self.dio.value((data >> i) & 1)
            self.clk.value(1)

    def send_command(self, cmd):
        self.stb.value(0)
        self.send_byte(cmd)
        self.stb.value(1)

    def set_digit(self, pos, data):
        # ตำแหน่งของ 7-segment (0 ถึง 7) Address คือ 0xC0, 0xC2, 0xC4...
        self.send_command(0x44) # โหมด Fixed address
        self.stb.value(0)
        self.send_byte(0xC0 + (pos * 2))
        self.send_byte(data)
        self.stb.value(1)

    def show_l1(self, state):
        # แสดง L1HI (1) หรือ L1LO (0) ในช่อง index 0-3
        # L = 0x38, 1 = 0x06, H = 0x76, I = 0x06, O = 0x3F
        data = [0x38, 0x06, 0x76, 0x06] if state == 1 else [0x38, 0x06, 0x38, 0x3F]
        for i in range(4):
            self.set_digit(i, data[i])

    def show_l2(self, state):
        # แสดง L2HI (1) หรือ L2LO (0) ในช่อง index 4-7
        # L = 0x38, 2 = 0x5B, H = 0x76, I = 0x06, O = 0x3F
        data = [0x38, 0x5B, 0x76, 0x06] if state == 1 else [0x38, 0x5B, 0x38, 0x3F]
        for i in range(4):
            self.set_digit(i + 4, data[i])

    def clear(self):
        self.send_command(0x40)
        self.stb.value(0)
        self.send_byte(0xC0)
        for _ in range(16):
            self.send_byte(0x00)
        self.stb.value(1)

# ==========================================
# 2. ตั้งค่า Hardware
# ==========================================
# ขาสำหรับ TM1638
tm = TM1638(stb_pin=4, clk_pin=5, dio_pin=18)

# ขาสำหรับ LED ภายนอก (Pin 21, Pin 22)
ext_led1 = machine.Pin(21, machine.Pin.OUT)
ext_led2 = machine.Pin(22, machine.Pin.OUT)

# ==========================================
# 3. ตั้งค่า Wi-Fi
# ==========================================
ssid = 'YOUR_WIFI_SSID'
password = 'YOUR_WIFI_PASSWORD'

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(ssid, password)

print('Connecting to Wi-Fi...')
while not station.isconnected():
    time.sleep(0.5)

print('IP Address:', station.ifconfig()[0])

# ==========================================
# 4. ฟังก์ชันจัดการสถานะ (ค่าเริ่มต้นคือ ปิด)
# ==========================================
led1_state = 0
led2_state = 0

def update_hardware():
    ext_led1.value(led1_state)
    ext_led2.value(led2_state)
    tm.show_l1(led1_state)
    tm.show_l2(led2_state)

# อัปเดต Hardware ครั้งแรกเมื่อบอร์ดพร้อม
update_hardware()

# ==========================================
# 5. ฟังก์ชันหน้าเว็บ (HTML UI แยกปุ่ม ON / OFF)
# ==========================================
def web_page():
    html = """<html>
    <head>
        <title>ESP32 LED Control</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
            body { font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f4;}
            h1 { color: #333; }
            .button { 
                display: inline-block; padding: 15px 30px; font-size: 20px; 
                cursor: pointer; text-decoration: none; color: white; 
                border-radius: 8px; margin: 10px; width: 80px;
            }
            .on-btn { background-color: #4CAF50; } /* สีเขียว */
            .off-btn { background-color: #f44336; } /* สีแดง */
            .status { font-size: 24px; font-weight: bold; margin-bottom: 20px;}
        </style>
    </head>
    <body>
        <h1>ESP32 & TM1638 Control</h1>
        
        <div class="status">LED 1 Status: """ + ("ON" if led1_state else "OFF") + """</div>
        <p>
            <a href="/?led1=on" class="button on-btn">ON</a>
            <a href="/?led1=off" class="button off-btn">OFF</a>
        </p>

        <hr style="width: 80%; border-top: 1px solid #ccc;">

        <div class="status">LED 2 Status: """ + ("ON" if led2_state else "OFF") + """</div>
        <p>
            <a href="/?led2=on" class="button on-btn">ON</a>
            <a href="/?led2=off" class="button off-btn">OFF</a>
        </p>
    </body>
    </html>"""
    return html

# ==========================================
# 6. เริ่มต้น Web Server
# ==========================================
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(('', 80))
s.listen(5)
print('Web server started!')

while True:
    try:
        conn, addr = s.accept()
        request = conn.recv(1024)
        request = str(request)
        
        # ตรวจสอบ Request เพื่อเปิด-ปิดแบบเฉพาะเจาะจง
        if '/?led1=on' in request:
            led1_state = 1
        elif '/?led1=off' in request:
            led1_state = 0
            
        if '/?led2=on' in request:
            led2_state = 1
        elif '/?led2=off' in request:
            led2_state = 0

        # สั่งให้ Hardware อัปเดตตามสถานะใหม่
        update_hardware()

        # ตอบกลับหน้าเว็บ
        response = web_page()
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(response)
        conn.close()
        
    except OSError:
        conn.close()