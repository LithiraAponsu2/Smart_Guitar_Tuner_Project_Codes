from machine import Pin, ADC
import time

# initialize ADC
adc = ADC(Pin(26))

# read analog value every 1 second
while True:
    value = adc.read_u16()
    print("Analog value:", value)
    time.sleep(0.01)
