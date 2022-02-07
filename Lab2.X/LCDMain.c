/*
  Archivo:  LAB1.c
  Autor:    Alejandro Ramirez Morales
  Creado:   24/enero/22
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include "LCD.h"
#include "EUSART.h"
#define _XTAL_FREQ 1000000

char Mensaje1 [] = " S0:   S1:   S2: ";
uint8_t S0;
uint8_t S1;
uint8_t S2;
uint8_t portEs = 0;

int S0D;
uint8_t S0DC;
uint8_t S0DD;
uint8_t S0DU;
uint8_t centenas0, decenas0, unidades0;

int S1D;
uint8_t S1DC;
uint8_t S1DD;
uint8_t S1DU;
uint8_t centenas1, decenas1, unidades1;

int S2D;
uint8_t S2DC;
uint8_t S2DD;
uint8_t S2DU;
uint8_t centenas2, decenas2, unidades2;

/*
 +----------------------------------------------------------------------------+
 |                             VARIABLES GLOBALES                             |
 +----------------------------------------------------------------------------+
 */

/*
 +----------------------------------------------------------------------------+
 |                          PROTOTIPOS DE FUNCIONES                           |
 +----------------------------------------------------------------------------+
 */
void setup(void);
void divisor(int valor, uint8_t *unidades, uint8_t *decena, uint8_t *centena);
void Map255a500 (uint8_t a, int *b);
uint8_t NumAAscii (uint8_t valor);
void ValoresDisplay(void);
void MensajeLCD(void);
void CambioCanalADC (void);
/*
 +----------------------------------------------------------------------------+
 |                               INTERRUPCIONES                               |
 +----------------------------------------------------------------------------+
 */
void __interrupt() isr (void)
{
    if(PIR1bits.ADIF){
        if(ADCON0bits.CHS == 11)
            S0 = ADRESH;
        else
            S1 = ADRESH;
        PIR1bits.ADIF = 0;
    }
    
    else if(PIR1bits.RCIF){
        portEs = RCREG;
        if(portEs == 43){
            S2 = S2++;
        }
        else if(portEs == 45){
            S2 = S2--;
        }
        else{
            S2 = S2;
        }
    }
}

/*
 +----------------------------------------------------------------------------+
 |                                   LOOP                                     |
 +----------------------------------------------------------------------------+
 */
void main(void) 
{
    setup();            // Se ejecuta funcion setup
    //ADCON0bits.GO = 1;  // El ciclo A/D ocurre. Se resetea sola.
    LCD_Inicar();       // Inicializar LCD
    while(1)
    {
        MensajeLCD();  
        CambioCanalADC();              
        ValoresDisplay();
    }
}

/*
 +----------------------------------------------------------------------------+
 |                            SETUP Y FUNCIONES                               |
 +----------------------------------------------------------------------------+
 */

void CambioCanalADC (void){
    if (ADCON0bits.GO == 0){    // Si sucede la interrupcion
            if(ADCON0bits.CHS == 11)    // Cambio de canal
                ADCON0bits.CHS = 13;    // Si canal 11, pasar a 13
            else
                ADCON0bits.CHS = 11;    // Si no pasar a 11
    
            __delay_us(50);             // Pausa pa cambio de canal
            ADCON0bits.GO = 1;
    }    
}

void Map255a500 (uint8_t a, int *b){
    *b = (int)a*100/51;     // Realiza el mapeo de 255 a 5
    return;
}

void MensajeLCD (void){
    LCD_Set_Cursor(0,0);
    LCD_Imprimir_Texto(Mensaje1);  
    LCD_Set_Cursor(1,0);
    LCD_Imprimir_Char(centenas0); 
    LCD_Imprimir_Char('.');
    LCD_Imprimir_Char(decenas0); 
    LCD_Imprimir_Char(unidades0); 
    LCD_Imprimir_Char('v');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(centenas1); 
    LCD_Imprimir_Char('.');
    LCD_Imprimir_Char(decenas1); 
    LCD_Imprimir_Char(unidades1); 
    LCD_Imprimir_Char('v');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(centenas2); 
    LCD_Imprimir_Char(decenas2); 
    LCD_Imprimir_Char(unidades2); 
}

