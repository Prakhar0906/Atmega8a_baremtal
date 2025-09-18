/*
 * the_pwm_signal.c
 *
 * Created: 13-06-2025 17:51:44
 * Author : prakh
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include<avr/iom8a.h>
#include<util/delay.h>


int main(void)
{
	DDRB |= (1<<PORTB1);
	
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0)|(1<<WGM10);
	TCCR1B |= (1<<WGM12)|(1<<CS12);
    /* Replace with your application code */
	unsigned char duty;
    while (1) 
    {
		for(duty=0; duty<255; duty++)
		{
			OCR1A=duty;  /*increase the LED light intensity*/
			_delay_ms(8);
		}
		for(duty=255; duty>1; duty--)
		{
			OCR1A=duty;  /*decrease the LED light intensity*/
			_delay_ms(8);
		}
    }
}

