#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned long milliseconds;

ISR(TIMER1_COMPA_vect) 
{
	milliseconds++;
}

unsigned long millis()
{
	return milliseconds;
}

int main(void){
	unsigned long cpt = 0;
	unsigned long interval = 1000;

	// configure PB0 qnd PB1 as output PWM
	DDRB |= 1<<DDB1 | 1<<DDB0;

	// PWM config
	TCCR0A = 2<<COM0A0 | 2<<COM0B0 | 3<<WGM00;
	TCCR0B = 0<<WGM02 | 1<<CS00;

	TCCR1 = 1<<CTC1 | 7<<CS10;
	OCR1C = 0.001 * F_CPU/64 - 1;

	// enable interrupt
	TIMSK = 1<<OCIE1A;

	sei();

	while(1){
		if (cpt == 0) {
			cpt = millis() + interval;
		}

		if (millis() > cpt) {
			if (OCR0A == 255) {
				OCR0A = 0;
			} else {
				OCR0A = 255;
			}
			cpt = 0;
		}
	}
}
