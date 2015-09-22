/* 
 * File:   LedLAMP.h
 * Author: GuillermoARB
 *
 * Created on 22 de abril de 2015, 18:37
 */

#ifndef LEDLAMP_H
#define	LEDLAMP_H

#ifdef	__cplusplus
extern "C" {
#endif

//#define LedLAMP PORTCbits.RC1


    void LedKonfiguration (unsigned  int Hz);
    void LedInitialiserung (void);
    void LedKont(unsigned char POW);



#ifdef	__cplusplus
}
#endif

#endif	/* LEDLAMP_H */

