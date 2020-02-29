/* 
 * File:   ADC.h
 * Author: Daniela Baldizon
 *
 * Created on 1 de febrero de 2020, 12:37 AM
 */

#ifndef ADC_H
#define	ADC_H
#include <stdint.h>

#include <xc.h>

void ADC_CONFIG(uint8_t canal, uint8_t justif);

void ADC_INTERRUPT(void);

void ADC_RES(uint8_t AH, uint8_t AL);

#endif	/* ADC_H */

