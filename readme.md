## TWI/I2C added to Brewpi. 
 - Latest available code base from Brewpi at this time
 - Supports ATmego328P (e.g. Uno, nano)
 - Config.h & #define selectable
 - Not reliant on twi.h, which is prone to busy waiting on a less than ideal i2c/twi bus. Busy waiting effectively hangs the MCU - ask me how I know and how low my fridge can go. :)
  
- Using the same pinout as the revC shield is possible, thus preserving the hard-coded onewire pin occupying one of the pins required for hardware i2c/twi.

## Includes code from:
 - Peter Fleury's assembler code http://homepage.hispeed.ch/peterfleury/avr-software.html adapted by feliasfogg (https://github.com/felias-fogg/SoftI2CMaster)
 - https://github.com/slintak/brewpi-avr/blob/feature/IICdisplay/brewpi_avr/IicLcd.cpp

## To use
- Copy and modify /app/controller/Config.h to /app/controller/Config.h
- Modify accordingly
- Build project in Atmel Studo v6.2
- Upload hex file in Brewpi web UI
- Use revC style pinout - except that pin 10 and 11 are SDA and SCL (i.e. I2C/TWI) respectively.


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
		
