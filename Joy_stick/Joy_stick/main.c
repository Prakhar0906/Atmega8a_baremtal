/*
 * Joy_stick.c
 *
 * Created: 15-09-2025 19:00:24
 * Author : prakh
 */ 
#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>

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

int ADC_Read(void)
{
	int Ain,AinLow;

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		USART_Transmit(str[j]);
		j++;
		
	}
}

void int_to_str(char* data_str,int data){
	int i=0;
	for(i;data!=0 && i<6;i++){
		int digit = data%10;
		data_str[i] = digit + 48;
		data = data%10;
	}
	data_str[i++]='\n';
	data_str[i] = '\0';
	
}


int main(void)
{
	DDRD |= (1 << DDRD1);
	DDRB |= (1 << DDRB0);
	int data;
	USART_Init(9600);
	char data_str[7];
    /* Replace with your application code */
	ADCSRA |= (1<<ADEN);
    while (1) 
    {
		
		//data = ADC_Read();
		//int_to_str(data_str,data);
		UART_SendString("He");

		_delay_ms(1000);
		
    }
}

