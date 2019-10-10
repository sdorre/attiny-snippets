#include "i2cmaster.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HDC1080_ADDRESS 0x80

typedef enum {
	HDC1080_TEMPERATURE		= 0x00,
	HDC1080_HUMIDITY		= 0x01,
	HDC1080_CONFIGURATION	= 0x02,
	HDC1080_MANUFACTURER_ID = 0xFE,
	HDC1080_DEVICE_ID		= 0xFF,
	HDC1080_SERIAL_ID_FIRST	= 0xFB,
	HDC1080_SERIAL_ID_MID	= 0xFC,
	HDC1080_SERIAL_ID_LAST	= 0xFD,
} hdc1080_pointer_t;

double readTemperature() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t rawVal;

    i2c_start(HDC1080_ADDRESS+I2C_WRITE);
    i2c_write(HDC1080_TEMPERATURE);
    i2c_stop();

    _delay_ms(15);

    i2c_start_wait(HDC1080_ADDRESS+I2C_READ);
    msb = i2c_readAck();
    lsb = i2c_readNak();
    i2c_stop();

    rawVal = (uint16_t)((msb << 8) | lsb);
    return (rawVal / 65536.0) * 165.0 - 40.0;
}

double readHumidity() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t rawVal;

    i2c_start(HDC1080_ADDRESS+I2C_WRITE);
    i2c_write(HDC1080_HUMIDITY);
    i2c_stop();

    _delay_ms(10);

    i2c_start_wait(HDC1080_ADDRESS+I2C_READ);
    msb = i2c_readAck();
    lsb = i2c_readNak();
    i2c_stop();

    rawVal = (uint16_t)((msb << 8) | lsb);
	return (rawVal / 65536.0) * 100.0;
}

int main (void)
{
    char val[16];
    i2c_init();

    double humidity = readHumidity();
	sprintf(val, "%02d.%02d", (int)humidity, (int)((humidity - (int)humidity) * 100));

    double temperature = readTemperature();
    sprintf(val, "%02d.%02d", (int)temperature, (int)((temperature - (int)temperature) * 100));

    while (1) {
    }

    return 1;
}
