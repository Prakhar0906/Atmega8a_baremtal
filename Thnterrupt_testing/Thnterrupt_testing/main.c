#define F_CPU 8000000UL

#include<avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

ISR(INT1_vect){
	
	PORTB |= (1 << PORTB1);
	_delay_ms(1000);
	PORTB &= (0 << PORTB1);
	
}

int main(){
	DDRD &= (0 << DDRD3);
	DDRB |= (1 << DDRB1);
	
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC11);
	
	while(1){
		
	}
}