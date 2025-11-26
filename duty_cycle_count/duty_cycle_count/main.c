/*
 * duty_cycle_count.c
 *
 * Created: 25-11-2025 23:18:38
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


int main(void)
{
	DDRD = 0xFF;
	LCD_PORT &= ~(1 << ENABLE_PIN); // make the enable pin hign
	_delay_ms(10);
	LCD_init();
	LCD_Clear();
	lcd_string("yes",1);
	int first_rise,first_fall,second_rise,high,period;
	char duty[10];
    /* Replace with your application code */
	TCCR1A = 0;
    while (1) 
    {
		
		TCCR1A = 0;
		TCNT1 = 0;
		TIFR = (1 << ICF1);
		
		TCCR1B = (1 << ICES1)|(1 << CS11);
		while((TIFR&(1<<ICF1)) == 0);
		first_rise = ICR1;
		TIFR = (1 << ICF1);
		
		TCCR1B = 0x02;
		while((TIFR&(1<<ICF1))==0);
		first_fall = ICR1;
		TIFR = (1 << ICF1);
		
		TCCR1B = (1 << ICES1)|(1 << CS11);
		while((TIFR&(1<<ICF1))==0);
		second_rise = ICR1;
		TIFR = (1 << ICF1);
		
		TCCR1B = 0;
		
		
		
		if(first_rise < first_fall && first_fall << second_rise){
			high = first_fall - first_rise;
			period = second_rise - first_rise;
			
			long freq = F_CPU/period;
			long duty_cycle = ((float) high / (float)period)*100;
			
			int_to_str((int)duty_cycle,duty);
			LCD_Clear();
			lcd_string(duty,1);
				
		}
		_delay_ms(100);
		
		
    }
}

