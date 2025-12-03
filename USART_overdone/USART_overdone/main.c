#include <avr/io.h>
#define F_CPU 4000000
#include "usart_func.h"



int main(void)
{
	DDRD |= (1 << DDRD3);
	USART_Init(9600);
	
    /* Replace with your application code */
    while (1) 
    {
		USART_Send_str("hello\r\n");
    }
}

