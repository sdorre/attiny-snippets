#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t* Port[] = {&OCR0A, &OCR0B, &OCR1B};

void SetColor(int color, int intensity)
{
	*Port[color] = 255 - intensity;
}

int main(void){
	int color;
	int i;

	//declare as output
	DDRB |= 1<<0|1<<1|1<<4;
	//switch off the leds
	PORTB |= 1<<0|1<<1|1<<4;

	TCCR0A = 3<<COM0A0 | 3<<COM0B0 | 3<< WGM00;
	TCCR0B = 0<<WGM02 | 3<<CS00;

	GTCCR = 1<<PWM1B | 3<<COM1B0;
	TCCR1 = 3<<COM1A0 | 7<<CS10;

	while(1){
		for(color=0; color <3; color++){
			SetColor((color+2) % 3, 0);
			
			for(i=0; i<=255; i++){
				SetColor((color+1)%3, i);
				SetColor(color, 255-i);
				_delay_ms(25);
			}
		}
	}
}
