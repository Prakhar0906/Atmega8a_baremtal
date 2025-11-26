/*
 * Pot_led_control.c
 *
 * Created: 14-06-2025 10:37:57
 * Author : prakh
 */ 
#define F_CPU 4000000UL

#include <avr/io.h>
#include<avr/iom8a.h>
#include<util/delay.h>
#include <string.h>

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

void USART_Send_str(char* str){
	for(int i=0;str[i] != '\0';i++){
		USART_Transmit(str[i]);
		_delay_ms(2);
	}
}

void USART_Receive_str(char* str){
	char c;
	int i=0;
	while((c = USART_Receive()) != '\n'){
		str[i++]= c;
	}
}

void save_str(char* s){
	char c;
	int i=0;
	while((c = USART_Receive()) != '\r'){
		s[i++] = c;
	}
	s[i] = '\0';
}

void int_to_str(int N, char *str){
	int i=0;
	
	while(N > 0){
		str[i++] = N%10 + 48;
		N/=10;
	}
	str[i] = '\0';
	
	for(int j = 0, k = i - 1; j < k; j++, k--){
		char temp = str[j];
		str[j] = str[k];
		str[k] = temp;
	}
	
}



int main(void)
{
	DDRB |= (1 << DDRB1);	
	DDRD |= (1 << DDRD7)|(1<<DDRD1);
	
	USART_Init(9600);


	unsigned int t1;
	unsigned int t2;
	
	
	//TCCR1A |= (1<<COM1A1)|(1<<COM1A0)|(1 << WGM10);
	//TCCR1B |= (1<<WGM12);
	PORTD &= (0 << PORTD7);
	char str[10];
    while (1) 
    {
		TCCR1A = 0;
		TIFR |=(1<<ICF1);
		TCCR1B = 0x41;
		PORTD |= (1 << PORTD7);
		_delay_us(10);
		PORTD &= (0 << PORTD7);
		while((TIFR&(1<<ICF1))==0);
		t1 = ICR1;
		TIFR |= (1 <<ICF1);
		
		TCCR1B = 0x01;
		while((TIFR&(1<<ICF1))==0);
		t2 = ICR1;
		
		double time_dur = (t2-t1)/4;	
		time_dur =255 - time_dur * (255.0/2800);
		if(time_dur <= 0)
			time_dur = 1;
		/*
		int_to_str((int)time_dur,str);
		strcat(str,"\r\n");
		USART_Send_str(str);
		*/
		
		TCCR1A |= (1<<COM1A1)|(1<<COM1A0)|(1 << WGM10);
		TCCR1B |= (1<<WGM12);
		OCR1A = time_dur;
	
		
		_delay_ms(100);
		
    }
}

