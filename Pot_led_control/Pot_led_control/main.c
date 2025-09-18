/*
 * Pot_led_control.c
 *
 * Created: 14-06-2025 10:37:57
 * Author : prakh
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include<avr/iom8a.h>
#include<util/delay.h>



int main(void)
{
	DDRB |= (1<<PORTB1);

	unsigned char duty;
	int adc_low,adc_val;
	
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0)|(1 << WGM10);
	TCCR1B |= (1<<WGM12) | (1<< CS12);
	ADCSRA |= (1<<ADEN)|(1<<ADIE);
    /* Replace with your application code */
    while (1) 
    {
		ADCSRA|= (1<<ADSC);
		while(ADCSRA&(1<<ADIF));
		adc_low = (int)ADCL;
		adc_val = adc_low+(ADCH*256);
		duty = (int)adc_val*(255.0/1023.0);
		OCR1A = duty;
		_delay_ms(8);
		
    }
}

