

#include "AirQ.h"
#include <plib/adc.h>
#include <plib/timers.h>
#define USE_AND_MASK

unsigned int GetCO()
{
    unsigned int CORaw=0;

    SetChanADC(ADC_CH0);

    ConvertADC();

    while(BusyADC());
    CORaw = ReadADC();

    //CloseADC();

    return CORaw;

}

int GetVOC()
{
    unsigned int VOC_Raw=0;

    SetChanADC(ADC_CH1);

    ConvertADC();

    while(BusyADC());
    VOC_Raw = ReadADC();

    //CloseADC();
    

    return VOC_Raw;


}


void Init_ADC_AirQ()
{
 
    unsigned char config1=0x00,config2=0x00,config3=0x00;
    

    config1 = ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_0_TAD ;
    config2 = ADC_CH0 & ADC_CH1 & ADC_INT_OFF ;
    config3 = ADC_REF_VDD_VSS ;

    OpenADC(config1,config2,config3);

}

void InitVOC()
{
    ANSELBbits.ANSB5=0;  // RB5 as digital output
    TRISBbits.RB5=0;
    ANSELAbits.ANSA0=1;  //RA0 as ADC input
    ANSELAbits.ANSA1=1;  //RA1 as ADC input

}

void SetupTimerVOC()
{
    unsigned char Timer1Config, Timer1Config1;

    Timer1Config = TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_FOSC_4 & T1_PS_1_8 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF;
    Timer1Config1 = TIMER_GATE_OFF & TIMER_GATE_TOGGLE_OFF & TIMER_GATE_1SHOT_OFF & TIMER_GATE_INT_OFF;

    OpenTimer1(Timer1Config,Timer1Config1);
    WriteTimer1(0);
    TGS8100Pulse=0;

}

