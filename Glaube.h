/* 
 * File:   Glaube.h
 * Author: GuillermoARB
 *
 * Created on 6 de mayo de 2015, 12:57
 */

#ifndef GLAUBE_H
#define	GLAUBE_H

#ifdef	__cplusplus
extern "C" {
#endif

//Definición de posiciones
#define AbsenderPos 4
#define RSSI_Pos 12
#define HerkunftPos 14
#define ReisezielPos 22
#define GB_ID_Pos 30
#define GB_Data_Pos 31

//Posiciones Paquete Synch
#define CL_ID_Pos 31
#define PAN_ID_Pos 32
#define AES_KEY_Pos 34
#define CH_Pos 50


//Estructuras para Glaube
struct Aussaat {
    unsigned char Adressat[8];
    unsigned char PW;
    unsigned char S_GW;
    unsigned char PwGW;
};

extern struct Aussaat DataAussaat[10];

struct Melken {
    unsigned char Absender[8];
    unsigned char Reiseziel[8];
    unsigned char Herkunft[8];
    unsigned char RSSI;
    unsigned char PW2NMC;
    unsigned int  S2NMC;
};

extern struct Melken MelkenNMC[30];

extern unsigned char AdrsBroadcast[8]; //Dirección de broadcasting
extern unsigned char AdrsMyAdrs[8];    //Dirección MAC de radio
extern unsigned char AdrsGW[8];        //Dirección del GW
extern unsigned char AdrsAbsender[8];  //Dirección remitente
extern unsigned char AdrsHerkunft[8];
extern unsigned char AdrsReiseziel[8];
extern unsigned char AdrsAdressat[8];
extern unsigned char AdrsDummy[8];
extern unsigned char AdrsP2P[8];

//Varaibles de estado
extern unsigned char CL_ID[1];         //Valor de cluster asignado
extern unsigned char AES_KEY[16];   //Key para AES 128
extern unsigned char PAN_ID[2];     //Variable para PAN ID de red.
extern unsigned char CH[1];            //Variable de canal
extern unsigned char NdID;          //Identificador para tipo de dispositvo

//Banderas
extern unsigned char FlagSynchRes; // 0.- No sincronizado ... 1.- Solicitud de sincornización

//Variables de Ordeña
extern unsigned char    RSSI;
extern unsigned char    GlaubeOut[80];          //Vector donde es descargada la información para capas superiores.


//Funciones
void GB_Melken(unsigned char Data[], unsigned char GB_Len);
/**
 * @Param
    Data[]  Cadena de información recivida por el puerto serial
    Len     Longitud de la cadena recivida
 * @Returns
    none
 * @Descripción
    Función para ordeña de paquete recivido por puerto serial de acuerdo a protocolo glaube.
 * @Example
    GB_Melken(BufferRx,RxLen);

    Donde BufferRx es el vector donde se almaceno la información recivda por el puerto serial y Rx Len la longitud de dicha información.
 */

void GW_Suche(void);
void GW_SynchReq(unsigned char X);
void GB_Init(void);                         //Inicialización de Glaube




#ifdef	__cplusplus
}
#endif

#endif	/* GLAUBE_H */


//    unsigned char AdrsID[8];
//    //unsigned char AdrsAdressat[8]={0x00,0x13,0xA2,0x00,0x40,0x9C,0x57,0xC5}; //Dirección destinatario/destino inmediato  0013A200409C57C5
//    unsigned char AdrsAdressat[8]={0x00,0x13,0xA2,0x00,0x40,0x97,0x21,0xDF};   //0013A200409721DF
//    unsigned char AdrsAbsender[8]={0x00,0x13,0xA2,0x00,0x40,0xA7,0x11,0xD9};   //Dirección remitente/Origen inmediato
//    unsigned char AdrsReiseziel[8]={0x00,0x13,0xA2,0x00,0x40,0xA7,0x11,0xF5};  //Dirección destino final
//    unsigned char AdrsHerkunft[8]={0x00,0x13,0xA2,0x00,0x40,0xA7,0x11,0xD9};   //Dirección de origen cero