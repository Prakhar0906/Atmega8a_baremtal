/*
 * IncFile1.h
 *
 * Created: 02-12-2025 19:39:45
 *  Author: prakh
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define LCD_PORT PORTD
void LCD_init(void);
void LCD_Command(unsigned char);
void LCD_Data(unsigned char);
void LCD_String(char *,int);
void LCD_Clear(void);




#endif /* INCFILE1_H_ */