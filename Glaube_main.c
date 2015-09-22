#define _XTAL_FREQ 32000000
#define USE_AND_MASKS

#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <math.h>
#include <plib/timers.h>
#include <plib/usart.h>
#include <plib/adc.h>
#include "XBeeAPI.h"
#include "Glaube.h"
#include "Utilities.h"
#include "Interrupt_manager.h"



unsigned char Timer0Config;

void SetupInit(void);
void SetupClock(void);
void Setup_USART2XBAPI();
void SetupTimer0(unsigned char T,unsigned char M);
void putch(char data);
void SetupPorts();
void Sekunde();



void main() {

    FlagPaqRx2=0;

    SetupInit();  //Inicializar sensores y cualquier cosa que se tenga que nicializar

    //GB_Init();


    while(1)
    {
        //GetMyAdrs();
        //XBAPI_Tx(CL_ID,1,0x01,0x01,0x58,AdrsGW,AdrsMyAdrs,AdrsGW);  //eJEMPLO DE FUNCIÓN PARA ENVIAR EN MODO API

    }
}



//void interrupt ISR()  //Rutinas de interrupción
//{
//    //Timer 0 Interrupción
//    if(INTCONbits.TMR0IF == 1)
//    {
//        Timer0Flag=1;
//        PORTCbits.RC3 = !PORTCbits.RC3;
//
//        Sekunde();
//        WriteTimer0(Load);    //0.001 Seg. FOSC 32 MHz
//        INTCONbits.TMR0IF = 0;
//
//    }
//
//
//
//    //UART2 Interrupción
////        if (USART2_PIR && USART2_PIE) //Nombres definidos en XBeeAPI.h
//    {
//        BufferRxUART[iRx1XBAPI] = Read2USART();
//
//        if (iRx1XBAPI == 2) // Se recive suficiente información para determinar la longitud del paquete
//            if (BufferRxUART[0] == XBAPI_StrDel) //Identificacion de un paquete API XBee
//                PaqXBAPILen = Make16(BufferRxUART[1], BufferRxUART[2]); //Se obtiene la longitud del paquete esperado
//
//        //Terminación por longitud de paquete esperado
//        if (iRx1XBAPI == PaqXBAPILen + 3) //Si se ha alcanzado el final del paquete esperado
//        {
//            iRx1XBAPI = 0; //Se cierra el paquete
//            NoPaqXBAPI++; //Se aumenta el contador de paquetes recividos
//            FlagPaqRx2 = 1; //Se habilita bandera de nuevo paquete
//
//
//            PIR3bits.RC2IF = 0;   //Experimental
//
//            NewPackUART(PaqXBAPILen);  //PaqXBAPILen = Longitud de paquete - Start Delimiter - Length Bytes - Check Sum
//            //FlagBufferRx1Full=0;      //Se deshabilita bandera de buffer Rx lleno
//            return;
//        }
//
//        iRx1XBAPI++;
//        PIR3bits.RC2IF = 0; // clear rx flag
//
//
//    }
//
//
//}




void SetupTimer0(unsigned char T,unsigned char N)  //Timer para interrupción de cada segundo
{
    switch (T)
    {
        case 1:   //Opción para 1 Segundo
            Timer0Config = TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256 ;
            Load=0x85ED; //1 Seg. FOSC 32 MHz
        break;
        case 2:   //Opcion para 1 ms
            Timer0Config = TIMER_INT_ON & T0_8BIT & T0_SOURCE_INT & T0_PS_1_256 ;
            Load=0xE1;
        break;

    }

    if(T==0)
        CloseTimer0();
    else
    {
        OpenTimer0(Timer0Config);
        WriteTimer0(Load);
        INTCONbits.TMR0IF = 0;
    }

}


void SetupInit()  //Función para inicialización de sensores
{
    SetupClock();
    SetupPorts();
    Setup_USART2XBAPI();
    SetupTimer0(1,0);
    INTERRUPT_Initialize();
    //SetupTimerVOC();
    //InitVOC();
    //Init_ADC_AirQ();
    //LedInitialiserung();


    PEIE = 1; //Enable pheripheral interrupt serial port is a pheripheral)
    INTCONbits.GIE = 1;

}


void SetupClock()
{
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
}



void SetupPorts()  //Configuración de puertos
{
    TRISAbits.RA0=1;    //RA0 as input
    TRISAbits.RA1=1;
    ANSELAbits.ANSA0=1; //RA0 as ADC input
    ANSELAbits.ANSA1=1;

    //ANSELCbits.ANSC3=0;
    //TRISCbits.RC3 = 0;

}


void Sekunde()  //Función que se ejecuta cada segundo
{
   
    XBAPI_Tx(AdrsDummy,8,0x01,0x01,0x7B,AdrsP2P,AdrsMyAdrs,AdrsDummy);

}