/*
 * SPI_Slave.c
 *
 * Created: 14-06-2025 18:46:22
 * Author : prakh
 */ 

#include <avr/io.h>
#include<avr/iom8a.h>

char SPI_slave_receive(void){
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

int main(void)
{
	DDRB |= (1<<DDRB4) | (1<<DDRB1);
	
	//SPI SETUP
	SPCR |= (1<<SPIE)|(1<<SPE);
	char data;
    /* Replace with your application code */
    while (1) 
    {
		data = SPI_slave_receive();
		if(data == 0x01){
			PORTB |= (1<< PORTB1);
		}
		else if (data == 0x00){
			PORTB &= (0 << PORTB0);
		}
		
    }
}

