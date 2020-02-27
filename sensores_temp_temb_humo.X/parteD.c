/* 
 * File:   parteD.c
 * Author: Daniela Baldizon
 *
 * Created on 25 de febrero de 2020, 10:49 PM
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
#include "ADC.h"
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
void opcion_envio(uint8_t seleccion);

uint8_t adc, adc1, eleccion, z, envio;
uint8_t min = 0;
uint8_t SISMO;
uint8_t TEMP;
uint8_t TEMPA = 0;
uint8_t HUMO;

void __interrupt() ISR(void){
    if(PIR1bits.ADIF == 1){ // SI SE HABILITA LA INTERRUPCION
        adc1 = ADRESH; // guarda en la variable el valor del adres
        ADIF = 0; //LIMPIAR LA BANDERA
        ADCON0bits.GO = 1;      //INICIA EL ADC
    }
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            eleccion = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = envio;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISC = 0x00;
    TRISB = 0xFF;
    TRISD = 0x00;
    
    Lcd_Init();
    I2C_Slave_Init(0x10);
    
    //ADC
    ADC_CONFIG(13,0); //CANAL 13 PARA EL SENSOR DE TEMPERATURA 
    ADC_INTERRUPT(); //INTERRUPCION DEL ADC
    ADCON0bits.GO = 1;      //INICIA EL ADC
    
    //CONFIGURACIÓN DE LA LCD
    Lcd_Set_Cursor(1,1); //posicionar cursor
    Lcd_Write_String("Temp:"); //escribir
    Lcd_Set_Cursor(1,7); //posicionar cursor
    Lcd_Write_String("S2:"); // escribir
    Lcd_Set_Cursor(1,12);
    Lcd_Write_String("S3:");
   
    float adc;
    char s1[5] = "";
    
    while(1){
        TEMP = -0.3519*adc1+59.185+4;
        TEMP = (TEMP+TEMPA)/2; //PROMEDIAR LA TEMPERATURA ACTUAL CON LAS 3 ANTERIORES PARA ESTABILIZAR
        Lcd_Set_Cursor(2,1);
        s1[2] = b0(TEMP); // CONVERSOR DE INT A STRING
        s1[1] = b1(TEMP);
        s1[0] = b2(TEMP);
        Lcd_Write_String(s1);
        
        if (PORTBbits.RB1 == 1){ //EL SISMO VA EN EL RB1
            SISMO = 1; // CAMBIO DE ESTADO DEL SENSOR DE SISMO
            RD0 = 1;
        }
        else {
            SISMO = 0;
            RD0 = 0;
        }
        
        if(TEMP >=29){
            RD1 = 1; //PIN PARA EL RELAY
        }
        else{
            RD1 = 0;
        }
        
        if (PORTBbits.RB0 == 0){ //EL HUMO VA EN EL RB0
            HUMO = 1; // CAMBIO DE ESTADO DEL SENSOR DE HUMO
            RD3 = 1;
        }
        else {
            HUMO = 0;
            RD3 = 0;
        }
        
        
        TEMPA = TEMP;
        opcion_envio(eleccion);
    }
    
}

void opcion_envio(uint8_t seleccion){
    switch(seleccion){
        case 1:
            envio=TEMP;
            break;
        case 2:
            envio=HUMO;
            break;
        case 3:
            envio=SISMO;
            break;
        default:
            envio=250;
            break;
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
/*
void InitTimer0(){ //TMR0 1 ms
  OPTION_REGbits.T0CS = 0;
  OPTION_REGbits.PSA = 0;
  OPTION_REGbits.PS = 2;
  INTCONbits.T0IE = 1;
  TMR0 = 6;
  INTCONbits.T0IF = 0; // LIMPIAR BANDERA
  
}*/

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