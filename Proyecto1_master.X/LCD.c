/*
 * File:   LCD.c
 * Author: José Pellecer
 * Librería para el uso de una pantalla LCD 2x16 de 8 bits 
 * Basado en el código de electrosome.com
 * Created on February 04, 2020, 9:32 PM
 */

#include <xc.h>
#include "LCD.h"
#define _XTAL_FREQ 8000000

#define RS RB7
#define E RD7
#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
#define D5 RA5
#define D6 RA6
#define D7 RA7

void lcd_port(char a){
    //Realiza ANDs por cada bit de a, si es verdadero, pone el pin en HIGH
    if(a & 1){
        D0 = 1;
    } else {
        D0 = 0;
    }
    
    if(a & 2){
        D1 = 1;
    } else {
        D1 = 0;
    }
    
    if(a & 4){
        D2 = 1;
    } else {
        D2 = 0;
    }
    
    if(a & 8){
        D3 = 1;
    } else {
        D3 = 0;
    }
    
    if(a & 16){
        D4 = 1;
    } else {
        D4 = 0;
    }
    
    if(a & 32){
        D5 = 1;
    } else {
        D5 = 0;
    }
    
    if(a & 64){
        D6 = 1;
    } else {
        D6 = 0;
    }
    
    if(a & 128){
        D7 = 1;
    } else {
        D7 = 0;
    }
}

void lcd_cmd(char a){
    //Cambia a modo de comandos con el RS
    RS = 0;
    lcd_port(a);    //Envía el valor del comando a la pantalla
    //Manda el flanco para que el integrado detecte el comando
    E = 1;
    __delay_ms(10);
    E = 0;
}

void lcd_init(void){
    //Inicialización de la pantalla
    __delay_ms(15);
    lcd_port(0x00);
    __delay_ms(20);
    lcd_cmd(0x30);
	__delay_ms(5);
    lcd_cmd(0x30);
	__delay_us(160);
    lcd_cmd(0x30);
    
    //Configuración de la pantalla
    lcd_cmd(0x38);      //Modo de 8 bits, 2 líneas y 5*7 pixeles
    lcd_cmd(0x10);      //Apagar pantalla
    lcd_cmd(0x01);      //Limpiar pantalla
    lcd_cmd(0x06);      //Incremento sin desplazamiento 
    lcd_cmd(0x0C);      //Pantalla encendida, cursor activado, con parpadeo  
       
}

void lcd_clear(void){
    //Limpiar la pantalla con lcd_cmd(1)
    lcd_cmd(0x01);
}

void lcd_set_cursor(char a, char b){
    if (a == 1){
        //Manda el comando para la ubicación de la DDRAM
        //Fila 1, posición b
        lcd_cmd(0x80 + b - 1);
    } else if (a == 2){
        //Fila 2, posición b
        lcd_cmd(0xC0 + b - 1);
    }
}

void lcd_write_char(char a){
    //RS en 1 para escribir en la DDRAM
    RS = 1;
    lcd_port(a);        //Envía el valor del caracter a la pantalla
    //Flanco en E para que detecte los datos
    E = 1;
    __delay_ms(4);
    E = 0;
}

void lcd_write_string(char *a){
    //Manda caracter por caracter
    int i;
    for (i = 0; a[i] != '\0'; i++) 
        lcd_write_char(a[i]);
}

void lcd_shift_right(){
    //Desplazamiento a la derecha
    lcd_cmd(0x1C);
}

void lcd_shift_left(){
    //Desplazamiento a la izquierda
    lcd_cmd(0x18);
}
