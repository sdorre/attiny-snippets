#include <avr/io.h>
#include <util/delay.h>

int main(void){
    //declare as output
    DDRB |= 1<<2|1<<3|1<<4;
    /* there are differents way to do that: 
     *  DDRB |= 7<<2;
     *  or ( but this method is  
     *  DDRB = 0b00011100;
    */
    //switch off the leds
    PORTB |= 7<<2;

    while(1){

        //turn on and turn off the leds
        PORTB ^= (7<<2);
        _delay_ms(500);
        PORTB ^= (7<<2);
        _delay_ms(1000);

        //turn on each led separately
        PORTB &= ~(1<<2);
        _delay_ms(500);
        PORTB &= ~(1<<3);
        _delay_ms(500);
        PORTB &= ~(1<<4);
        _delay_ms(1000);

        PORTB |= 1<<2;
        _delay_ms(500);
        PORTB |= 1<<3;
        _delay_ms(500);
        PORTB |= 1<<4;
        _delay_ms(1000);
    }
}
