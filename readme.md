## TWI/I2C added to Brewpi. 
 - Latest available code base from Brewpi at this time
 - Supports ATmego328P (e.g. Uno, nano)
 - Config.h & #define selectable
 - Not reliant on twi.h, which is prone to busy waiting on a less than ideal i2c/twi bus. Busy waiting effectively hangs the MCU - ask me how I know and how low my fridge can go. :)
  - Using the same pinout as the revC shield is possible, thus preserving the hard-coded onewire pin occupying one of the pins required for hardware i2c/twi.

## Modified code from:
 - Peter Fleury's assembler code http://homepage.hispeed.ch/peterfleury/avr-software.html adapted by feliasfogg (https://github.com/felias-fogg/SoftI2CMaster)
 - https://github.com/slintak/brewpi-avr/blob/feature/IICdisplay/brewpi_avr/IicLcd.cpp

## To use
- Copy and modify /app/controller/Config.h to /app/controller/Config.h
- Modify accordingly
- Build project in Atmel Studo (6.2(
- Upload hex file in Brewpi web UI
- Presently: I use non-revC pinout, so take note of Pins.h
