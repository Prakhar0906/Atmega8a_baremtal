/*
 * LCD_My_implementation.c
 *
 * Created: 19-09-2025 21:02:14
 * Author : prakh
 */ 

#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#define LCD_PORT PORTD

#define REGISTER_SEL_PIN  PD1
#define ENABLE_PIN   PD2
#define LCD_D4  PD4
#define LCD_D5  PD5
#define LCD_D6  PD6
#define LCD_D7  PD7

int cursor_ind = 0;

void LCD_init(){
	
	LCD_Command(0x02); // Initialize LCD in 4-bit mode
	LCD_Command(0x28); // 2 line, 5x7 matrix
	LCD_Command(0x0D); // Display on, cursor off
	LCD_Command(0x06); // Increment cursor
	LCD_Command(0x01); // Clear display
	
	
	_delay_ms(20); // arbitary delay
}

void LCD_Command(unsigned char command){
	LCD_PORT = (LCD_PORT & 0x0F) | (command & 0xF0); // Send high nibble
	LCD_PORT &= (0 << REGISTER_SEL_PIN); // RS=0 for command
	
	LCD_PORT |= (1 << ENABLE_PIN);
	_delay_us(1);
	LCD_PORT &= ~(1 << ENABLE_PIN);
	
	_delay_us(200);
	LCD_PORT = (LCD_PORT & 0x0F) | (command << 4);

	LCD_PORT |= (1 << ENABLE_PIN);
	_delay_us(1);
	LCD_PORT &= ~(1 << ENABLE_PIN);
	_delay_ms(10);
	
}



void LCD_Data(unsigned char command){
	
	
	LCD_PORT = (LCD_PORT & 0x0F) | (command & 0xF0); // Send higher nibble
	LCD_PORT |=  (1<<REGISTER_SEL_PIN); // RS=1 for data
	LCD_PORT |= (1 << ENABLE_PIN);
	_delay_us(1);
	LCD_PORT &= ~(1 << ENABLE_PIN);

	_delay_us(200);

	LCD_PORT = (LCD_PORT & 0x0F) | (command << 4); // Send lower nibble
	LCD_PORT |= (1 << ENABLE_PIN);
	_delay_us(1);
	LCD_PORT &= ~(1 << ENABLE_PIN);
	_delay_ms(2);
	
}
void lcd_string(char *str,int line)
{
	if (line == 1){
		while(*str != '\0')
		{
			LCD_Data(*str);
			str++;
			cursor_ind++;
		}
	}
	else if (line == 2){
		int n_spaces = 40 - cursor_ind ;
		for(int i=0;i< n_spaces;i++)
			LCD_Data(' ');
		while(*str != '\0')
		{
			LCD_Data(*str);
			str++;
		}
		cursor_ind = 0;
	}
	// Display string on LCD
	
}

void LCD_Clear(){
	LCD_Command(0x01);
	cursor_ind = 0;
}

int ADC_Read()
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

//3906 steps

int main(void)
{
    
	DDRD = 0xFF;
	DDRC |= (1 << DDRC0);
	ADMUX |= (1 << MUX2) | (1 << MUX0);
	ADCSRA |= (1 << ADEN);
	
	
	
	LCD_PORT &= ~(1 << ENABLE_PIN); // make the enable pin hign
	_delay_ms(10);
	LCD_init();
	int volt,c;
	char s[5];
	

	
    while (1) 
    {
		LCD_Clear();
		volt = ADC_Read();
		int_to_str(volt,s);
		lcd_string(s,1);
		_delay_ms(500);
		
    }
}


