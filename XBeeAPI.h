/*
 * File:   XBeeAPI.h
 * Author: GuillermoAdrián
 *
 * Created on 4 de noviembre de 2014, 03:25 PM
 */

#ifndef XBEEAPI_H
#define	XBEEAPI_H



//Configuración
#define UART2_Baudrate  51
#define XBAPI_BuffLen   127
#define AdrsSize        64
#define USART2_PIR PIR3bits.RC2IF
#define USART2_PIE PIE3bits.RC2IE
#define USART1_PIR PIR1bits.RC1IF
#define USART1_PIE PIE1bits.RC1IE


//Identificadors recervados  por XBee API
#define XBAPI_StrDel        0x7E        //Delimitador de inicio en modo API de XBee
#define XBAPI_Rx64ID        0x80        //Rx API Identifier



//Posiciones estandar en Buffer para trama API
//#define XBAPI_Start     BufferRxXBAPI[0]
//#define XBAPI_LenMSB    BufferRxXBAPI[1]
//#define XBAPI_LenLSB    BufferRxXBAPI[2]
#define XBAPI_FramTyp   BufferRxXBAPI[3]

//#define XBAPI_SouAdrsSt BufferRxXBAPI[4]
#define XBAPI_RSSI      BufferRxXBAPI[12]
#define XBAPI_Opt       BufferRxXBAPI[13]
#define XBAPI_DataSt    BufferRxXBAPI[14]

//Definición de variables
extern unsigned int Length;
extern unsigned char ApiID;
extern unsigned char ApiOp;
extern unsigned char iRx1XBAPI;
extern unsigned int PaqXBAPILen;
extern unsigned int NoPaqXBAPI;

//Variables manejo de puerto serial
extern unsigned char FlagPaqRx2=0;
extern unsigned char BufferRxUART[127];

//Variables comandos AT
extern unsigned char Pmtr[32];  //Parametro para comando AT
extern unsigned char AT_Resp;


//Definición de funciones

void XBAPI_PaqRepEcho(char Term, unsigned char Buffer[]); // Term='T'-> Datos enviados a una terminal serial / Term='X' Datos enviados a otro XBee en modo API
char ChecksumGen(unsigned char Frame[]);
int  Make16 (char MSB, char LSB);
char Make8(int Var, int NoByte);
void Setup_USART2XBAPI();
void putch(char data);
void XBAPI_Tx(unsigned char Data[], unsigned char Length, unsigned char FrameID,unsigned char FrameOp, unsigned char GB_ID, unsigned char AdrsReiseziel[], unsigned char Herkunft[], unsigned char AdrsAdressat[]);
void XBAPI_Rx(unsigned char Data[]);
void XBAPI_Melken(unsigned char Data[],char *AdrsAbsender, char *AdrsReiseziel, char *AdrsHerkunft, char *RSSI, char *ApiID, char *ApiOp, int *Length);
void XBAPI_ATtx(unsigned char Length, unsigned char FrID, unsigned char AT1,unsigned char AT2, unsigned char Parameter[]);
void XBAPI_ATrx(unsigned char Daten[]);
void XBAPI_MdmSts(unsigned char Daten[]);
void DelayNms(unsigned int);
void NewPackUART(unsigned int);
void GetMyAdrs(void);                   //Obtención de dirección MAC
void XB_Reset(void);
void SetPAN_ID(unsigned char PAN_ID[]);
void SetAES_Key(unsigned char AES_Key[]);   //Set AES KEY ... 16 Hex key
void SetCH(unsigned char CH[]);
void AES_EE(unsigned char AES_Sts);         //1-Enable AES 0- Disable AES
void XBAPI_WR(void);
void Wait4AT_Resp(unsigned char Resp); //Espera por respuesta de comando AT API
void UART_XBeeAPI_ISR(void);




#endif	/* XBEEAPI_H */