/* 
 * File:   LCD.c
 * Author: Daniela Baldizon
 *
 * Created on 4 de febrero de 2020, 10:36 PM
 */

#include <pic16f887.h>
#include "LCD.h"
#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 8000000
#define RS RC1 //DEFINIR VARIABLES MÁS SENCILLAS PARA LOS PINES
#define EN RC0
#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
#define D5 RA5
#define D6 RA6
#define D7 RA7
#define P1 RD0
#define P2 RD1
//Referencia del codigo de LCD del repositorio de GitHub https://github.com/pdmazariegos-uvg/ie3027/blob/master/Ejemplos/LCD4Bits.X/LCD.c
void Lcd_Port(char a) //Revisa bit por bit de a y según el valor de cada bit le asigna 1 o 0 a D
{
    if(a & 1) 
		D0 = 1;
	else
		D0 = 0;

	if(a & 2)
		D1 = 1;
	else
		D1 = 0;

	if(a & 4)
		D2 = 1;
	else
		D2 = 0;
    
	if(a & 8)
		D3 = 1;
	else
		D3 = 0;
    
	if(a & 16)
		D4 = 1;
	else
		D4 = 0;

	if(a & 32)
		D5 = 1;
	else
		D5 = 0;

	if(a & 64)
		D6 = 1;
	else
		D6 = 0;

	if(a & 128)
		D7 = 1;
	else
		D7 = 0;
}

void Lcd_Cmd(char a) //Envía datos a la LCD
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
    __delay_ms(5);
    EN  = 0;             // => E = 0
    __delay_ms(1);
}

void Lcd_Clear(void) //Limpiar la pantalla
{
	Lcd_Cmd(1);	
}

void Lcd_Set_Cursor(char a, char b)// a fila b columna SE MUEVE ENTRE FILAS Y COLUMNAS
{
	if(a == 1)
	{
		Lcd_Cmd(0x80 + b - 1);
	}
	else if(a == 2)
	{
		Lcd_Cmd(0xC0 + b - 1);
	}
}

void Lcd_Init(void)
{
  //Lcd_Port(0x00); //Limpia el puerto de la LCD
   __delay_ms(15);
  Lcd_Cmd(0x030); //Envía 0x30
  __delay_ms(5);
  Lcd_Cmd(0x030);
  __delay_us(160);
  Lcd_Cmd(0x030);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x38);//SET LENGTH
  Lcd_Cmd(0x10);//Visualizador apagado
  Lcd_Cmd(0x01);//Visualizador borrado
  Lcd_Cmd(0x06);//Introducir los caracteres
  Lcd_Cmd(0x0C);//0x0c para quitar el cursor, 0x0f para ponerlo
  
}

void Lcd_Write_Char(char a) //ESCRIBE UN CARACTER EN LA POSICION ACTUAL
{

   RS = 1;             // => RS = 1 LOS D SON DATOS
   Lcd_Port(a);             //Data transfer
   EN = 1;
   __delay_us(100);
   EN = 0;
}

void Lcd_Write_String(char *a) //ESCRIBIR UNA CADENA DE CARACTERES
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void)//MOVER EL VISUALIZADOR HACIA LA DERECHA
{
	Lcd_Cmd(0x1C);
}

void Lcd_Shift_Left(void)//MOVER EL VISUALIZADOR HACIA LA IZQUIERDA
{
	Lcd_Cmd(0x18);
}