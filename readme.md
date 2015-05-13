## TWI/I2C added to Brewpi. 

 - Supports ATmego328P (e.g. Uno, nano)
 - Config.h & #define selectable
 - Not reliant on twi.h, which is prone to busy waiting on a less than ideal i2c/twi bus. Busy waiting effectively hangs the MCU - ask me how I know and how low my fridge can go. :)
 - TWI/I2C bus timeouts added - detects if LCD becomes garbled and resets it.
 - I2C-Master modified as to not cause LCD backlight flicker.
 - Currently using a new shield type (forked brewpi-script needed as well). The one wire pin (for mainly sensors) is  hard coded in brewpi to a A4 (SDA) so it has to move. There are better ways to implement this however, ideally without creating a new shield type.

## Modified code from:
 - I2CMaster (twi.h replacement): http://www.dsscircuits.com/articles/86-articles/66-arduino-i2c-master-library
 - https://github.com/slintak/brewpi-avr/blob/feature/IICdisplay/brewpi_avr/IicLcd.cpp

## Default pinout for shield "diy_twi" (also reflected in the forked brewpi-script repo):

Pin | Uno  | BrewPi RevC | DIY_TWI
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
10| SPI SS, PWM |  SPI (LCD)	
11| SPI MOSI, PWM | SPI (LCD)	
12| SPI MISO | SPI (LCD)	
13| SPI SCK, LED | SPI (LCD)	
A0|||			
A1|||			
A2||| OneWire
A3||| Actuator 4
A4| TWI SDA | OneWire | TWI SDA (LCD)
A5| TWI SCL | Actuator 4 | TWI SCL (LCD)
			
		Notes	
		Buzzer requires PWM.	
		TWI is Atmel's name for I2C.
		
