/*
 * Pot_led_control.c
 *
 * Created: 14-06-2025 10:37:57
 * Author : prakh
 */ 
#define F_CPU 4000000UL


#include<avr/io.h>
#include<util/delay.h>
#include <usart.h>
#include <string.h>

#include <stdlib.h>

#define SIZE 10




int compare(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int mod(int* a, int n){
	qsort(a, n, sizeof(int), compare);
	int mode = a[0];
	int curr_count = 1;
	int max_count = 1;

	// Iterate through the sorted array to find the mode
	for (int i = 1; i < n; ++i) {
		if (a[i] == a[i - 1]) {
			// Increment the count if the current element is
			// equal to the previous one
			++curr_count;
		}
		else {
			// Check if the count of the previous element is
			// greater than the maximum count
			if (curr_count > max_count) {
				max_count = curr_count;
				mode = a[i - 1];
			}
			// Reset the count for the current element
			curr_count = 1;
		}
	}

	// Check the count of the last element
	if (curr_count > max_count) {
		mode = a[n - 1];
	}	
	return (int)mode;
}

void int_to_str(int N, char *str){
	int i=0;
	int sign = 0;
	if (N < 0){
		str[i++] = '-';
		N=N*-1;
		
	}
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

int get_val(void){
	unsigned int t1;
	unsigned int t2;
	
	TCCR1A = 0;
	TIFR |=(1<<ICF1);
	TCCR1B = 0x41;
	PORTD |= (1 << PORTD7);
	_delay_us(10);
	PORTD &= (0 << PORTD7);
	while((TIFR&(1<<ICF1))==0);
	t1 = ICR1;
	TIFR |= (1 <<ICF1);
	
	TCCR1B = 0x01;
	while((TIFR&(1<<ICF1))==0);
	t2 = ICR1;
	
	double time_dur = ((t2-t1)/4)/58.0;
	double pwm = time_dur*(-14.16) + 354.12;
	
	
	return (int)pwm;
	
}



int main(void)
{
	DDRB |= (1 << DDRB3);	
	DDRD |= (1 << DDRD7)|(1 << DDRD1);
	
	USART_Init(9600);

	
	PORTD &= (0 << PORTD7);
	int pwm = 2;
	char str[10];
	int vals[SIZE];
	OCR2 = 100;
	TCCR2 |= (1 << WGM21)|(1<<WGM20)|(1<<COM21)|(1<<CS22);
    while (1) 
    {	
		for(int i=0;i<SIZE;i++){
			vals[i] = get_val();
			_delay_ms(200);
		}
		pwm = mod(vals,SIZE);
		if(pwm > 248){
			pwm = 248;
		}
		if(pwm < 8){
			pwm = 8;
		}
		OCR2 = pwm;
		
    }
}

