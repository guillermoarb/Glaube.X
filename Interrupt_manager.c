
#include <xc.h>
#include "XBeeAPI.h"
#include "Utilities.h"

void INTERRUPT_Initialize(void) {

    /* Interrup flags for Timer */
    INTCON2bits.TMR0IP = 0;

    /*Interrupt flags for UART CONTROL*/
    IPR1bits.RC1IP = 1; //Receive Interrupt: High Priority
    PIE1bits.RC1IE = 1; //Receive Interrupt: Enabled
    PIR1bits.RC1IF = 0; //Reset de EUSART2 Receive Interrupt Flag

    PEIE = 1;               //Global interrupts
    INTCONbits.GIE = 1;
}

void interrupt ISR() //Rutinas de interrupción
{
    //Timer 0 Interrupción
    if (INTCONbits.TMR0IF == 1) {
        TMR0_Glaube_ISR();

    }



    //UART2 Interrupción
    if (USART2_PIR && USART2_PIE) //Nombres definidos en XBeeAPI.h
    {
        UART_XBeeAPI_ISR();
        PIR3bits.RC2IF = 0;

    }


}





/**
 End of File
 */