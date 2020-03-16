/* 
 * File:   Traductor.c
 * Author: Daniela Baldizon
 *
 * Created on 5 de marzo de 2020, 11:05 PM
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
#include "SPI.h"
#include "USART.h"


#define _XTAL_FREQ 4000000

// VARIABLES GLOBALES
char C_UART = 0;
char ENVIO;
char sec;
char hour;
char min;
char date;
char month;
char year;
char unidades;
char temperatura;
char humo;
char sismo;
char contador;
char cambio;
char resta;
uint8_t ctrl = 0;

void __interrupt() isr(){
   if(SSPIF == 1){              // REVISA LA INTERRUPCION DEL SPI
       ENVIO = spiRead();       // LEE LA SOLICITUD DE LA RASPBERRY
       if(ENVIO == 0){
           spiWrite(sec);
       }
       else if(ENVIO == 1){
           spiWrite(min);
       }
       else if(ENVIO == 2){
           spiWrite(hour);
       }
       else if(ENVIO == 3){
           spiWrite(date);
       }
       else if(ENVIO == 4){
           spiWrite(month);
       }
       else if(ENVIO == 5){
           spiWrite(year);
       }
       else if(ENVIO == 6){
           spiWrite(contador);
       }
       else if(ENVIO == 7){
           spiWrite(temperatura);
       }
       else if(ENVIO == 8){
           spiWrite(humo);
       }
       else{
           spiWrite(sismo);
       }
       SSPIF = 0;                  //LIMPIA LA BANDERA DEL SPI PINCHE SPI TE ODIOOOOOOOO
    }
}

void main(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    TRISA = 0;
    PORTA = 0;
    
    init_usart();
    
    //CONFIGURACION DEL SPI
    PIR1bits.SSPIF = 0;     //SE LIMPIA LA BANDERA
    PIE1bits.SSPIE = 1;     //INTERRUPCION ACTIVA
    INTCONbits.PEIE = 1;    //SE HABILITAN LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.GIE = 1;     //SE HABILITAN LAS INTERRUPCIONES GLOBALES
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_HIGH, SPI_ACTIVE_2_IDLE); // ESCLAVO
    
    
    
    while(1){
        C_UART = recibe_caracter_usart();
        if(C_UART == 200){ // ESPERA LA BANDERA
            sec = recibe_caracter_usart();
        }    
        if(C_UART == 201){ // ESPERA LA BANDERA
            min = recibe_caracter_usart();
        }
        if(C_UART == 202){ // ESPERA LA BANDERA
            hour = recibe_caracter_usart();
        }
        if(C_UART == 203){ // ESPERA LA BANDERA
            date = recibe_caracter_usart();
        }
        if(C_UART == 204){ // ESPERA LA BANDERA
            month = recibe_caracter_usart();
        }
        if(C_UART == 205){ // ESPERA LA BANDERA
            year = recibe_caracter_usart();
        }
        if(C_UART == 206){ // ESPERA LA BANDERA
            unidades = recibe_caracter_usart();
        }
        if(C_UART == 207){ // ESPERA LA BANDERA
            temperatura = recibe_caracter_usart();
        }
        if(C_UART == 208){ // ESPERA LA BANDERA
            humo = recibe_caracter_usart();
        }
        if(C_UART == 209){ // ESPERA LA BANDERA
            sismo = recibe_caracter_usart();
        }
        
        PORTB = sec;
        
        
        
        resta = unidades - cambio;
        if(resta != 0){ // SI DETECTA CAMBIO EN UNIDADES AUMENTA LA VARIABLE A LA MISMA CANTIDAD DE LA RESTA, YA QUE ES LO QUE CAMBIO LA VARIABLE
            if(unidades == 9){
            ctrl++;
            }
        }
        
        
        contador = unidades + 10*ctrl;
        cambio = unidades; // GUARDAR EL VALOR ANTERIOR DE LAS UNIDADES*/
    }
}