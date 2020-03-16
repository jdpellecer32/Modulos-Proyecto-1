/*
 * File:   StepperP1.c
 *
 * Created on 26 de febrero de 2020, 04:15 PM
 * 
 * Definición de las funciones para el manejo del stepper extraído de: 
 * https://circuitdigest.com/microcontroller-projects/interfacing-stepper-motor-with-pic16f877a
 * 
 * Arreglos realizados por: David Vela
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include "stepperlib.h"

char z;
char  signal = 0;
char dummyvar = 0;
uint8_t sel = 0;
uint8_t send = 0;
uint8_t subir = 0;
uint8_t bajar = 0;

void setup(void);

void __interrupt() ISR(void){
    //Rutina de interrupción con los botones del puerto B
    if (INTCONbits.RBIF == 1){
        INTCONbits.RBIF = 0;
        subir = PORTBbits.RB7;
        bajar = PORTBbits.RB6;
    }
    else;
}

void main(void){
    setup();
    while(1){
        if ((subir == 0)&&(bajar == 0)); // Si ningún botón se presiona, no hay op. 
        
        else if ((subir == 1)&&(bajar == 1)); // Si ambos se presionan, no hay op.
        
        else if (subir == 1){ // Si se presiona para subir, comienza la rutina
            //Se hace un for para los pasos establecidos en el header
            for(int i=0;i<steps;i++){
                wave_drive(clockwise);
                if (bajar == 0);
                //Si durante la subida se presiona bajar, se termina la rutina
                else if (bajar == 1){
                    break;
                }
            }
        }
        else if (bajar == 1){ // Si se presiona para bajar, comienza la rutina
            //Se hace un for para los pasos establecidos en el header
            for(int i=0;i<steps;i++){
                wave_drive(anti_clockwise);
                if (subir == 0); //Si durante la bajada se presiona subir, se termina
                else if (subir == 1){
                    break;
                }
            }
        }
        }
    }


void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0b11000000;
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
    IOCBbits.IOCB7 = 1;
    IOCBbits.IOCB6 = 1;
    INTCON = 0b11001000;
    OSCCONbits.IRCF = 7; //Oscilador de 8MHz
}
