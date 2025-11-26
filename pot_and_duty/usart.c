
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

