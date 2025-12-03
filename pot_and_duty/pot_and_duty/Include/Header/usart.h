/*
 * usart.h
 *
 * Created: 02-12-2025 18:22:46
 *  Author: prakh
 */ 


#ifndef USART_H_
#define USART_H_

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART_Init(long);
void USART_Flush(void);
unsigned char USART_Receive(void);
void USART_Transmit(char);
void USART_Send_str(char*);
void USART_Receive_str(char*);
void save_str(char*);
#endif /* USART_H_ */