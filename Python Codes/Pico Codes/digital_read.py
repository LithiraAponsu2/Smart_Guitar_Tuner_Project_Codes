from machine import Pin
import time

led = Pin(15, Pin.OUT)
button = Pin(14, Pin.IN, Pin.PULL_DOWN)

while True:
    print(button.value())
    if button.value():
        
        led.toggle()
    time.sleep(0.0005)