#define F_CPU 4000000UL
#include<util/delay.h>
#include<avr/io.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)



void USART_Init(long USART_BAUDRATE){
	UBRRL = BAUD_PRESCALE;			/* Load lower 8-bits of the baud rate */
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB |= (1 << RXEN) | (1 << TXEN);	/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit char size */
	
}

void USART_Flush(void){
	unsigned char dummy;
	while(UCSRA & (1 << RXC)) dummy = UDR;
}

unsigned char USART_Receive(void){
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);
}

void USART_Transmit(char data){
	while(!(UCSRA & (1 << UDRE)));
	UDR = data;
}

int main(){
	unsigned char data;
	DDRD |= (1 << DDRD1);
	DDRD &= (0 << DDRD0);
	DDRB |= (1 << DDRB0);
	USART_Init(9600);
	PORTB |= (1 << PORTB0);
	
	while(1){
		while(!(data = USART_Receive()));
		USART_Transmit(data);
		//PORTB ^= (1 << PORTB0);
		if(data == '1'){
			PORTB ^= (1 << PORTB0);
		}
		
		_delay_ms(10);
	}
}