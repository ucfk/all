# ESP32 MicroPython
# 4-Digit 7-Segment + Interrupt
# เริ่มต้นแสดงเลข 4499
# GPIO26 กด +2
# GPIO25 กด -1
# Common Cathode

from machine import Pin
from time import sleep_us, ticks_ms

# ========= ค่าเริ่มต้น =========
counter = 3216

# Segment pins a,b,c,d,e,f,g
segPins = [
    Pin(17, Pin.OUT),
    Pin(5, Pin.OUT),
    Pin(18, Pin.OUT),
    Pin(19, Pin.OUT),
    Pin(21, Pin.OUT),
    Pin(22, Pin.OUT),
    Pin(23, Pin.OUT)
]

# Digit pins หลักพัน หลักร้อย หลักสิบ หลักหน่วย
digitPins = [
    Pin(13, Pin.OUT),
    Pin(12, Pin.OUT),
    Pin(14, Pin.OUT),
    Pin(27, Pin.OUT)
]

# ตัวเลข 0-9
digits = [
    0b0111111, #0
    0b0000110, #1
    0b1011011, #2
    0b1001111, #3
    0b1100110, #4
    0b1101101, #5
    0b1111101, #6
    0b0000111, #7
    0b1111111, #8
    0b1101111  #9
]

# Interrupt buttons
btn_add = Pin(26, Pin.IN, Pin.PULL_UP)
btn_sub = Pin(25, Pin.IN, Pin.PULL_UP)

last1 = 0
last2 = 0
debounce = 200


# ========= Interrupt =========
def isr1(pin):
    global counter, last1
    now = ticks_ms()
    if now - last1 > debounce:
        counter += 2
        last1 = now

def isr2(pin):
    global counter, last2
    now = ticks_ms()
    if now - last2 > debounce:
        counter -= 1
        last2 = now


btn_add.irq(trigger=Pin.IRQ_FALLING, handler=isr1)
btn_sub.irq(trigger=Pin.IRQ_FALLING, handler=isr2)


# ========= Display =========
def showDigit(pos, num):

    # ปิดทุกหลักก่อน
    for d in digitPins:
        d.value(1)   # CC ปิด = HIGH

    seg = digits[num]

    # ส่งค่า segment
    for i in range(7):
        segPins[i].value((seg >> i) & 1)

    # เปิดหลักที่ต้องการ
    digitPins[pos].value(0)   # CC เปิด = LOW

    sleep_us(3000)


def displayNumber(num):

    if num < 0:
        num = (10000 + num % 10000) % 10000
    if num > 9999:
        num = num % 10000

    d0 = (num // 1000) % 10
    d1 = (num // 100) % 10
    d2 = (num // 10) % 10
    d3 = num % 10

    showDigit(0, d0)
    showDigit(1, d1)
    showDigit(2, d2)
    showDigit(3, d3)


# ========= Main Loop =========
while True:
    displayNumber(counter)