void USART_Init(long USART_BAUDRATE);
void USART_Flush(void);
unsigned char USART_Receive(void);
void USART_Transmit(char data);
void USART_Send_str(char* str);
USART_Receive_str(char* str);
void save_str(char* s);

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
