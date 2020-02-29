/* 
 * File:   Slave_Temperatura.c
 * Author: Daniela Baldizon
 *
 * Created on 28 de febrero de 2020, 09:45 PM
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
#include "ADC.h"
#include "I2C.h"

#define _XTAL_FREQ 8000000


uint8_t adc, adc1, z, eleccion;
uint8_t TEMP;
uint8_t TEMPA = 0;

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
            SSPBUF = TEMP;              // GUARDAR TEMP EN EL BUFFER
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
    
    // I2C
    I2C_Slave_Init(0x10);
    
    //ADC
    ADC_CONFIG(13,0); //CANAL 13 PARA EL SENSOR DE TEMPERATURA 
    ADC_INTERRUPT(); //INTERRUPCION DEL ADC
    ADCON0bits.GO = 1;      //INICIA EL ADC

    while(1){
        TEMP = -0.3519*adc1+59.185+4;
        TEMP = (TEMP+TEMPA)/2; //PROMEDIAR LA TEMPERATURA ACTUAL CON LAS 3 ANTERIORES PARA ESTABILIZAR
        
        if(TEMP >=29){      //   CONTROL DEL RELAY
            RD1 = 1;        //PIN PARA EL RELAY
        }
        else{
            RD1 = 0;
        }
        
        
        TEMPA = TEMP;       // GUARDA EL VALOR ANTERIOR
    }
    
}

