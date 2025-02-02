/*
 * File:   lab5_master.c
 * Author: Jos� Pellecer
 *
 * Created on February 19, 2020, 2:11 PM
 * La parte correspondiente a la comunicacion con el RTC fue basada en el trabajo mostrado en el siguiente
 * enlace: https://circuitdigest.com/microcontroller-projects/pic16f877a-ds3231-rtc-digital-clock
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "oscilador.h"
#include "LCD.h"
#include "I2C.h"
#include "USART.h"


#define _XTAL_FREQ 4000000

#define RS RB7
#define E RD7
#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
#define D5 RA5
#define D6 RA6
#define D7 RA7

void configPorts(void);
void configInterrupciones(void);
int  BCD_2_DEC(int to_convert);
int DEC_2_BCD (int to_convert);
void Set_Time_Date();
void Update_Current_Date_Time();
void separa_digitos_rtc(void);
void mostrar_lcd(void);
void primera_llamada(char h, char m, char s);
void segunda_llamada(char h, char m, char s);
void tercera_llamada(char h, char m, char s);
void apagar_luces(char t);
void check_infrarrojo(void);
void slave_1(void);
void slave_2(void);
void slave_3(void);
void boton_lcd(void);
void mandar_USART(void);

char b2(float m);
char b0(float m);
char b1(float m);



float pot;
//int sec, min, hour, day, date, month, year;

char sec_0, sec_1, min_0, min_1, hour_0, hour_1, date_0, date_1, month_0, month_1, year_0, year_1, modo, limpiar_lcd, temperatura, sismo, humo;
char num_case(uint8_t num);
char s1[5] = "";


//Este segmento sirve para configurar la hora y solo se activa cuando se desee configurar.
int sec = 00;
int min = 55;
int hour = 10;
int date = 24;
int day = 1;
int month = 02;
int year = 20;
    /*Time and Date Set*/
char unidades = 0;
char decenas = 0;
uint8_t AR1, s_temperatura, s_humo, s_temblor;

void __interrupt() isr(void){
    
    if (INTCONbits.RBIF == 1){
         INTCONbits.RBIF = 0;
         //INTCONbits.GIE = 0;
        if (PORTBbits.RB4 == 1){
            modo++;               //modo LCD
            lcd_clear();
            limpiar_lcd=1;
            __delay_ms(200);
    }
    //INTCONbits.GIE = 1; 
}
}


void main(void) {
    initOsci4MHZ();
    configPorts();
    configInterrupciones();
    lcd_init();         //se inicializa la pantalla
    I2C_Master_Init(100000);        //se inicializa el modo maestro con una frecuencia de 100k
    init_usart();
    
    //codigo para mostrar los titulos de la informacion de la LCD
    lcd_write_string("Fecha:");
    lcd_set_cursor(2, 1);
    lcd_write_string("Hora:");


    Set_Time_Date();      //Solo se activa cuando se desee configurar la hora
    
    s_humo = 0;
    s_temblor = 0;
    modo=0;
    
    while(1){
        
        Update_Current_Date_Time(); //Read the current date and time from RTC module
        boton_lcd();            //En esta funcion se decide que se mostrara en la lcd
        
        //Los parametros de las funciones de las llamadas son horas,minutos, segundos en que se activa la llamada.
        primera_llamada(10, 55, 5);
        segunda_llamada(10, 55, 10);
        tercera_llamada(10, 55, 15);
        apagar_luces(5);            //el parametro indica cuanto tiempo despues se apagar�n las luces de llamada.
        check_infrarrojo();         //funcion que revisa el sensor infrarrojo
        slave_1();
        slave_2();
        slave_3();
        mandar_USART();
        
        
        
        if(limpiar_lcd==1){
            lcd_clear();
            limpiar_lcd=0;
        }else
            limpiar_lcd=0;
                
  
    }
    return;
}


/**************************Comunicacion serial USART***************************/
void mandar_USART(void){
    envia_caracter_usart(200);
    envia_caracter_usart(sec);
    envia_caracter_usart(201);
    envia_caracter_usart(min);
    envia_caracter_usart(202);
    envia_caracter_usart(hour);
    envia_caracter_usart(203);
    envia_caracter_usart(date);
    envia_caracter_usart(204);
    envia_caracter_usart(month);
    envia_caracter_usart(205);
    envia_caracter_usart(year);
    envia_caracter_usart(206);
    envia_caracter_usart(unidades);
    envia_caracter_usart(207);
    envia_caracter_usart(temperatura);
    envia_caracter_usart(208);
    envia_caracter_usart(humo);
    envia_caracter_usart(209);
    envia_caracter_usart(sismo);
    
}
/********************************Cambio de modos*******************************/

