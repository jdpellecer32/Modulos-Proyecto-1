/*
 * File:   ServoP1.c
 * Author: David Vela
 *
 * Created on 22 de febrero de 2020, 06:38 PM
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
#include <pic16f887.h>
char ADCvalant = 0;
char ADCval = 0;
char close = 0;
char open = 0;

void __interrupt() ISR(void){
    if (PIR1bits.TMR2IF == 1){
        PIR1bits.TMR2IF = 0;
        TRISCbits.TRISC1 = 0;
    }
    else
        asm("nop");
}
void setup(void);
char var1 = 0;

void main(void) {
    setup();
    while(1){
        //Sí la entrada RB7 es 1, cambia el PWM a 1ms (0°)
        if (PORTBbits.RB7 == 0){
            CCPR2L = 0b00000100;
            CCP2CONbits.DC2B0 = 1;
            CCP2CONbits.DC2B1 = 0;
        }
        //Sí la entrada RB6 es 1, cambia el PWM a 2ms (180°)
        else if (PORTBbits.RB7 == 1){
            CCPR2L = 0b00001011;
            CCP2CONbits.DC2B0 = 1;
            CCP2CONbits.DC2B1 = 1;
        }
        else{
        }
    }
    return;
}

void setup(void){
    //Entradas analógicas deshabilitadas
    ANSEL = 0;
    ANSELH = 0;
    //Establecemos un reloj interno de 500kHz
    OSCCONbits.IRCF = 3;
    //Inicializamos el puerto B
    TRISBbits.TRISB7 = 1;
    //Habilitamos las interrupciones 
    INTCON = 0b11000000;
    TRISCbits.TRISC1 = 0;
    PORTCbits.RC1 = 0;
    //Asignamos el prescaler del TMR2 y su bandera sin interrupción
    T2CONbits.T2CKPS = 2;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    //Cargamos el valor del PR2
    PR2 = 156;
    CCP2CONbits.CCP2M = 12;
    /*CCPR2L = 0b00001011;
    CCP2CONbits.DC2B0 = 1;
    CCP2CONbits.DC2B1 = 1;*/
    CCPR2L = 0b00000100;
    CCP2CONbits.DC2B0 = 1;
    CCP2CONbits.DC2B1 = 0;
    T2CONbits.TMR2ON = 1;
}