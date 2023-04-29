from machine import I2S
from machine import Pin

# ESP32
sck_pin = Pin(3)   # Serial clock output
ws_pin = Pin(4)    # Word clock output
sd_pin = Pin(2)    # Serial data output

audio_in = I2S(2,
               sck=sck_pin, ws=ws_pin, sd=sd_pin,
               mode=I2S.RX,
               bits=32,
               format=I2S.STEREO,
               rate=22050,
               ibuf=20000)