#include "EUSART.h"

void EUSART_C ( uint8_t sinc, uint8_t brgh, uint8_t brg16, uint8_t spbrg, 
                uint8_t spbrgh, uint8_t spen, uint8_t rx9, uint8_t cren,
                uint8_t txen, uint8_t tx9){
    // 9600 baudios
    // Configuracion de TX y RX
    TXSTAbits.SYNC = sinc;  // Asincronico
    TXSTAbits.BRGH = brgh;  // High speed 
    BAUDCTLbits.BRG16 = brg16; // Generador de 16 bits
    
    // SPBRGH:SPBRG = 25
    SPBRG  = spbrg;
    SPBRGH = spbrgh;
    
    RCSTAbits.SPEN = spen; // Serial port enable RX/DT y TX/CK
    RCSTAbits.RX9  = rx9; // Recibir 8 bits
    RCSTAbits.CREN = cren; // Activar recepcion
    
    TXSTAbits.TXEN = txen; // Activa trasmi
    TXSTAbits.TX9  = tx9;   
}