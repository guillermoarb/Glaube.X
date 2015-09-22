/* 
 * File:   AirQ.h
 * Author: GuillermoARB
 *
 * Created on 11 de marzo de 2015, 11:46
 */

#ifndef AIRQ_H
#define	AIRQ_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define TGS8100Pulse PORTBbits.RB5

    unsigned int GetCO(void);
    int  GetVOC(void);
    void Init_ADC_AirQ(void);
    void InitVOC();
    void SetupTimerVOC();



#ifdef	__cplusplus
}
#endif

#endif	/* AIRQ_H */

