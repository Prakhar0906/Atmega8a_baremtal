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

#ifndef LCD_PORT
#define LCD_PORT PORTD
#endif
#ifndef ENABLE_PIN
#define ENABLE_PIN PD2
#endif


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

int get_dutyCycle(void){
	long duty_cycle;
	int first_rise,first_fall,second_rise,high,period;
	TCCR1A = 0;
	TCNT1 = 0;
	TIFR = (1 << ICF1);
	
	TCCR1B = (1 << ICES1)|(1 << CS10);
	while((TIFR&(1<<ICF1)) == 0);
	first_rise = ICR1;
	TIFR = (1 << ICF1);
	
	TCCR1B = (1 << CS10);
	while((TIFR&(1<<ICF1))==0);
	first_fall = ICR1;
	TIFR = (1 << ICF1);
	
	TCCR1B = (1 << ICES1)|(1 << CS10);
	while((TIFR&(1<<ICF1))==0);
	second_rise = ICR1;
	TIFR = (1 << ICF1);
	
	TCCR1B = 0;
	
	
	
	if(first_rise < first_fall && first_fall << second_rise){
		high = first_fall - first_rise;
		period = second_rise - first_rise;
		duty_cycle = ((float) high / (float)period)*100;
	}
	return duty_cycle;
}


int main(void)
{
	DDRD = 0xFF;
	DDRC |= (1 << DDRC5);
	LCD_PORT &= ~(1 << ENABLE_PIN); // make the enable pin high
	_delay_ms(10);
	LCD_init();
	LCD_Clear();
	LCD_String("waiting for response",1);
	char str[10];
    /* Replace with your application code */
	TCCR1A = 0;
	long water_level;
	int refilling = 1;
    while (1) 
    {
		water_level = get_dutyCycle();
		
		if(water_level <= 15){
			refilling = 1;
		}else if(water_level >=95){
			refilling = 0;
		}
		
		if(water_level > 15 && water_level < 95 && refilling == 0){
			PORTC &= (0 << PORTC5);
		}else if(refilling == 1){
			PORTC |= (1 << PORTC5);
		}
		LCD_Clear();
		water_level = (int) water_level;
		int_to_str(water_level,str);
		LCD_String(str,1);
		_delay_ms(800);
		
		/*
		LCD_Clear();
		water_level = (int) water_level;
		int_to_str(water_level,str);
		LCD_String(str,1);
		_delay_ms(10);
		*/
    }
}

