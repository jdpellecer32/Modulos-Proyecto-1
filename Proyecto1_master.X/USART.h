/* 
 * File:   USART.h
 * Author: José Pellecer
 *
 * Created on February 7, 2020, 9:55 AM
 */

#ifndef USART_H
#define	USART_H

void init_usart();
unsigned char recibe_caracter_usart();
void envia_caracter_usart(unsigned char caracter);
void envia_cadena_usart(char* cadena);

#endif	/* USART_H */

