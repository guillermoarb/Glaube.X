/* 
 * File:   SHT15.h
 * Author: MiguelAlejandro
 *
 * Created on 22 de septiembre de 2014, 11:20
 *
 * SHT15.h requiere que se definan los Pines de trabajo para el SCK y SDA.
 * En este caso, se están utilizando como SCK y SDA a RC3 y RC4, respectivamente.
 * Si se desean cambiar, deberán ajustarse coherentemente las siguientes declaraciones:
 *      #define SDA
 *      #define SCK
 *      #define SCK_DIG
 *      #define SDA_DIG
 *      #define WRITE_SCK
 *      #define READ_SCK
 *      #define WRITE_SDA
 *      #define READ_SDA
 */

#ifndef SHT15_H
#define	SHT15_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* ==== DECLARACIONES ==== */
    #define Buff_Size   2                   //Tamaño del Buffer 2 Bytes.
    #define SCK         PORTCbits.RC3
    #define SDA         PORTCbits.RC4
    #define SCK_DIG()   ANSELCbits.ANSC3=0
    #define SDA_DIG()   ANSELCbits.ANSC4=0
    #define WRITE_SCK() TRISCbits.RC3=0
    #define READ_SCK()  TRISCbits.RC3=1
    #define WRITE_SDA() TRISCbits.RC4=0     //SDA Output al Escribir
    #define READ_SDA()  TRISCbits.RC4=1     //SDA Input al Leer
    /* Registros */
    #define SHT15_TEMP  0x03    //Measure Temperature:  00011
    #define SHT15_HUMD  0x05    //Measure Relative Humidity:    00101
    #define SHT15_READ  0x07    //Read Status:  00111
    #define SHT15_WRITE 0x06    //Write Status: 00110
    /* Resolución Default de Temperatura: 14 bits */
    #define D1          -40.1   // for °C @ 5V
    #define D2          0.01    // for 14 bits @ °C
    /* Resolución Default de Humedad: 12 bits */
    #define C1          -2.0468         // for 12 bits
    #define C2          0.0367          // for 12 bits
    #define C3          -0.000001595    // for 12 bits
    /* Compensación de Humedad por Temperatura */
    #define T1          0.01    // for 12 bits
    #define T2          0.00008 // for 12 bits

    /* Función para armar dato de 16bits */
    unsigned int make16(unsigned char MSB, unsigned char LSB);
    /* Lectura del Sensor */
    double Read_SHT15(unsigned char Opcion);
    /* Envia el Comando específico de START para el SHT15 */
    void Start_SHT15(void);
    /* Configura los pines SDA y SCK */
    void setSHT15(void);
    /* Envia Byte al SHT15 */
    void SendByte_SHT15(unsigned char Registro);
    /* Lee 2 Bytes del SHT15 */
    unsigned int Read2Bytes_SHT15(void);
    /* Skip CRC */
    void SkipCRC(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SHT15_H */