void divisor(int valor, uint8_t *unidades, uint8_t *decena, uint8_t *centena){
    *unidades = valor%10;           // Modulo da las unidades de /10
    int cociente0 = (int)(valor/10);// Obtener  los primeros dos digitos 
    *decena = cociente0%10;         // Residuo de los primeros dos digitos
    *centena = (int)(cociente0/10); // Primer digito de la division     
    return;
}
void ValoresDisplay(void){
    Map255a500(S0,&S0D);
    divisor(S0D,&S0DU,&S0DD,&S0DC);

    Map255a500(S1,&S1D);
    divisor(S1D,&S1DU,&S1DD,&S1DC);
    
    //Map255a500(S2,&S2D);
    divisor(S2,&S2DU,&S2DD,&S2DC);

    unidades0 = NumAAscii(S0DU);
    decenas0  = NumAAscii(S0DD);
    centenas0 = NumAAscii(S0DC);

    unidades1 = NumAAscii(S1DU);
    decenas1  = NumAAscii(S1DD);
    centenas1 = NumAAscii(S1DC);    
    
    unidades2 = NumAAscii(S2DU);
    decenas2  = NumAAscii(S2DD);
    centenas2 = NumAAscii(S2DC); 
    return;
}

// Convierte de numeros a Ascii
uint8_t NumAAscii (uint8_t valor){
    switch(valor){
        case 0:
            return 48; 
            break;
        case 1:
            return 49;
            break;
        case 2:
            return 50;
            break;
        case 3:
            return 51;
            break;
        case 4:
            return 52;
            break;
        case 5:
            return 53;
            break;
        case 6:
            return 54;
            break;
        case 7:
            return 55;
            break;
        case 8:
            return 56;
            break;
        case 9:
            return 57;
            break;
        default:
            break;  
    }    
}
void setup(void)
{
    // Ports 
    ANSEL   =   0;              // Digital Ports
    ANSELH  =   0b00101000;
    //ANSELH  =   0;
    
    TRISA   =   0;              // PORTA - salida
    TRISB   =   0b00110000;
    //TRISB = 0;
    //TRISC   =   0;              // PORTC - salida
    TRISD   =   0;              // PORTD - salida
    TRISE   =   0;              // PORTE - salida
    
    PORTA   =   0;              // PORTA en 0
    PORTB   =   0; 
    //PORTC   =   0; // Display en 0
    PORTD   =   0;              // PORTD en 0
    PORTE   =   0;              // PORTB en 0
            
    // Reloj
    OSCCONbits.IRCF = 0b100;    // 4MHz
    OSCCONbits.SCS = 1;         // Activar reloj interno
    
    EUSART_C(0,1,1,25,0,1,0,1,1,0);
    
    /*
    // 9600 baudios
    // Configuracion de TX y RX
    TXSTAbits.SYNC = 0;  // Asincronico
    TXSTAbits.BRGH = 1;  // High speed 
    BAUDCTLbits.BRG16 = 1; // Generador de 16 bits
    
    // SPBRGH:SPBRG = 25
    SPBRG  = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1; // Serial port enable RX/DT y TX/CK
    RCSTAbits.RX9  = 0; // Recibir 8 bits
    RCSTAbits.CREN = 1; // Activar recepcion
    
    TXSTAbits.TXEN = 1; // Activa trasmi
    TXSTAbits.TX9      = 0;
    */
    
    // Interrupciones
    INTCONbits.GIE  = 1;        // Interrupciones globales activadas
    INTCONbits.PEIE = 1;        // Interrupciones perifericas activadas
    INTCONbits.RBIE = 0;        // Interrupion del puerto b activada
    INTCONbits.RBIF = 0;        // Bandera interrupcion portb apagada
    PIE1bits.TMR1IE = 0;        // Interrupcion del TMR1
    PIR1bits.TMR1IF = 0;        // Bandera TMR1 apagada    
    PIE1bits.ADIE = 1;      // Activa la interrupción de ADC
    PIR1bits.ADIF = 0;      // A/D conversion no ha empezado o completado
    PIR1bits.RCIF   = 0;
    PIE1bits.RCIE   = 1;
        
    // Configuración del ADC
    ADCON1bits.ADFM  = 0;   // Justificado a la izquierda
    ADCON1bits.VCFG1 = 0;   // Referencia como tierra
    ADCON1bits.VCFG0 = 0;   // Referencia poder
    
    ADCON0bits.ADCS = 0b01; // Fosc/8
    ADCON0bits.CHS  = 11;    // Ansel 11
    ADCON0bits.ADON = 1;    // ADC activo
    __delay_us(50);
    
    return;   
}