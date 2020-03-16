/*
 * File:   stepperlib.c
 * Author: David Vela
 *
 * Created on 26 de febrero de 2020, 04:17 PM
 * Funciones extra�das de: 
 * https://circuitdigest.com/microcontroller-projects/interfacing-stepper-motor-with-pic16f877a
 */



#include <xc.h>
#include "stepperlib.h"
 
//Funci�n para un full drive (dos bobinas por paso))
void full_drive (char direction){
    if (direction == anti_clockwise){
        PORTB = 0b00000011;
        ms_delay(speed);
        PORTB = 0b00000110;
        ms_delay(speed);
        PORTB = 0b00001100;
        ms_delay(speed);
        PORTB = 0b00001001;
        ms_delay(speed);
        PORTB = 0b00000011;
        ms_delay(speed);
    }
    if (direction == clockwise){
        PORTB = 0b00001001;
        ms_delay(speed);
        PORTB = 0b00001100;
        ms_delay(speed);
        PORTB = 0b00000110;
        ms_delay(speed);
        PORTB = 0b00000011;
        ms_delay(speed);
        PORTB = 0b00001001;
        ms_delay(speed);
    }
        
}
 
/* This method will drive the motor in half-drive mode using direction input */
 //Funci�n para un half drive (intercala entre 1 y 2 bobinas encendias))
void half_drive (char direction){
    if (direction == anti_clockwise){
        PORTB = 0b00000001;
        ms_delay(speed);
        PORTB = 0b00000011;
        ms_delay(speed);
        PORTB = 0b00000010;
        ms_delay(speed);
        PORTB = 0b00000110;
        ms_delay(speed);
        PORTB = 0b00000100;
        ms_delay(speed);
        PORTB = 0b00001100;
        ms_delay(speed);
        PORTB = 0b00001000;
        ms_delay(speed);
        PORTB = 0b00001001;
        ms_delay(speed);
    }
    if (direction == clockwise){
       PORTB = 0b00001001;
       ms_delay(speed);
       PORTB = 0b00001000;
       ms_delay(speed);
       PORTB = 0b00001100;
       ms_delay(speed); 
       PORTB = 0b00000100;
       ms_delay(speed);
       PORTB = 0b00000110;
       ms_delay(speed);
       PORTB = 0b00000010;
       ms_delay(speed);
       PORTB = 0b00000011;
       ms_delay(speed);
       PORTB = 0b00000001;
       ms_delay(speed);
    }
}
 
//Funci�n para un wavedrive (una bobina por paso))
void wave_drive (char direction){
    if (direction == anti_clockwise){
        PORTB = 0b00000001;
        ms_delay(speed);
        PORTB = 0b00000010;
        ms_delay(speed);
        PORTB = 0b00000100;
        ms_delay(speed);
        PORTB = 0b00001000;
        ms_delay(speed);
    }
     if (direction == clockwise){
        PORTB = 0b00001000;
        ms_delay(speed);
        PORTB = 0b00000100;
        ms_delay(speed);
        PORTB = 0b00000010;
        ms_delay(speed);
        PORTB = 0b00000001;
        ms_delay(speed);
    }
    
}
//Funci�n para establecer el delay entre pasos
void ms_delay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<250;j++);
}
