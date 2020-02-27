/* 
 * File:   LCD.h
 * Author: Daniela Baldizon
 *
 * Created on 4 de febrero de 2020, 10:35 PM
 */

#ifndef LCD_H
#define	LCD_H
#include <stdint.h>

#include <xc.h>

void Lcd_Init(void);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Cmd(char a);

#endif	/* LCD_H */

