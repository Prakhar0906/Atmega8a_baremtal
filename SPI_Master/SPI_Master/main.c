/*
 * SPI_Master.c
 *
 * Created: 14-06-2025 16:56:53
 * Author : prakh
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include<avr/iom8a.h>
#include<util/delay.h>

void SPI_MasterTransmit(char Data){
	SPDR = Data;
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB1);
	_delay_ms(100);
	PORTB &= (0<< PORTB1);
	
}

char SPI_Master_receive(void){
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	return (SPDR);
}

int main(void)
{
	DDRB |= (1<<DDRB3)|(1<<DDRB5)|(1<<DDRB2)|(1<<DDRB1);
	PORTB |= (1<<PORTB2);
	//SPI SETUP 
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
    /* Replace with your application code */
	char data;
    while (1) 
    {
		data = 0x01;
		PORTB &= (0 << PORTB2);//SS Low 
		SPI_MasterTransmit(data); // Send data
		 // led indication 
		PORTB |= (1<< PORTB2); // SS high
		
		
		data = 0x00; // change data 
		PORTB &= (0<<PORTB2); // SS low 
		SPI_MasterTransmit(data); // send data
		PORTB |= (1<< PORTB2); // SS HIGH
		
		_delay_ms(100); // delay
    }
}

