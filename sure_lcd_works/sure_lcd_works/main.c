#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 4000000

// Define LCD pin connections
#define LCD_RS  PB0
#define LCD_E   PB1
#define LCD_D4  PB4
#define LCD_D5  PB5
#define LCD_D6  PB6
#define LCD_D7  PB7

// Function Prototypes
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init();
void lcd_string(char *str);

// Main Function
int main(void)
{
	// Set data direction registers for Port B
	DDRB = 0xFF; // Set all pins of Port B as output

	lcd_init(); // Initialize the LCD

	lcd_string("Hello World!"); // Display string on LCD

	while(1)
	{
		// Your application code
	}

	return 0;
}

// Function Definitions
void lcd_init()
{
	// LCD initialization sequence
	lcd_command(0x02); // Initialize LCD in 4-bit mode
	lcd_command(0x28); // 2 line, 5x7 matrix
	lcd_command(0x0C); // Display on, cursor off
	lcd_command(0x06); // Increment cursor
	lcd_command(0x01); // Clear display
	_delay_ms(2);
}

void lcd_command(unsigned char cmd)
{
	// Send command to LCD
	PORTB = (PORTB & 0x0F) | (cmd & 0xF0); // Send higher nibble
	PORTB &= ~ (1<<LCD_RS); // RS=0 for command
	PORTB |= (1<<LCD_E); // E=1
	_delay_us(1);
	PORTB &= ~(1<<LCD_E); // E=0

	_delay_us(200);

	PORTB = (PORTB & 0x0F) | (cmd << 4); // Send lower nibble
	PORTB |= (1<<LCD_E); // E=1
	_delay_us(1);
	PORTB &= ~(1<<LCD_E); // E=0

	_delay_ms(2);
}

void lcd_data(unsigned char data)
{
	// Send data to LCD
	PORTB = (PORTB & 0x0F) | (data & 0xF0); // Send higher nibble
	PORTB |= (1<<LCD_RS); // RS=1 for data
	PORTB |= (1<<LCD_E); // E=1
	_delay_us(1);
	PORTB &= ~(1<<LCD_E); // E=0

	_delay_us(200);

	PORTB = (PORTB & 0x0F) | (data << 4); // Send lower nibble
	PORTB |= (1<<LCD_E); // E=1
	_delay_us(1);
	PORTB &= ~(1<<LCD_E); // E=0

	_delay_ms(2);
}

void lcd_string(char *str)
{
	// Display string on LCD
	while(*str != '\0')
	{
		lcd_data(*str);
		str++;
	}
}