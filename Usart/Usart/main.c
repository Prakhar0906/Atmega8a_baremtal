#define F_CPU 4000000UL
#include<util/delay.h>
#include<avr/io.h>
#include<string.h>
#include<usart_func.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


int main(){
	WDTCR &= (0 << WDE);
	DDRD |= (1 << PORTD1);
	DDRC |= (1 << DDRC5);
	int value;
	
	char str[10] = "Hello";
	
	
	USART_Init(9600);
	
	strcat(str,"\r\n");
	while(1){
		USART_Send_str(str);
		_delay_ms(1000);
	}
}