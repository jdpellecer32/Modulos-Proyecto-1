/*
 * File:   USART.c
 * Author: Jos� Pellecer
 *
 * Created on February 7, 2020, 9:56 AM
 * Basada en el codigo de electrosome.com y de microcontroladores http://microcontroladores-mrelberni.com/usart-pic-ejemplos/
 */


#include <xc.h>
#include "USART.h"

///////////////////////////////////////////////
//inicializaci�n del m�dulo USART PIC modo as�ncrono
//en una funci�n, a 8bits,a 9600 baudios
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
     //TXSTA=0b00100110;          // 8bits, transmisi�n habilitada, as�ncrono, alta velocidad
     
     RCSTAbits.SPEN=1;          //habilita el puerto serial
     RCSTAbits.RX9=0;          //recepcion de 8 bits
     RCSTAbits.CREN=1;          //habilita el receptor
     //RCSTA=0b10010000;          //habilitado el USART PIC, recepci�n 8 bits,  habilitada, as�ncrono
     SPBRG=25;                  //9600baudios con un oscilador de 4Mhz
     
     SYNC = 0;
     SPEN = 1;
     CREN = 1;
     TXEN = 1;
     
    }
 
///////////////////////////////////////////////
//recepci�n de datos del m�dulo USART PIC modo as�ncrono
////////////////////////////////////////////////////////////
unsigned char recibe_caracter_usart(){
    if(PIR1bits.RCIF==1){//si el bit5 del registro PIR1 se ha puesto a 1
    return RCREG;//devuelve el dato almacenado en el registro RCREG
    }
    else//sino
        return 0;//retorna sin hacer nada
}
 
///////////////////////////////////////////////
//transmisi�n de datos del m�dulo USART PIC modo as�ncrono
///////////////////////////////////////////////
void envia_caracter_usart(unsigned char caracter){
        while(TXSTAbits.TRMT==0);// mientras el registro TSR est� lleno espera
	TXREG = caracter;//cuando el el registro TSR est� vacio se envia el caracter
}
 
 
///////////////////////////////////////////////
//transmisi�n de cadenas de caracteres con el m�dulo USART PIC modo as�ncrono
///////////////////////////////////////////////
void envia_cadena_usart(char* cadena){//cadena de caracteres de tipo char
    while(*cadena !=0x00){//mientras el �ltimo valor de la cadena sea diferente
                          //de el caracter nulo
        envia_caracter_usart(*cadena);//transmite los caracteres de cadena
        cadena++;//incrementa la ubicaci�n de los caracteres en cadena
                 //para enviar el siguiente caracter de cadena
    }
}
