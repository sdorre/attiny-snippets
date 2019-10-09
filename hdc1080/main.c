#include "i2cmaster.h"
#include "font8x8AJ.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <util/delay.h>

#define SSD1306_SCL   PORTB4  // SCL, Pin 4 on SSD1306 Board - for webbogles board
#define SSD1306_SDA   PORTB3  // SDA, Pin 3 on SSD1306 Board - for webbogles board
#define SSD1306_SA    0x78  // Slave address

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

// Function prototypes - screen control modified from https://bitbucket.org/tinusaur/ssd1306xled
void ssd1306_init(void);
void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(char byte);
void ssd1306_send_command(char command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);
void ssd1306_setpos(char x, char y);
void ssd1306_fillscreen(char fill_Data);
void ssd1306_char_f8x8(char x, char y, const char ch[]);

void drawScreenBorder(void);


void ssd1306_char_f8x8(char x, char y, const char ch[]) {
    char c, i, j = 0;

    while (ch[j] != '\0')
    {
        c = ch[j] - 32; // to space
        if (c > 0) c = c - 12; // to dash
        if (c > 15) c = c - 7;
        if (c > 40) c = c - 6;

        ssd1306_setpos(y, x);
        ssd1306_send_data_start();
        for (i = 0; i < 8; i++) {
                ssd1306_send_byte(pgm_read_byte(&font[c][7 - i]));
        }
        ssd1306_send_data_stop();
        x += 1;
        j++;
    }
}

// Screen control functions
void ssd1306_init(void) {
    ssd1306_send_command(0xAE); // display off
    ssd1306_send_command(0x00); // Set Memory Addressing Mode
    ssd1306_send_command(0x10); // 00,Horizontal Addressing Mode;01,VERTDRAWical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_send_command(0x40); // Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_send_command(0x81); // Set COM Output Scan Direction
    ssd1306_send_command(0xCF); // ---set low column address
    ssd1306_send_command(0xA1); // ---set high column address
    ssd1306_send_command(0xC8); // --set start line address
    ssd1306_send_command(0xA6); // --set contrast control register
    ssd1306_send_command(0xA8);
    ssd1306_send_command(0x3F); // --set segment re-map 0 to 127
    ssd1306_send_command(0xD3); // --set normal display
    ssd1306_send_command(0x00); // --set multiplex ratio(1 to 64)
    ssd1306_send_command(0xD5); //
    ssd1306_send_command(0x80); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_send_command(0xD9); // -set display offset
    ssd1306_send_command(0xF1); // -not offset
    ssd1306_send_command(0xDA); // --set display clock divide ratio/oscillator frequency
    ssd1306_send_command(0x12); // --set divide ratio
    ssd1306_send_command(0xDB); // --set pre-charge period
    ssd1306_send_command(0x40); //
    ssd1306_send_command(0x20); // --set com pins hardware configuration
    ssd1306_send_command(0x02);
    ssd1306_send_command(0x8D); // --set vcomh
    ssd1306_send_command(0x14); // 0x20,0.77xVcc
    ssd1306_send_command(0xA4); // --set DC-DC enable
    ssd1306_send_command(0xA6); //
    ssd1306_send_command(0xAF); // --turn on oled panel
}

void ssd1306_xfer_start(void) {
    i2c_start_wait(SSD1306_SA+I2C_WRITE);
}

void ssd1306_xfer_stop(void) {
    i2c_stop();
}

void ssd1306_send_byte(char byte) {
    i2c_write(byte);
}

void ssd1306_send_command(char command) {
    i2c_start_wait(SSD1306_SA+I2C_WRITE);
    ssd1306_send_byte(0x00); // write command
    ssd1306_send_byte(command);
    ssd1306_xfer_stop();
}

void ssd1306_send_data_start(void) {
    i2c_start_wait(SSD1306_SA+I2C_WRITE);
    ssd1306_send_byte(0x40); //write data
}

void ssd1306_send_data_stop(void) {
    ssd1306_xfer_stop();
}

void ssd1306_setpos(char x, char y)
{
    if (y > 7) return;
    i2c_start_wait(SSD1306_SA+I2C_WRITE);
    ssd1306_send_byte(0x00); //write command

    ssd1306_send_byte(0xb0 + y);
    ssd1306_send_byte(((x & 0xf0) >> 4) | 0x10); // |0x10
    ssd1306_send_byte((x & 0x0f) | 0x01); // |0x01

    ssd1306_xfer_stop();
}

void ssd1306_fillscreen(char fill_Data) {
    char m, n;
    for (m = 0; m < 8; m++)
    {
        ssd1306_send_command(0xb0 + m); //page0-page1
        ssd1306_send_command(0x00); //low column start address
        ssd1306_send_command(0x10); //high column start address
        ssd1306_send_data_start();
        for (n = 0; n < 128; n++)
        {
                ssd1306_send_byte(fill_Data);
        }
        ssd1306_send_data_stop();
    }
}

void drawScreenBorder(void) {
    int8_t c;
    ssd1306_setpos(0, 0);
    ssd1306_send_data_start();
    ssd1306_send_byte(0xFF);
    for (c = 1; c < 126; c++) {
            ssd1306_send_byte(0b00000001);
    }
    ssd1306_send_byte(0xFF);
    ssd1306_send_data_stop();

    for (c = 1; c < 7; c++) {
            ssd1306_setpos(0, c);
            ssd1306_send_data_start();
            ssd1306_send_byte(0xFF);
            ssd1306_send_data_stop();
            ssd1306_setpos(127, c);
            ssd1306_send_data_start();
            ssd1306_send_byte(0xFF);
            ssd1306_send_data_stop();
    }

    ssd1306_setpos(0, 7);
    ssd1306_send_data_start();
    ssd1306_send_byte(0xFF);
    for (c = 1; c < 126; c++) {
            ssd1306_send_byte(0b10000000);
    }
    ssd1306_send_byte(0xFF);
    ssd1306_send_data_stop();
}

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
    i2c_init();                          // init I2C interface

    ssd1306_init();   // initialise the screen

    ssd1306_fillscreen(0x0);

    _delay_ms(500);
    drawScreenBorder();

    ssd1306_char_f8x8(1, 90, "HUMID");

    double humidity = readHumidity();

    sprintf(val, "%02d.%02d", (int)humidity, (int)((humidity - (int)humidity) * 100));
    ssd1306_char_f8x8(1, 80, val);

    ssd1306_char_f8x8(1, 50, "TEMP");
    // ssd1306_char_f8x8(1, 40, "SCORE");

    double temperature = readTemperature();

    sprintf(val, "%02d.%02d", (int)temperature, (int)((temperature - (int)temperature) * 100));
    ssd1306_char_f8x8(1, 40, val);
    while (1) {
    }

    return 1;
}
