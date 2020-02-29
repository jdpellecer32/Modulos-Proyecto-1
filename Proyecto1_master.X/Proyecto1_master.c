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


#define _XTAL_FREQ 8000000

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
void slave_1(uint8_t sensor);
void slave_2(void);
void slave_3(void);
void boton_lcd(void);



float pot;
//int sec, min, hour, day, date, month, year;

char sec_0, sec_1, min_0, min_1, hour_0, hour_1, date_0, date_1, month_0, month_1, year_0, year_1, modo, limpiar_lcd;

//Este segmento sirve para configurar la hora y solo se activa cuando se desee configurar.
int sec = 00;
int min = 55;
int hour = 10;
int date = 24;
int day = 1;
int month = 02;
int year = 20;
    /*Time and Date Set*/
uint8_t sens = 0;
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
    initOsci8MHZ();
    configPorts();
    configInterrupciones();
    lcd_init();         //se inicializa la pantalla
    I2C_Master_Init(100000);        //se inicializa el modo maestro con una frecuencia de 100k
    
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
        
        //slave_1(1);
        //slave_1(2);
        //slave_1(3);
        
        //PORTBbits.RB0 =s_humo;
        //PORTBbits.RB1 = s_temblor;
        
        /*lcd_set_cursor(1, 1);            
        lcd_write_char(s_humo+48);
        lcd_set_cursor(2, 1);            
        lcd_write_char(s_temblor+48);*/
        if(limpiar_lcd==1){
            lcd_clear();
            limpiar_lcd=0;
        }else
            limpiar_lcd=0;
                
  
    }
    return;
}

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
        lcd_write_char(sens+48);
        
    }else if (modo==2){
        lcd_set_cursor(1, 1);
        lcd_write_string("Temperatura:");
    }else if (modo==3){
        lcd_set_cursor(1, 1);
        lcd_write_string("Humo:");
    }else if (modo==4){
        lcd_set_cursor(1, 1);
        lcd_write_string("Temblor:");
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
                sens++; // INCREMENTO DE CONTADOR
                AR1 = 0; // REGRESO AL BOTON ESTADO INICIAL
                __delay_ms(5); // DELAY POR SI LAS MOSCAS
            }
                
        }
        if (sens==255){
                sens = 0; 
                }
}

//***********Funciones para solicitar o enviar datos a los slaves**************

void slave_1(uint8_t sensor){
    switch(sensor){
        case 1:
            I2C_Master_Start();
            I2C_Master_Write(0x10);      //aqui va la direccion del sensor 1 (el maestro recibe el dato))
            I2C_Master_Write(sensor);
            I2C_Master_Stop();
            __delay_ms(100);
            
            I2C_Master_Start();
            I2C_Master_Write(0x11);
            s_temperatura = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(100);
            break;
        case 2:
            I2C_Master_Start();
            I2C_Master_Write(0x10);      //aqui va la direccion del sensor 1 (el maestro recibe el dato))
            I2C_Master_Write(sensor);
            I2C_Master_Stop();
            __delay_ms(100);
            
            I2C_Master_Start();
            I2C_Master_Write(0x11);
            s_humo = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(100);
            break;
        case 3:
            I2C_Master_Start();
            I2C_Master_Write(0x10);      //aqui va la direccion del sensor 1 (el maestro recibe el dato))
            I2C_Master_Write(sensor);
            I2C_Master_Stop();
            __delay_ms(100);
            
            I2C_Master_Start();
            I2C_Master_Write(0x11);
            s_temblor = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(100);
            break;     
    }
    
}
void slave_2(void){
    
}
void slave_3(void){
    
}