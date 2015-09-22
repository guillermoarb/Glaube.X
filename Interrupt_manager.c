
#include <xc.h>
#include "XBeeAPI.h"
#include "Utilities.h"


void  INTERRUPT_Initialize (void)
{
    // Disable Interrupt Priority Vectors (16CXXX Compatibility Mode)
    

    // Clear peripheral interrupt priority bits (default reset value)

    // RCI
//    IPR3bits.RC2IP = 0;
//    // TXI
//    IPR3bits.TX2IP = 0;
    // TMRI
    INTCON2bits.TMR0IP = 0;

    /*Interrupt flags for UART CONTROL*/
    IPR1bits.RC1IP =1;      //Receive Interrupt: High Priority
    PIE1bits.RC1IE=1;       //Receive Interrupt: Enabled
    PIR1bits.RC1IF=0;       //Reset de EUSART2 Receive Interrupt Flag
}

//void interrupt low_priority INTERRUPT_InterruptManager (void)
//{
//
//
//    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
//    {
//        TMR0_Glaube_ISR();
//    }
//    else
//    {
//        //Unhandled Interrupt
//    }
//}
//
//
//void interrupt INTERRUPT_InterruptManager2 (void)
//{
//
//   // interrupt handler
//     if (USART2_PIR && USART2_PIE)
//    {
//        UART_XBeeAPI_ISR();
//    }
//}




void interrupt ISR()  //Rutinas de interrupción
{
    //Timer 0 Interrupción
    if(INTCONbits.TMR0IF == 1)
    {
        Timer0Flag=1;
        PORTCbits.RC3 = !PORTCbits.RC3;

        Sekunde();
        WriteTimer0(Load);    //0.001 Seg. FOSC 32 MHz
        INTCONbits.TMR0IF = 0;

    }



    //UART2 Interrupción
    if (USART2_PIR && USART2_PIE) //Nombres definidos en XBeeAPI.h
    {
        BufferRxUART[iRx1XBAPI] = Read2USART();

        if (iRx1XBAPI == 2) // Se recive suficiente información para determinar la longitud del paquete
            if (BufferRxUART[0] == XBAPI_StrDel) //Identificacion de un paquete API XBee
                PaqXBAPILen = Make16(BufferRxUART[1], BufferRxUART[2]); //Se obtiene la longitud del paquete esperado

        //Terminación por longitud de paquete esperado
        if (iRx1XBAPI == PaqXBAPILen + 3) //Si se ha alcanzado el final del paquete esperado
        {
            iRx1XBAPI = 0; //Se cierra el paquete
            NoPaqXBAPI++; //Se aumenta el contador de paquetes recividos
            FlagPaqRx2 = 1; //Se habilita bandera de nuevo paquete


            PIR3bits.RC2IF = 0;   //Experimental

            NewPackUART(PaqXBAPILen);  //PaqXBAPILen = Longitud de paquete - Start Delimiter - Length Bytes - Check Sum
            //FlagBufferRx1Full=0;      //Se deshabilita bandera de buffer Rx lleno
            return;
        }

        iRx1XBAPI++;
        PIR3bits.RC2IF = 0; // clear rx flag


    }


}





/**
 End of File
*/