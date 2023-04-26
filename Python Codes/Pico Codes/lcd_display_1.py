import utime

import machine
from machine import I2C
from lcd_api import LcdApi
from pico_i2c_lcd import I2cLcd

I2C_ADDR     = 0x27
I2C_NUM_ROWS = 4
I2C_NUM_COLS = 20

i2c = I2C(0, sda=machine.Pin(0), scl=machine.Pin(1), freq=400000)
lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)

def welcome_screen():
    lcd.clear()
    lcd.move_to(5,0)
    lcd.putstr("HOWDY!")

def arrow():
    lcd.clear()
    lcd.move_to(7,1)
    lcd.custom_char(0, bytearray([
        0x04,
        0x0E,
        0x1F,
        0x04,
        0x04,
        0x04,
        0x04,
        0x04]))
    lcd.putchar(chr(0))

while True:
    welcome_screen()
    utime.sleep(2)
    arrow()
    utime.sleep(2)