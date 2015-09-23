//Definición de variables
#include <xc.h>
#include "XBeeAPI.h"
#include <stdio.h>
#include <plib/usart.h>
#include "Glaube.h"

#define _XTAL_FREQ 32000000

unsigned int Length;
unsigned char ApiID;
unsigned char ApiOp;
unsigned char Pmtr[32];
unsigned char AT_Resp=0xFF;

//VARIABLES GLOBALES
unsigned char BufferRxUART[127];
unsigned char FlagPaqRx2=0;
unsigned char iRx1XBAPI=0;
unsigned int PaqXBAPILen=0;
unsigned int NoPaqXBAPI=0;


void UART_XBeeAPI_ISR(void)
{
        BufferRxUART[iRx1XBAPI] = Read2USART();

        if (iRx1XBAPI == 2) // Se recive suficiente información para determinar la longitud del paquete
            if (BufferRxUART[0] == XBAPI_StrDel) //Identificacion de un paquete API XBee
                PaqXBAPILen = Make16(BufferRxUART[1], BufferRxUART[2]); //Se obtiene la longitud del paquete esperado

        //Terminación por longitud de paquete esperado
        if (iRx1XBAPI >= PaqXBAPILen + 3) //Si se ha alcanzado el final del paquete esperado
        {
            iRx1XBAPI = 0; //Se cierra el paquete
            NoPaqXBAPI++; //Se aumenta el contador de paquetes recividos
            FlagPaqRx2 = 1; //Se habilita bandera de nuevo paquete


            //PIR3bits.RC2IF = 0;   //Experimental

            //NewPackUART(PaqXBAPILen);  //PaqXBAPILen = Longitud de paquete - Start Delimiter - Length Bytes - Check Sum
            //FlagBufferRx1Full=0;      //Se deshabilita bandera de buffer Rx lleno
            
           
            return ;
        }

        iRx1XBAPI++;
        //PIR3bits.RC2IF = 0; // clear rx flag

}


void XBAPI_Tx(unsigned char Data[], unsigned char Length, unsigned char FrameID,unsigned char FrameOp,unsigned char GB_ID, unsigned char AdrsAdressat[], unsigned char AdrsHerkunft[], unsigned char AdrsReiseziel[])
{
    unsigned char   BufferTxXBAPI[XBAPI_BuffLen];

    Length+=17; //Ajuste de longitud por direcciones y Glaube Options

    BufferTxXBAPI[0]=0x7E;
    BufferTxXBAPI[1]=0x00;          // La longitud no supera 255 bytes
    BufferTxXBAPI[2]=Length+11;     //Longitud de los datos Data Length+11 (Address Length + API ID Len+FrameID Len+ Option Len)
    BufferTxXBAPI[3]=0x00;          //Identificador de envio para direccion de 64 Bits... Pag 61.
    BufferTxXBAPI[4]=FrameID;
    BufferTxXBAPI[13]=FrameOp;      //Options
    BufferTxXBAPI[30]=GB_ID;

    for(unsigned char i=0;i<=7;i++)
    {
        BufferTxXBAPI[i+5]=AdrsAdressat[i];        //Direccion destino inmediato
        BufferTxXBAPI[i+14]=AdrsHerkunft[i];       //Direccion de origen
        BufferTxXBAPI[i+22]=AdrsReiseziel[i];      //Direccion de destino final
    }

    for(unsigned char i=0;i<=Length;i++)
    {
        //BufferTxXBAPI[i+30]=Data[i];
        BufferTxXBAPI[i+31]=Data[i];
    }

    //CheckSum
    //BufferTxXBAPI[26]=0xF2;
    BufferTxXBAPI[Length+14]=ChecksumGen(BufferTxXBAPI);

    for(unsigned char i=0;i<=Length+14;i++)
    {
        while(!TXSTA2bits.TRMT);
        //Write1USART(BufferTx[i]);
        Write2USART(BufferTxXBAPI[i]);
    }

}


int Make16 (char MSB, char LSB)
{
    int Maked16=0;

    Maked16=(MSB<<8)+LSB;


    return Maked16;
}

char Make8(int Var, int NoByte)
{
    //Función para separar una variable de 16 bits en dos de 8 bits
    //Var= Variable de 16 bits a ser separada
    //NoByte = 1,2 Selecciona el byte que será retornado, 1=MSB, 2=LSB

    unsigned char Maked8=0;

    if(NoByte==2)
    Maked8=Var & 0xFF;
    if(NoByte==1)
    Maked8=Var>>8;

    return Maked8;
}