void boton_lcd(void){
    if(modo==0){
        lcd_set_cursor(1, 1);
        lcd_write_string("Fecha:");
        lcd_set_cursor(2, 1);
        lcd_write_string("Hora:");
        separa_digitos_rtc();       //Separa los digitos en chars para mostrarlos en la lcd
        mostrar_lcd();          //imprime en la pantalla LCD la hora y la fecha
        
    }else if(modo==1){
        lcd_set_cursor(1, 1);
        lcd_write_string("Contador:");
        lcd_set_cursor(2, 1);             //esto sirve para revisar el contador en la lcd
        
        if(unidades==10){
            decenas++;
            unidades=0;
            
        }
        if(decenas==10){
            unidades=0;
            decenas=0;
        }
        
        lcd_write_char(decenas+48);
        lcd_write_char(unidades+48);
        
    }else if (modo==2){
        lcd_set_cursor(1, 1);
        lcd_write_string("Temperatura:");
        slave_1();
        lcd_set_cursor(2, 1);            // ESCRIBE EN LA LCD
        s1[2] = b0(temperatura);
        s1[1] = b1(temperatura);
        s1[0] = b2(temperatura);
        lcd_write_string(s1);
        lcd_write_char(223);
        lcd_write_string("C");
        
    }else if (modo==3){
        lcd_set_cursor(1, 1);
        lcd_write_string("Temblor:");
        slave_2();
        if (sismo == 1){ 
            lcd_set_cursor(2, 1);
            lcd_write_string("Esta temblando!");
            __delay_ms(3000);
            //lcd_clear();
            
        }
        else {
            //lcd_clear();
            lcd_set_cursor(2, 1);
            lcd_write_string("Tranquilo prro ");

        }
    }else if (modo==4){
        lcd_set_cursor(1, 1);
        lcd_write_string("Humo:");
        slave_3();
        if (humo == 1){ 
            lcd_set_cursor(2, 1);
            lcd_write_string("Presente");
            //__delay_ms(5000);
            //lcd_clear();   
        }
        else {
            //lcd_clear();
            lcd_set_cursor(2, 1);
            lcd_write_string("Ausente ");

        }
        
    }else if(modo>4){
        modo=0;
    }
}

void separa_digitos_rtc(void){
    sec_0 = sec%10;
    sec_1 = (sec/10);
    min_0 = min%10;
    min_1 = min/10;
    hour_0 = hour%10;
    hour_1 = hour/10;
    date_0 = date%10;           //se comenta porque solo se desea mostrar hora, minutos y segundos.
    date_1 = date/10;
    month_0 = month%10;
    month_1 = month/10;
    year_0 = year%10;
    year_1 = year/10;
    
}

void mostrar_lcd(void){
    lcd_set_cursor(1, 8);
        lcd_write_char(date_1+48);
        lcd_write_char(date_0+48);
        lcd_write_string("/");
        lcd_write_char(month_1+48);
        lcd_write_char(month_0+48);
        lcd_write_string("/");
        lcd_write_char(year_1+48);
        lcd_write_char(year_0+48);
        
        
        lcd_set_cursor(2, 8);
        lcd_write_char(hour_1+48);
        lcd_write_char(hour_0+48);
        lcd_write_string(":");
        lcd_write_char(min_1+48);
        lcd_write_char(min_0+48);
        lcd_write_string(":");
        lcd_write_char(sec_1+48);
        lcd_write_char(sec_0+48);
}

void primera_llamada(char h, char m, char s){
    // la primera llamada se hara cuando sean las h:m:s
    
    if(hour==h && min==m && sec==s){
        PORTBbits.RB0 = 1;
    }
}

void segunda_llamada(char h, char m, char s){
    // la segunda llamada se hara cuando sean las h:m:s
    
    if(hour==h && min==m && sec==s){
        PORTBbits.RB1 = 1;
    }
}

void tercera_llamada(char h, char m, char s){
    // la tercera llamada se hara cuando sean las h:m:s
    
    if(hour==h && min==m && sec==s){
        PORTBbits.RB2 = 1;
    }
}

void apagar_luces(char t){
    // las luces se apagaran t segundos despues de la tercera llamada
    
    if(hour==10 && min==55 && sec==(15+t)){
        PORTBbits.RB0 = 0;
        PORTBbits.RB1 = 0;
        PORTBbits.RB2 = 0;
    }
}

