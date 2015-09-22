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