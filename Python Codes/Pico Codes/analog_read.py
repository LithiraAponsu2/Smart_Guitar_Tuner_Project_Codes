from machine import Pin, ADC
import utime

# initialize ADC
adc = ADC(0)
sample_count = 1
analog = []

# read analog value every 1 second
while sample_count < 16385:
    analog.append(adc.read_u16())
    #if (sample_count) % 25 == 0:
    #    print()
    
    #if value > 40000 or value < 30000:
    #    print(value, end=", ")
    
    #else:
    #    print(0, end=", ")
    
    
    #time.sleep(1)
    utime.sleep_us(int(1000000/8000))
    sample_count+=1



for n,i in enumerate(analog):
    print(i, end=",")
    if n % 24 == 0:
        print()
    
        
"""import machine
import utime

sensor_temp = machine.ADC(4)
conversion_factor = 3.3 / (65535)

while True:
    reading = sensor_temp.read_u16() * conversion_factor
    temperature = 27 - (reading - 0.706)/0.001721
    print(temperature)
    utime.sleep(2)"""
