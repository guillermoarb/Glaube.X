#include <xc.h>
#include "SHT15.h"

/* ==== DECLARACIONES ==== */
//#define _XTAL_FREQ 8000000  //Velocidad del OSC Externo/Interno
#define _XTAL_FREQ 32000000  //Velocidad del OSC Externo/Interno

/* Función para armar dato de 16bits */
unsigned int make16(unsigned char MSB, unsigned char LSB)
{
    /* H: MS_Byte; L: LS_Byte */
    return ((MSB<<8)|LSB);
}

/* Envia el Comando específico de START para el SHT15 */
void Start_SHT15(void)
{
 /*
 *      START SEQUENCE FOR SHT15 SENSOR
 *
 *           -------     -------
 * SCK _____|       |___|       |_______
 *
 *       -------             -------
 * SDA _|       |___________|       |___
 *
 *
 */
    WRITE_SDA();    //SDA (RC4) Salida
    SDA = 1;
    __delay_ms(1);
    SCK = 1;
    __delay_ms(1);
    SDA = 0;
    __delay_ms(1);
    SCK = 0;
    __delay_ms(1);
    SCK = 1;
    __delay_ms(1);
    SDA = 1;
    __delay_ms(1);
    SCK = 0;
    __delay_ms(1);
    //SDA = 0;
}

/* Configura los pines SDA y SCK */
void setSHT15(void)
{
    SCK_DIG();      //SCK Analógico OFF
    SDA_DIG();      //SDA Analógico OFF
    WRITE_SCK();    //SCK Output
    WRITE_SDA();    //SDA Output
}

/* Lee Humedad[1]/Temperatura[0] del SHT15 */
double Read_SHT15(unsigned char Opcion)
{
    unsigned int T=0;
    signed int RH=0;
    double RH_Lineal=0,RH_Real=0,Temperatura=0;

    /* ==== Leer Temperatura ==== */
    Start_SHT15();
    SendByte_SHT15(SHT15_TEMP);
    T = Read2Bytes_SHT15();  //Lee 2 Bytes
    SkipCRC();
    Temperatura = D1 + (T * D2);

    if(Opcion == 1) /* === Humedad === */
    {
        /* ==== Leer Humedad ==== */
        Start_SHT15();
        SendByte_SHT15(SHT15_HUMD);
        RH = Read2Bytes_SHT15();
        SkipCRC();
        RH_Lineal = C1 + (C2 * RH) + (C3 * RH * RH);

        //Temperatura: 20°C - 30°C
        if(20 < Temperatura && Temperatura < 30)
        {
            return RH_Lineal;
        }   //Temperatura dentro de Rango de Operación -40°C - 123.8°C
        else if (-40 < Temperatura && Temperatura < 123.8)
        {
            /* ==== Compensación de la Señal de Humedad por Temperatura ==== */
            /* Para temperaturas significativamente diferentes a 25°C (77°F) */
            RH_Real = (Temperatura - 25)*(T1 + T2*RH) + RH_Lineal;
            return RH_Real;
        }   //Temperatura Fuera de Rango, por lo tanto RH será errónea.
        else
        {
            return 0;
        }
    }
    else if(Opcion == 0) /* === Temperatura === */
    {   /* === Dentro del Rango? === */
        if(-40 < Temperatura && Temperatura < 123.8)
            return Temperatura;
        else
            return 0;   //No.
    }
    else /* === Opción != 0 && Opción != 1 === */
    {
        return 0;
    }
}

/* Envia Byte al SHT15 */
void SendByte_SHT15(unsigned char Registro)
{
    unsigned char i;
    WRITE_SDA();
    for(i = 0 ; i < 8 ; i++)
    {
        SCK = 0;
        __delay_ms(1);
        SDA = (Registro & 0x80)>>7;
        __delay_ms(1);
        SCK = 1;
        __delay_ms(1);
        Registro = Registro<<1;
    }
    /* ===== Wait for SHT15 to acknowledge ===== */
    SCK = 0;
    __delay_ms(1);
    READ_SDA();
    while (SDA == 1);   //Wait for SHT to pull line low
    SCK = 1;
    __delay_ms(1);
    SCK = 0;            //Falling edge of 9th clock
    __delay_ms(1);

    /* ===== Measurement 80mS for 12bit ===== */
    while(SDA==1);  //Wait for SHT to finish measurement (SDA will be pulled low)
}

/* Lee 2 Bytes del SHT15 */
unsigned int Read2Bytes_SHT15(void)
{
    //8 bits
    unsigned char Buffer_SHT[Buff_Size];
    unsigned char i,j;
    //16 bits
    unsigned int in_byte;

    for(j=0; j<2;j++)
    {
        SCK = 0;
        READ_SDA();

        for(i = 0; i < 8; i++)
        {
            SCK = 0;
            __delay_ms(1);
            SCK = 1;
            __delay_ms(1);
            in_byte = in_byte << 1;
            if(SDA == 1) in_byte = in_byte|0x01;
        }

        //Send acknowledge to SHT7x
        SCK = 0;
        WRITE_SDA();
        SDA = 0;
        SCK = 1;
        __delay_ms(1);
        SCK = 0;        //Falling edge of 9th clock
        __delay_ms(1);

        Buffer_SHT[j] = in_byte;
    }
    return make16(Buffer_SHT[0],Buffer_SHT[1]);
}

/* Skip CRC */
void SkipCRC(void)
{
    /* === Lee el CRC, no lo usa === */
    unsigned char i;
    unsigned int crc;

    SCK = 0;
    READ_SDA();

    for(i = 0; i < 8; i++)
    {
        SCK = 0;
        __delay_ms(1);
        SCK = 1;
        __delay_ms(1);
        crc = crc << 1;
        if(SDA == 1) crc = crc|0x01;
    }
    //Send acknowledge to SHT7x
    SCK = 0;
    WRITE_SDA();
    SDA = 0;
    SCK = 1;
    __delay_ms(1);
    SCK = 0;        //Falling edge of 9th clock
    __delay_ms(1);

//    return crc;
}
