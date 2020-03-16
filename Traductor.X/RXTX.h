/* 
 * File:   RXTX.h
 * Author: Daniela Baldizon
 *
 * Created on 8 de febrero de 2020, 02:56 PM
 */

#ifndef RXTX_H
#define	RXTX_H
#include <stdint.h>
#include <xc.h>

char UART_Init(const long int baudrate);
void UART_Write(char data);
char UART_TX_Empty();
char UART_Data_Ready();
char UART_Read();
void UART_Read_Text(char *Output, unsigned int length);

#endif	/* RXTX_H */

