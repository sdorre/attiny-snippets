# Attiny Code Snippets

## Small Projects
### Blink

A Simple LED Blinker example

### PWM

A small firmware that use 3 PWM channels and fades an RGB led.

### Millis

A small setup to implement the millis() function. Often used in Arduino scripts.

### SSD1306

Firmware using a small SSD1306 OLED display. It is running using i2c protocol. The Minimal i2c Library is included it uses Bitbanging on PB3 (SDA) and PB4 (SCL).

### HDC1080

This snippet show how to fetch temperature and humidity data from an I2C temperature sensor. The sensor is a Texas Instrument [HDC1080](http://www.ti.com/lit/ds/symlink/hdc1080.pdf)

### Sleep

Firmware Running a few second and then going to sleep. You can wake it up using a button on pin 5 (PB0). It will alternate between Deep Sleep and Idle state.

Power Measurements :
- Running at 1MHz: 1.23mA (+20ma if the led is plugged)
- Running at 8MHz: 4.35mA (+20ma if the led is plugged)
- Sleep Idle : 814uA
- Deep Sleep : 235uA


## Notes

### Useful includes

There are all the include files provided by avr-gcc. When I installed it on my computer it went to
`/usr/local/Cellar/avr-gcc/9.1.0/avr/include/avr`

When compiling for an Attiny85, the include you want to see are :
- iotn85.h
- iotnx5.h

### Porting programs that use the deprecated sbi/cbi macros

[source](https://www.nongnu.org/avr-libc/user-manual/group__avr__sfr.html)

```
Access to the AVR single bit set and clear instructions are provided via the standard C bit manipulation
commands. The sbi and cbi macros are no longer directly supported.
sbi (sfr,bit) can be replaced by sfr |= _BV(bit) .

i.e.: sbi(PORTB, PB1); is now PORTB |= _BV(PB1);

This actually is more flexible than having sbi directly, as the optimizer will use a hardware sbi if
appropriate, or a read/or/write operation if not appropriate. You do not need to keep track of which
registers sbi/cbi will operate on.

Likewise, cbi (sfr,bit) is now sfr &= ~(_BV(bit));
```
