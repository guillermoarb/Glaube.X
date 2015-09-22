#include "Glaube.h"
#include "XBeeAPI.h"

unsigned char AdrsBroadcast[8]={0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF}; //Dirección de broadcasting
unsigned char AdrsMyAdrs[8]={0x0A,0x0A,0x0A,0x0A,0x40,0x9C,0x0A,0x0A};  //Dirección MAC de radio
unsigned char AdrsAbsender[8]={0xFF,0x13,0xA2,0x00,0x40,0x9C,0x57,0xFF};
unsigned char AdrsHerkunft[8]={0xFF,0x13,0xA2,0x00,0x40,0x9C,0x57,0xFF};
unsigned char AdrsReiseziel[8]={0xFF,0x13,0xA2,0x00,0x40,0x9C,0x57,0xFF};
unsigned char AdrsAdressat[8]={0x00,0x13,0xA2,0x00,0x40,0x97,0x21,0xDF};
//unsigned char AdrsGW[8]={0x00,0x13,0xA2,0x00,0x40,0x97,0x21,0xDF};
unsigned char AdrsGW[8]={0x00,0x13,0xA2,0x00,0x40,0xB6,0xB7,0x29};
unsigned char AdrsDummy[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char AdrsP2P[8]={0x00,0x13,0xA2,0x00,0x40,0xB6,0xB7,0x29};             //Direción MAC debajo del XBee 0013A20040B6B729 del receptor
unsigned char RSSI;

//Varaibles de estado
unsigned char CL_ID[1]=0x01;       //Valor de cluster asignado
unsigned char AES_KEY[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x01,0x02,0x03,0x04,0x05,0x06};   //Key para AES 128
unsigned char PAN_ID[2]={0x08,0x08};
unsigned char CH[1]=0x17;
unsigned char NdID=0x01;    //ID para líder de cluster

//Banderas
unsigned char FlagSynchRes=0;

struct Aussaat DataAussaat[10];
struct Melken MelkenNMC[30];


void GB_Melken(unsigned char Data[])
{



    RSSI=Data[RSSI_Pos];

    for(unsigned char i=0;i<8;i++)
    {
        AdrsAbsender[i]=Data[i+AbsenderPos];
        AdrsHerkunft[i]=Data[i+HerkunftPos];
        AdrsReiseziel[i]=Data[i+ReisezielPos];
    }

    XBAPI_Tx(AdrsAbsender,8,0x01,0x01,0x10,AdrsP2P,AdrsDummy,AdrsDummy);
    XBAPI_Tx(AdrsHerkunft,8,0x01,0x01,0x7B,AdrsP2P,AdrsDummy,AdrsDummy);
    XBAPI_Tx(AdrsReiseziel,8,0x01,0x01,0x7B,AdrsP2P,AdrsDummy,AdrsDummy);


    switch (Data[GB_ID_Pos])
    {
        case 0x58:  //GW2NMC_Synch

            for(unsigned char i=0;i<16;i++)
            {
                AES_KEY[i]=Data[i+AES_KEY_Pos];  //Adquisición de AES KEY
            }

            PAN_ID[0]=Data[PAN_ID_Pos];         //Adquisición de PAN ID 1º byte
            PAN_ID[1]=Data[PAN_ID_Pos+1];       //Adquisición de PAN ID 2º byte
            CL_ID[0]= Data[CL_ID_Pos];             //Adquisición de cluster id asignado
            CH[0]=Data[CH_Pos];                    //Adquisición de canal asignado en sincronización

            for(unsigned char i=0;i<8;i++)      //Adquisición de dirección del GW
                AdrsGW[i]=Data[i+AbsenderPos];

            FlagSynchRes=1;

            break;

    }

}

void GB_Init(void)
{
    GetMyAdrs();
    XBAPI_Tx(AdrsMyAdrs,8,0x01,0x01,0x10,AdrsP2P,AdrsDummy,AdrsDummy);
}