#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <util/delay.h>



// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep() {

  ADCSRA |= _BV(ADEN);                 // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_cpu();                         // System actually sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out

  ADCSRA &= ~(_BV(ADEN));              // switch Analog to Digitalconverter ON

}

int main(void){

	// setup PB1 as output (LED)
	DDRB |= (1 << DDB4);

	// Setup PB0 as input interrupt
	PCMSK |= (1 << PCINT0);
	GIMSK |= (1 << PCIE);
	sei();

	uint8_t sleepMode = 0;

	while(1){
		//
		switch (sleepMode) {
			case 0:{
				for (uint8_t i=0; i < 5; i++) {
					PORTB |= (1 << PB4);
					_delay_ms(500);
					PORTB &= ~(1 << PB4);
					_delay_ms(500);
				}

				_delay_ms(5000);

				sleepMode = 2;
				break;
			}

			case 1: {
				// Idle mode enables the MCU to wake up from external triggered interrupts as well as internal ones like the Timer
				// Overflow. If wake-up from the Analog Comparator interrupt is not required, the Analog Comparator can be powered
				// down by setting the ACD bit in “ACSR

				ADCSRA |= _BV(ADEN);

				set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
				sleep_enable();

				sleep_cpu();                        // System actually sleeps here

				sleep_disable();                     // System continues execution here when watchdog timed out

				ADCSRA &= ~(_BV(ADEN));                    // switch Analog to Digitalconverter ON

				sleepMode = 2;
				break;
			}

			case 2: {
				//  Power Down mode, the Oscillator is stopped, while the external interrupts, the USI start condition detection and the Watchdog
				// continue operating (if enabled). Only an External Reset, a Watchdog Reset, a Brown-out Reset, USI start condition
				// interupt, an external level interrupt on INT0 or a pin change interrupt can wake up the MCU. This sleep mode halts
				// all generated clocks, allowing operation of asynchronous modules only.

				system_sleep();
				sleepMode = 0;
				break;
			}
		}
		// TODO Should test
		// - Running Mode
		// - Idle Mode
		// - Power Down
		//
		// Try PRR – Power Reduction Register
		// Disabling ADC, USI, Timer1 and/or Timer0
	}
}



// EXAMPLE EXAMPLE EXAMPLE
/*
Sleep demo for ATtinyX5.                                             *
 * Wire a button from pin D2 (INT0, PB2, DIP pin 7) to ground.          *
 * Wire an LED with an appropriate dropping resistor from pin           *
 * D4 (PB4, DIP pin 3) to ground.                                       *
 * Pushing the button wakes the MCU.                                    *
 * After waking, the MCU flashes the LED, then waits 10 seconds before  *
 * going back to sleep.
 */

/***********************************************
#include <avr/sleep.h>

#define LED 4                      //LED on pin 4, PB4, DIP pin 3
#define KEEP_RUNNING 10000         //milliseconds
#define BODS 7                     //BOD Sleep bit in MCUCR
#define BODSE 2                    //BOD Sleep enable bit in MCUCR

void setup(void)
{
    //to minimize power consumption while sleeping, output pins must not source
    //or sink any current. input pins must have a defined level; a good way to
    //ensure this is to enable the internal pullup resistors.

    for (byte i=0; i<5; i++) {     //make all pins inputs with pullups enabled
        pinMode(i, INPUT);
        digitalWrite(i, HIGH);
    }

    pinMode(LED, OUTPUT);          //make the led pin an output
    digitalWrite(LED, LOW);        //drive it low so it doesn't source current
}

void loop(void)
{
    goToSleep();

    for (byte i=0; i<5; i++) {     //wake up, flash the LED
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }
    delay(KEEP_RUNNING);           //opportunity to measure active supply current
}

void goToSleep(void)
{
    byte adcsra, mcucr1, mcucr2;

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    MCUCR &= ~(_BV(ISC01) | _BV(ISC00));      //INT0 on low level
    GIMSK |= _BV(INT0);                       //enable INT0
    adcsra = ADCSRA;                          //save ADCSRA
    ADCSRA &= ~_BV(ADEN);                     //disable ADC
    cli();                                    //stop interrupts to ensure the BOD timed sequence executes as required
    mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);  //turn off the brown-out detector
    mcucr2 = mcucr1 & ~_BV(BODSE);            //if the MCU does not have BOD disable capability,
    MCUCR = mcucr1;                           //  this code has no effect
    MCUCR = mcucr2;
    sei();                                    //ensure interrupts enabled so we can wake up again
    sleep_cpu();                              //go to sleep
    sleep_disable();                          //wake up here
    ADCSRA = adcsra;                          //restore ADCSRA
}

//external interrupt 0 wakes the MCU
ISR(INT0_vect)
{
    GIMSK = 0;                     //disable external interrupts (only need one to wake up)
}

************************************************/
