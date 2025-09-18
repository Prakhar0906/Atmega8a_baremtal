/*
 * Push-button.c
 *
 * Created: 12-06-2025 16:19:35
 * Author : prakh
 */ 

#include <avr/io.h>
#include<avr/iom8a.h>


int main(void)
{
	DDRB |= (1<<DDB1);
	DDRB |= (0 << DDB0);
	
	PORTB |= (1 << PORTB1);
	
	
	int pin_status = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		pin_status = PINB & (1<<0);
		if(pin_status){
			PORTB |= (1 << PORTB1);
    }
	else{
		PORTB &= (0 << PORTB1);
	}
}
return 0;
}

