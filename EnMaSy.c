
#include "XBeeAPI.h"
#include "AirQ.h"
#include "EnMaSy.h"
#include <math.h>
#include "LedLAMP.h"
#include "Glaube.h"

void MonPerSend(double Temp, double Hum, unsigned char Luft,unsigned int Co, unsigned int Lux, unsigned int Leistung, unsigned char AdrsAdressat[], unsigned char AdrsHerkunft[], unsigned char AdrsReisezeil[])
{
    double TempE;


    unsigned char ZeitlichData[11];

    char TempINT = (unsigned char)Temp;
    //char TempDEC = ((unsigned char)(Temp*1000)%1000);
    //char TempDEC = (unsigned char) (Temp-TempINT)*1000;
    unsigned char TempDEC = (unsigned char) (modf(Temp,&TempE)*100);

    //ZeitlichData[0]=0x7B;
    ZeitlichData[0]=TempINT;
    ZeitlichData[1]=TempDEC;
    ZeitlichData[2]= (unsigned char) Hum;
    ZeitlichData[3]=Luft;
    ZeitlichData[4]=Make8(Co,1);
    ZeitlichData[5]=Make8(Co,2);
    ZeitlichData[6]=Make8(Lux,1);
    ZeitlichData[7]=Make8(Lux,2);
    ZeitlichData[8]=Make8(Leistung,1);
    ZeitlichData[9]=Make8(Leistung,2);


    XBAPI_Tx(ZeitlichData,10,0x01,0x01,0x7B,AdrsGW,AdrsMyAdrs,AdrsGW);

}


void SimadePack(double Temp, double Hum, unsigned int VOC,unsigned int Co, unsigned int Ax, unsigned int Ay, unsigned int Az, unsigned char Adrs[])
{
    double TempE;


    unsigned char Data[13];

    char TempINT = (unsigned char)Temp;
    //char TempDEC = ((unsigned char)(Temp*1000)%1000);
    //char TempDEC = (unsigned char) (Temp-TempINT)*1000;
    unsigned char TempDEC = (unsigned char) (modf(Temp,&TempE)*100);


    Data[0]=TempINT;
    Data[1]=TempDEC;
    Data[2]= (unsigned char) Hum;
    Data[3]=Make8(VOC,1);
    Data[4]=Make8(VOC,2);
    Data[5]=Make8(Co,1);
    Data[6]=Make8(Co,2);
    Data[7]=Make8(Ax,1);
    Data[8]=Make8(Ax,2);
    Data[9]=Make8(Ay,1);
    Data[10]=Make8(Ay,2);
    Data[11]=Make8(Az,1);
    Data[12]=Make8(Az,2);

    XBAPI_Tx(Data,13,0x01,0x01,0x7B,Adrs,AdrsMyAdrs,AdrsDummy);

}




void EnMaSyNeuPaket(unsigned char Daten[])
{

    if(Daten[30]==LedKontID)
    {
        LedKont(Daten[31]);
    }
}