/*
 * Nokia_511.c
 *
 * Created: 14-06-2025 23:49:04
 * Author : prakh
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include<avr/iom8a.h>
#include<util/delay.h>
#include<string.h>

#define SCK 5
#define MOSI 3
#define RESET 0
#define DC 1
#define SS 2



void SPI_init(void){
	DDRB |= (1<<SCK)|(1<<MOSI)|(1<<RESET)|(1<<DC)|(1<<SS);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
}

void SPI_SS_Enable(void){
	PORTB &= (0<<SS);
}

void SPI_SS_Disable(void){
	PORTB |= (1<<SS);
}

void SPI_Write(char data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));		
	
}

void N5110_Reset(void){
	PORTB &= (0 << RESET);
	_delay_ms(100);
	PORTB |= (1 << RESET);
}

void N5110_Cmnd(char Data){
	PORTB &= (0 << DC);
	SPI_SS_Enable();
	SPI_Write(Data);
	PORTB |= (1 << DC);
	SPI_SS_Disable();
	
}

void N5110_init()
{
	N5110_Reset();  /* reset the display */
	N5110_Cmnd(0x21);  /* command set in addition mode */
	N5110_Cmnd(0xC0);  /* set the voltage by sending C0 means VOP = 5V */
	N5110_Cmnd(0x07);  /* set the temp. coefficient to 3 */
	N5110_Cmnd(0x13);  /* set value of Voltage Bias System */
	N5110_Cmnd(0x20);  /* command set in basic mode */
	N5110_Cmnd(0x0C);  /* display result in normal mode */
}
/*
void N5110_Data(char *DATA)
{
	PORTB |= (1<<DC);  
	SPI_SS_Enable();  
	int lenan = strlen(DATA);  
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			SPI_Write(ASCII[DATA[g] - 0x20][index]); 
			
		}
		SPI_Write(0x00);
	}
	SPI_SS_Disable();
}
*/
void N5110_Data(char DATA)
{
	PORTB |= (1<<DC);
	SPI_SS_Enable();
	SPI_Write(DATA);
	
	//SPI_Write(0x00);
	
	SPI_SS_Disable();
}

void lcd_setXY(char x, char y)  /* set the column and row */
{
	N5110_Cmnd(x);
	N5110_Cmnd(y);
}

int main(void)
{
	SPI_init();
	
	
    /* Replace with your application code */
    while (1) 
    {
		N5110_Reset();
		
		N5110_Cmnd(0x21);
		N5110_Cmnd(0xC0);
		N5110_Cmnd(0x20);//function set
		N5110_Cmnd(0xC0);
		
		
		
		
		
		
		N5110_Data(0x1F);
		N5110_Data(0x05);
		N5110_Data(0x07);
		N5110_Data(0x00);
		N5110_Data(0x1F);
		N5110_Data(0x00);
		_delay_ms(5000);
    }
}

