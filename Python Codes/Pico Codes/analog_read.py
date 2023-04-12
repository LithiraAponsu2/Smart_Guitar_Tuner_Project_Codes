from machine import Pin, ADC
import time

# initialize ADC
adc = ADC(0)

# read analog value every 1 second
while True:
    value = adc.read_u16()
    print("Analog value:", value)
    time.sleep(1)

"""import machine
import utime

sensor_temp = machine.ADC(4)
conversion_factor = 3.3 / (65535)

while True:
    reading = sensor_temp.read_u16() * conversion_factor
    temperature = 27 - (reading - 0.706)/0.001721
    print(temperature)
    utime.sleep(2)"""
