from RPLCD.i2c import CharLCD
import gpsd2
import time

lcd = CharLCD(i2c_expander='PCF8574', address=0x27, port=1, cols=20, rows=4, dotsize=8)
lcd.clear() # Clear the display :)

gpsd2.connect() # Connect to the local gpsd
device = gpsd2.device()

lcd.write_string("Welcome to RoombaPI\n\r")
lcd.write_string("Created by MTK")

time.sleep(3)

lcd.clear()
lcd.write_string("GPS Device:\n\r")

path_str = f"Path: {device['path']}\n\r"
lcd.write_string(path_str)

speed_str = f"Speed: {device['speed']}\n\r"
lcd.write_string(speed_str)

driver_str = f"Driver: {device['driver']}\n\r"
lcd.write_string(driver_str)

#if __name__ == "__main__":
    