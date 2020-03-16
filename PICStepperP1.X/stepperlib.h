/* 
 * File:   stepperlib.h
 * Definición de las funciones para el manejo del stepper extraído de: 
 * https://circuitdigest.com/microcontroller-projects/interfacing-stepper-motor-with-pic16f877a
 * 
 * Arreglos realizados por: David Vela
 *
 * Created on 26 de febrero de 2020, 04:16 PM
 */

#ifndef STEPPERLIB_H
#define	STEPPERLIB_H
#define _XTAL_FREQ 8000000
#define speed 1 // Rango de velocidad; 1 más rápido; 10 más lento
#define steps (512*2) //Cantidad de pasos que dará
#define clockwise 0 // Macro para direccion CW
#define anti_clockwise 1 //Macro para direccion ACW
#include <xc.h> 
#include <pic16f887.h>
#include <stdint.h>

void full_drive (char direction); 
void half_drive (char direction); 
void wave_drive (char direction);
void ms_delay(unsigned int val);

#endif	/* STEPPERLIB_H */

