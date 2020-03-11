/*
 * File:   USART.c
 * Author: José Pellecer
 *
 * Created on February 7, 2020, 9:56 AM
 * Basada en el codigo de electrosome.com y de microcontroladores http://microcontroladores-mrelberni.com/usart-pic-ejemplos/
 */


#include <xc.h>
#include "USART.h"

///////////////////////////////////////////////
//inicialización del módulo USART PIC modo asíncrono
//en una función, a 8bits,a 9600 baudios
/////////////////////////////////////////////////////
void init_usart(){
     TRISCbits.TRISC7=1;        //pin RX como una entrada digital
     TRISCbits.TRISC6=0;        //pin TX como una salida digital
     
     BAUDCTLbits.SCKP = 0;
     BAUDCTLbits.BRG16 = 0;
     BAUDCTLbits.ABDEN = 0;
     
     TXSTAbits.TXEN=1;         //transmision habilitada
     TXSTAbits.SYNC=0;         //asincrono
     TXSTAbits.TX9=0;         //
     TXSTAbits.BRGH=1;         //alta velocidad
     //TXSTA=0b00100110;          // 8bits, transmisión habilitada, asíncrono, alta velocidad
     
     RCSTAbits.SPEN=1;          //habilita el puerto serial
     RCSTAbits.RX9=0;          //recepcion de 8 bits
     RCSTAbits.CREN=1;          //habilita el receptor
     //RCSTA=0b10010000;          //habilitado el USART PIC, recepción 8 bits,  habilitada, asíncrono
     SPBRG=25;                  //9600baudios con un oscilador de 4Mhz
     
     SYNC = 0;
     SPEN = 1;
     CREN = 1;
     TXEN = 1;
     
    }
 
///////////////////////////////////////////////
//recepción de datos del módulo USART PIC modo asíncrono
////////////////////////////////////////////////////////////
unsigned char recibe_caracter_usart(){
    if(PIR1bits.RCIF==1){//si el bit5 del registro PIR1 se ha puesto a 1
    return RCREG;//devuelve el dato almacenado en el registro RCREG
    }
    else//sino
        return 0;//retorna sin hacer nada
}
 
///////////////////////////////////////////////
//transmisión de datos del módulo USART PIC modo asíncrono
///////////////////////////////////////////////
void envia_caracter_usart(unsigned char caracter){
        while(TXSTAbits.TRMT==0);// mientras el registro TSR esté lleno espera
	TXREG = caracter;//cuando el el registro TSR está vacio se envia el caracter
}
 
 
///////////////////////////////////////////////
//transmisión de cadenas de caracteres con el módulo USART PIC modo asíncrono
///////////////////////////////////////////////
void envia_cadena_usart(char* cadena){//cadena de caracteres de tipo char
    while(*cadena !=0x00){//mientras el último valor de la cadena sea diferente
                          //de el caracter nulo
        envia_caracter_usart(*cadena);//transmite los caracteres de cadena
        cadena++;//incrementa la ubicación de los caracteres en cadena
                 //para enviar el siguiente caracter de cadena
    }
}
