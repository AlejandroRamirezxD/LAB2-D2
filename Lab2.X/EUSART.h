
/*
 +----------------------------------------------------------------------------+
 | La función te permite agregar un valor al FOSC y ELEGIR UN ANSEL.          |
 +----------------------------------------------------------------------------+
 */
// No olvidar apagar la bandera y agregar el delay

#ifndef EUSART_H
#define	EUSART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void EUSART_C ( uint8_t sinc, uint8_t brgh, uint8_t BRG16, uint8_t spbrg, 
                uint8_t spbrgh, uint8_t spen, uint8_t rx9, uint8_t cren,
                uint8_t txen, uint8_t tx9);


#endif	/* EUSART_H */ 

