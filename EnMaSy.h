/* 
 * File:   EnMaSy.h
 * Author: GuillermoARB
 *
 * Created on 20 de abril de 2015, 10:54
 */

#ifndef ENMASY_H
#define	ENMASY_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LedKontID 0xB7

void MonPerSend(double Temp, double Hum, unsigned char Luft,unsigned int Co, unsigned int Lux, unsigned int Leistung, unsigned char AdrsAdressat[], unsigned char AdrsHerkunft[], unsigned char AdrsReisezeil[]); //Función para envío de paquete de monitorización periodica
void EnMaSyNeuPaket(unsigned char Data[]);
void SimadePack(double Temp, double Hum, unsigned int VOC,unsigned int Co, unsigned int Ax, unsigned int Ay, unsigned int Az, unsigned char Adrs[]);


#ifdef	__cplusplus
}
#endif

#endif	/* ENMASY_H */