void Setup_USART1XBAPI()
{
    //Declaración de Variables
    unsigned char USART1Config=0;
    unsigned int BaudRate=0;

    TRISCbits.RC6=0;        //TX es salida
    TRISCbits.RC7=1;        //RX es entrada

    //Configuración USART
    USART1Config=USART_TX_INT_OFF   //Interrupcion por Ttransmisión: Off
            &USART_RX_INT_OFF       //Interrupcion por Recepción: Off
            &USART_ASYNCH_MODE      //Modo Asíncrono
            &USART_EIGHT_BIT        //Transmision de 8bits
            &USART_CONT_RX          //Recepción Continua
            &USART_BRGH_HIGH        //Baudios
            &USART_ADDEN_OFF;       //Detección de Dirección OFF
    BaudRate=51;            //Valor que se carga a SPRBH para Definir BaudRate = 9600
    Close1USART();          //Cierra USART2 en caso de estar previamente abierto.
    Open1USART(USART1Config, BaudRate); //Abre USART1

    IPR1bits.RC1IP =1;      //Receive Interrupt: High Priority
    PIE1bits.RC1IE=1;       //Receive Interrupt: Enabled
    PIR1bits.RC1IF=0;       //Reset de EUSART2 Receive Interrupt Flag
}


void putch(char data)
{
    while(!TX2IF)
    continue;
    TXREG2 = data;
//
//    while(!TX1IF)
//    continue;
//    TXREG1 = data;

}



char ChecksumGen(unsigned char Frame[])
{
    int Checksum=0,IndS=0,Len;
    unsigned int Sum=0;

    Len=Make16(Frame[1],Frame[2]);

    for(IndS=3;IndS<=Len+2;IndS++) //Checksum comienza despues de longitud y hasta el final.
    {
        Sum=Sum+Frame[IndS];
    }

    Checksum=Sum & 0xFF;
    Checksum=0xFF-Sum;

    return Checksum;
}


void Setup_USART2XBAPI()
{
    //Declaración de Variables
    unsigned char   USART2Config=0;
    unsigned int    BaudRate=0;

    TRISBbits.RB6=0;        //TX es salida   //cambiar estos son de USAR1
    TRISBbits.RB7=1;        //RX es entrada

    //Configuración USART
    USART2Config=USART_TX_INT_OFF   //Interrupcion por Ttransmisión: Off
            &USART_RX_INT_OFF       //Interrupcion por Recepción: Off
            &USART_ASYNCH_MODE      //Modo Asíncrono
            &USART_EIGHT_BIT        //Transmision de 8bits
            &USART_CONT_RX          //Recepción Continua
            &USART_BRGH_HIGH        //Baudios
            &USART_ADDEN_OFF;       //Detección de Dirección OFF
    //BaudRate=51;            //Valor que se carga a SPRBH para Definir BaudRate = 9600
    //BaudRate=25;      //19200 8 mHZ
    //BaudRate=103;     //19200 32 mHz
    BaudRate=51;        //38400 32 MHz
    Close2USART();          //Cierra USART2 en caso de estar previamente abierto.
    Open2USART(USART2Config, BaudRate); //Abre USART2
    IPR3bits.RC2IP=1;       //Receive Interrupt: High Priority
    PIE3bits.RC2IE=1;       //Receive Interrupt: Enabled
    PIR3bits.RC2IF=0;       //Reset de EUSART2 Receive Interrupt Flag
}


void XBAPI_ATtx(unsigned char Length, unsigned char FrID, unsigned char AT1,unsigned char AT2,unsigned char Parameter[])
{
    unsigned char   BufferTxXBAPI[40];

    BufferTxXBAPI[0]=0x7E;
    BufferTxXBAPI[1]=0x00;
    BufferTxXBAPI[2]=Length;
    BufferTxXBAPI[3]=0x08;
    BufferTxXBAPI[4]=FrID;
    BufferTxXBAPI[5]=AT1;
    BufferTxXBAPI[6]=AT2;

    for(unsigned char i=0;i<Length-4;i++)
        BufferTxXBAPI[7+i]=Parameter[i];

    BufferTxXBAPI[Length+3]=ChecksumGen(BufferTxXBAPI);

    for(unsigned char i=0;i<=Length+3;i++)
    {
        while(!TXSTA2bits.TRMT);
        Write2USART(BufferTxXBAPI[i]);
    }
}

