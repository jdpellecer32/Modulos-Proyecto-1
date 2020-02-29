/* 
 * File:   Master_HTS.c
 * Author: Daniela Baldizon
 *
 * Created on 28 de febrero de 2020, 10:05 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LCD.h"
#include "I2C.h"

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

#define _XTAL_FREQ 8000000

char num_case(uint8_t num);
char b2(float m);
char b0(float m);
char b1(float m);

uint8_t TEMPERATURA, SISMO, HUMO;

void main(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISC = 0x00;
    TRISB = 0x00;
    TRISD = 0x00;
    
    Lcd_Init();
    // CONFIGURACIÓN DEL I2C COMO MAESTRO
    I2C_Master_Init(100000);
    
    //CONFIGURACIÓN DE LA LCD
    Lcd_Set_Cursor(1,1); //posicionar cursor
    Lcd_Write_String("Temp:"); //escribir
   
    char s3[5] = "";
    
    while(1){
        
        I2C_Master_Start();             //INICIA LA COMUNICACION
        I2C_Master_Write(0x11);         //ESCRIBE PARA RECIBIR
        TEMPERATURA = I2C_Master_Read(0);     //GUARDA LO QUE LEE
        I2C_Master_Stop();              //TERMINA LA COMUNICACION
        __delay_ms(5);
        
        Lcd_Set_Cursor(2,1);            // ESCRIBE EN LA LCD
        s3[2] = b0(TEMPERATURA);
        s3[1] = b1(TEMPERATURA);
        s3[0] = b2(TEMPERATURA);
        Lcd_Write_String(s3);
        
        
        I2C_Master_Start();             //INICIA LA COMUNICACION
        I2C_Master_Write(0x21);         //ESCRIBE PARA RECIBIR
        SISMO = I2C_Master_Read(0);     //GUARDA LO QUE LEE
        I2C_Master_Stop();              //TERMINA LA COMUNICACION
        __delay_ms(5);
        
        I2C_Master_Start();             //INICIA LA COMUNICACION
        I2C_Master_Write(0x31);         //ESCRIBE PARA RECIBIR
        HUMO = I2C_Master_Read(0);     //GUARDA LO QUE LEE
        I2C_Master_Stop();              //TERMINA LA COMUNICACION
        __delay_ms(5);
        
        if (SISMO == 1){                // MOSTRAR EN EL MASTER LOS DATOS DEL ESCLAVO
            RB0 = 1;
        }
        else {
            RB0 = 0;
        } 
        
        if (HUMO == 1){                // MOSTRAR EN EL MASTER LOS DATOS DEL ESCLAVO
            RB1 = 1;
        }
        else {
            RB1 = 0;
        } 
    }
    
}

char num_case(uint8_t num){ //funcion que pasa un numero a string
    char r;
    switch(num){
        case 0:
            r = '0';
            break;
        case 1:
            r = '1';
            break;
        case 2:
            r = '2';
            break;
        case 3:
            r = '3';
            break;
        case 4:
            r = '4';
            break;
        case 5:
            r = '5';
            break;
        case 6:
            r = '6';
            break;
        case 7:
            r = '7';
            break;
        case 8:
            r = '8';
            break;
        case 9:
            r = '9';
            break;
    }
    return r;
}

char b2(float m){ //hacer el contador, pasar el "bit 2" a string
    uint8_t bb2;
    float local;
    char p2;
    
    bb2 = m/100;
    p2 = num_case(bb2);
    return p2;
}

char b1(float m){// pasar el "bit 1" a string
    uint8_t bb2;
    uint8_t bb1;
    uint8_t local;
    char p1;
    
    bb2 = m/100;
    local = m-bb2*100;
    bb1 = local/10;
    p1 = num_case(bb1);
    return p1;
}

char b0(float m){ //pasar el "bit0" a string
    uint8_t bb2;
    uint8_t bb1;
    uint8_t bb0;
    uint8_t local;
    char p0;
    
    bb2 = m/100;
    local = m-bb2*100;
    bb1 = local/10;
    bb0 = local-bb1*10;
    p0 = num_case(bb0);
    return p0;
}