void configPorts(void){
    TRISA = 0;
    //TRISC = 0;
    TRISD = 0b00000001;     //el pin RDO es entrada digital (sensor infrarrojo))
    TRISE = 0;
    TRISB = 0b00010000;     // el pin RB4 es entrada digital (boton para modo de lcd))       
    
    PORTA = 0;
    //PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTB = 0;
    
    
    ANSEL = 0;
    ANSELH = 0;

}

void configInterrupciones(void){
    INTCONbits.GIE = 1;     //Habilita las interrupciones
    INTCONbits.PEIE =   1;     //Habilita las interrupciones de los perifericos
    
    INTCONbits.RBIE = 1;        //Interrupcion del PORTB
    INTCONbits.RBIF = 0;        //Bandera de interrupcion del PORTB        
    IOCBbits.IOCB4 = 1;         //Se habilita la interrupcion en el RB4          
}

//************************************Funciones para el RTC*********************************

//Convierte los numeros de binario a decimal
int  BCD_2_DEC(int to_convert){
   return (to_convert >> 4) * 10 + (to_convert & 0x0F); 
}

//Convierte los numeros de decimal a binario
int DEC_2_BCD (int to_convert){
   return ((to_convert / 10) << 4) + (to_convert % 10);
}


//Funcion para configurar la hora
void Set_Time_Date(){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);                 //Direccion del RTC
    I2C_Master_Write(0);
    I2C_Master_Write(DEC_2_BCD(sec));       //actualizar los segundos
    I2C_Master_Write(DEC_2_BCD(min));       //actualizar los minutos
    I2C_Master_Write(DEC_2_BCD(hour));      //actualizar la hora
    I2C_Master_Write(1);                    //actualiza el dia con un 1 siempre, pues no importa
    I2C_Master_Write(DEC_2_BCD(date));      //actualizar la fecha
    I2C_Master_Write(DEC_2_BCD(month));     //actualizar el mes
    I2C_Master_Write(DEC_2_BCD(year));      //actualizar el a�o
    I2C_Master_Stop();

}


//Funcion para leer la hora.
void Update_Current_Date_Time(){
   //START to Read 
   I2C_Master_Start(); 
   I2C_Master_Write(0xD0);
   I2C_Master_Write(0);
   I2C_Master_Stop();
   
   //READ
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                      //Bloque para guardar los datos del RTC en variables
   sec = BCD_2_DEC(I2C_Master_Read(0));         //De una vez se convierten de binario a decimal
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                      //Bloque para guardar los datos del RTC en variables
   min = BCD_2_DEC(I2C_Master_Read(0));         //De una vez se convierten de binario a decimal
   I2C_Master_Stop();
    
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                     
   hour = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                     
   I2C_Master_Read(0);                          //No nos interesa saber el dia       
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                     
   date = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                     
   month = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                     
   year = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();

   I2C_Master_Stop();
   
   //End reading
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);
   I2C_Master_Read(0);
   I2C_Master_Stop();

}

//*********************************Sensor infrarrojo***************************
void check_infrarrojo(void){
    if (PORTDbits.RD0 == 0){
            AR1=1; // CAMBIO DE ESTADO DEL BOTON
        }
        else {
            
            if(AR1 == 1){
                unidades++; // INCREMENTO DE CONTADOR
                AR1 = 0; // REGRESO AL BOTON ESTADO INICIAL
                __delay_ms(5); // DELAY POR SI LAS MOSCAS
            }
                
        }
        if (unidades==255){
                unidades = 0; 
                }
}

//***********Funciones para solicitar o enviar datos a los slaves**************

void slave_1(){
    I2C_Master_Start();
    I2C_Master_Write(0x11);      //aqui va la direccion del sensor 1 (el maestro recibe el dato))
    temperatura = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(5);
    
}
void slave_2(void){
    I2C_Master_Start();             //INICIA LA COMUNICACION
    I2C_Master_Write(0x21);         //ESCRIBE PARA RECIBIR
    sismo = I2C_Master_Read(0);     //GUARDA LO QUE LEE
    I2C_Master_Stop();              //TERMINA LA COMUNICACION
    __delay_ms(5);
    
}
void slave_3(void){
        I2C_Master_Start();             //INICIA LA COMUNICACION
        I2C_Master_Write(0x31);         //ESCRIBE PARA RECIBIR
        humo = I2C_Master_Read(0);     //GUARDA LO QUE LEE
        I2C_Master_Stop();              //TERMINA LA COMUNICACION
        __delay_ms(5);

}


//***********Funciones para separar los digitos de temperatura***************
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