void XBAPI_ATrx(unsigned char Daten[])
{
    if (Daten[5]==0x53 && Daten[6]==0x48)
    {

        for(unsigned char i=0;i<=4;i++)
            AdrsMyAdrs[i]=Daten[i+8];

        XBAPI_Tx(AdrsMyAdrs,8,0x01,0x01,0x10,AdrsP2P,AdrsDummy,AdrsDummy);  //Auxiliar
        AT_Resp=0;
    }

    if (Daten[5]==0x53 && Daten[6]==0x4C)
    {
        for(unsigned char i=0;i<=4;i++)
            AdrsMyAdrs[i+4]=Daten[i+8];

        XBAPI_Tx(AdrsMyAdrs,8,0x01,0x01,0x10,AdrsP2P,AdrsDummy,AdrsDummy);  //Auxiliar
        AT_Resp=0;
    }

    if(Daten[5]==0x49 && Daten[6]==0x44)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

    if(Daten[5]==0x4B && Daten[6]==0x59)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

    if(Daten[5]==0x45 && Daten[6]==0x45)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

    if(Daten[5]==0x43 && Daten[6]==0x48)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

    if(Daten[5]==0x46 && Daten[6]==0x52)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

    if(Daten[5]==0x57 && Daten[6]==0x52)
    {
        if(Daten[7]==0x00)
        {
            AT_Resp=0;
        }
    }

}

void XBAPI_MdmSts(unsigned char Daten[])
{
    if (Daten[4]==0x01)
    {
        AT_Resp=0x00;
    }


}


void XBAPI_Melken(unsigned char Data[],char *AdrsAbsender, char *AdrsReiseziel, char *AdrsHerkunft, char *RSSI, char *ApiID, char *ApiOp, int *Length)
{
    *Length=Make16(Data[1],Data[2]);
    *ApiID=Data[3];
    *RSSI=Data[12];
    *ApiOp=Data[13];


    for(unsigned char i=0;i<8;i++)
    {
        AdrsAbsender[i]=Data[i+4];
        AdrsHerkunft[i]=Data[i+14];
        AdrsReiseziel[i]=Data[i+22];
    }

}


void DelayNms(unsigned int Ciclos)
{
    for(unsigned int i=0;i<Ciclos;i++)
    __delay_ms(10);
}

void NewPackUART(unsigned int RxLen)
{


            if(BufferRxUART[3] == 0x80)  //Paquete Rx normal
            {

              GB_Melken(BufferRxUART,RxLen);

            }

            if(BufferRxUART[3] == 0x88)  //Respuesta comando AT
            {

               XBAPI_ATrx(BufferRxUART);
            }

            if(BufferRxUART[3]==0x8A)
            {

                XBAPI_MdmSts(BufferRxUART);
            }

}

void GetMyAdrs(void)
{

    XBAPI_ATtx(0x04,0x01,'S','H',Pmtr);

    Wait4AT_Resp(0x00);
    //DelayNms(500);
    XBAPI_ATtx(0x04,0x01,'S','L',Pmtr);
    //DelayNms(500);
    Wait4AT_Resp(0x00);


}

void XB_Reset(void)
{
    XBAPI_ATtx(0x04,0x01,'F','R',Pmtr);
    Wait4AT_Resp(0x00);
}

void SetPAN_ID(unsigned char PAN_ID[])
{
    XBAPI_ATtx(0x06,0x01,'I','D',PAN_ID);

    Wait4AT_Resp(0x00);
}

void Wait4AT_Resp(unsigned char Resp)
{
    unsigned char Del4Ans=0;

    while(1)
    {
        if(FlagPaqRx2==1)
        {
            NewPackUART(0x00);
            FlagPaqRx2=0;
        }

        if(AT_Resp==Resp)
        {
            AT_Resp=0xFF;
            break;
        }

        if(Del4Ans==100)
        {
            AT_Resp=0xEE;
            break;
        }

        DelayNms(30);
        Del4Ans++;

    }
}

void SetAES_Key(unsigned char Key[])
{
    XBAPI_ATtx(0x14,0x01,'K','Y',Key);
    Wait4AT_Resp(0x00);
}

void AES_EE(unsigned char En)
{
    unsigned char Aux[1];
    Aux[0]=En;
    XBAPI_ATtx(0x05,0x01,'E','E',Aux);
    Wait4AT_Resp(0x00);
}

void SetCH(unsigned char CH[])
{
    XBAPI_ATtx(0x05,0x01,'C','H',CH);
    Wait4AT_Resp(0x00);
}

void XBAPI_WR(void)
{
    XBAPI_ATtx(0x04,0x01,'W','R',Pmtr);
    Wait4AT_Resp(0x00);
}