/* 
 * File:   LCD.h
 * Author: José Pellecer
 *
 * Created on February 5, 2020, 12:33 AM
 */

#ifndef LCD_H
#define	LCD_H

void lcd_port(char a);
void lcd_cmd(char a);
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(char a, char b);
void lcd_write_char(char a);
void lcd_write_string(char *a);
void lcd_shift_right();
void lcd_shift_left();

#endif	/* LCD_H */

