#define F_CPU 4000000UL
#include<util/delay.h>
#include<avr/io.h>
#include<string.h>
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

void ADC_Init()
{
	DDRC=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
	
}

int ADC_Read(char channel)
{
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Read lower byte*/
	Ain = (int)ADCH*256;		/* Read higher 2 bits and 
					Multiply with weight */
	Ain = Ain + AinLow;				
	return(Ain);			/* Return digital value*/
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

int main(){
	WDTCR &= (0 << WDE);
	DDRD |= (1 << PORTD1);
	DDRC |= (1 << DDRC5);
	int value;
	ADC_Init();
	char str[10];
	
	
	USART_Init(9600);
	
	while(1){
		value = ADC_Read(5) * (255.0 / 1023);
		
		int_to_str(value, str);
		strcat(str,"\r\n");
		USART_Send_str(str);
		_delay_ms(1000);
	}
}