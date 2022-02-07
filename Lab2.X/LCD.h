
/*
 +----------------------------------------------------------------------------+
 | La función te permite agregar un valor al FOSC y ELEGIR UN ANSEL.          |
 +----------------------------------------------------------------------------+
 */
// No olvidar apagar la bandera y agregar el delay

#ifndef LCD_H
#define	LCD_H

#define LCD_E     PORTBbits.RB7 
#define LCD_RS    PORTEbits.RE0 
#define BUS       PORTA
#define E_delay 0.5

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <string.h> //Para el tamaño del array de chars

#define _XTAL_FREQ 1000000

void LCD_Inicar (void);
void LCD_Instruccion (uint8_t Comando); 
void LCD_Clock_I (void);
void LCD_Imprimir_Char(uint8_t Comando);
void LCD_Limpiar (void);
void LCD_Set_Cursor (unsigned char a,unsigned char b);
void LCD_Imprimir_Texto(char a[]);
#endif	/* LCD_H */ 

