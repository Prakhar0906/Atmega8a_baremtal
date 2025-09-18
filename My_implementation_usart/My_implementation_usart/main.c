/*
 * My_implementation_usart.c
 *
 * Created: 13-09-2025 17:18:13
 * Author : prakh
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#define BAUD 9600
#define UBRR (((F_CPU / (BAUD * 16UL))) - 1)

void USART_Init(unsigned int ubrr){
	UBRRH = (unsigned char) (ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	
	UCSRB = (1 << RXEN)|(1<<TXEN);
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit(char data){
	while(!(UCSRA & (1<<UDRE)));
	
	UDR = data;
}

int main(void)
{

	DDRD |= (1 << DDRD1);
    /* Replace with your application code */
	USART_Init(BAUD);
    while (1) 
    {
		USART_Transmit('h');
		_delay_ms(1000);
		
    }
}

