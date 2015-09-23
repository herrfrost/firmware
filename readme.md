## TWI/I2C added to Brewpi. 
 - Latest available code base from Brewpi at this time
 - Supports ATmego328P (e.g. Uno, nano)
 - Config.h & #define selectable
 - Not reliant on twi.h & wiring, which are prone to busy waiting on a less than ideal i2c/twi bus. Busy waiting effectively hangs the MCU - ask me how I know and how low my fridge can go. :)
  
- Using the same pinout as the revC shield is possible, thus preserving the hard-coded onewire pin occupying one of the pins required for hardware i2c/twi.

## Includes code from:
 - Peter Fleury's assembler code http://homepage.hispeed.ch/peterfleury/avr-software.html adapted by feliasfogg (https://github.com/felias-fogg/SoftI2CMaster)
 - https://github.com/slintak/brewpi-avr/blob/feature/IICdisplay/brewpi_avr/IicLcd.cpp

## To use
- Copy and modify /app/fallback/Config.h to /app/controller/Config.h
- Modify accordingly
- Build project in Atmel Studo v6.2
- makefile _not_ updated
- Upload hex file in Brewpi web UI
- Use revC style pinout - except that pin 10 and 11 are SDA and SCL (i.e. I2C/TWI) respectively.
- You **must** use external pullups on both SDA and SCL - around 2 kΩ.


## Notes
Not related to I2C bus implementation, however it might be interesting for DIYers; many Arduino Nano clones from China are using the WCH 340 chip. The linux driver for that has a bug causing pyserial to reopen the port at an incorrect baud rate. It will manifest as serial line gibbersh after the serial port reopens, e.g. just before programming and when restarting the brewpi script. 

A hack to work around it is to make the following modifications to /home/brewpi/BrewPiUtil.py:
```
def setupSerial(config, baud_rate=57600, time_out=0.1):
    """
    HACK: replaced method due to ch341 driver bug used for some Arduino Nano clones
    http://www.spinics.net/lists/linux-usb/msg121599.html
    """
    setupSerialReal(config, baud_rate=9600, time_out=0.1).close()
    return setupSerialReal(config, baud_rate, time_out)

def setupSerialReal(config, baud_rate=57600, time_out=0.1):
[...]
```

## Pins
Most can be changed in code and in the web UI, however, and apart from occupying the hardware I2C_SDA pin, the onewire pin is hard-coded in multiple places. The only way to reasonably change it is to add another shield type. That adds needless complexity without addressing the underlying problem; hence a software I2C implementation.

Pin | Uno  | BrewPi RevC | BrewPi RevC I2C
--- | ----- | -------- | -----
0|Serial RX | Serial | Serial
1|Serial TX | Serial| Serial
2|INT0 | Actuator 3|Actuator 3
3|INT1, PWM|Beep|Beep
4||  Door sensor| Door sensor
5|PWM | Actuator 2|Actuator 2
6|PWM | Actuator 1  |Actuator 1
7|| Rotary|Rotary
8||   Rotary|Rotary
9| PWM | Rotary | Rotary
10| SPI SS, PWM |SPI (LCD)| ASM_SDA (LCD)	
11| SPI MOSI, PWM |SPI (LCD)| ASM_SCL (LCD)	
12| SPI MISO | SPI (LCD)|
13| SPI SCK, LED | SPI (LCD)|
A0|||			
A1|||			
A2||| 
A3||| 
A4| TWI SDA | OneWire | OneWire
A5| TWI SCL | Actuator 4 | Actuator 4
			
		Notes	
		Buzzer requires PWM.	
		TWI is Atmel's name for I2C.
		
