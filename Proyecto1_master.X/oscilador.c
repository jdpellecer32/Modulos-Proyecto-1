/*
 * File:   oscilador.c
 * Author: José Pellecer
 *
 * Created on January 28, 2020, 4:00 PM
 */


#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h>
#include <stdint.h>
#include "oscilador.h"
// Funcion para seleccionar el oscilador interno del PIC


void initOsci31KHZ(void){
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.SCS = 1;
}
void initOsci125KHZ(void){
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.SCS = 1;
}
void initOsci250KHZ(void){
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.SCS = 1;
}
void initOsci500KHZ(void){
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;
}
void initOsci1MHZ(void){
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.SCS = 1;
}
void initOsci2MHZ(void){
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;
}
void initOsci4MHZ(void){
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;
}
void initOsci8MHZ(void){
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;
}


