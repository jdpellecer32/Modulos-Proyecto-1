/* 
 * File:   USART.h
 * Author: Daniela Baldizon
 *
 * Created on 10 de marzo de 2020, 10:59 PM
 */

#ifndef USART_H
#define	USART_H

void init_usart();
unsigned char recibe_caracter_usart();
void envia_caracter_usart(unsigned char caracter);
void envia_cadena_usart(char* cadena);

#endif	/* USART_H */

