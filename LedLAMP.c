# include <xc.h>
# include <plib/pwm.h>
# include <plib/timers.h>
# include "LedLAMP.h"

void LedInitialiserung()
{
    unsigned char Timer2Config, Timer2Config1;

    TRISCbits.RC1=0;
    LATC1=0;

    Timer2Config = TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_8;
    //Timer2Config1 = TIMER_GATE_OFF & TIMER_GATE_TOGGLE_OFF & TIMER_GATE_1SHOT_OFF & TIMER_GATE_INT_OFF;

    OpenTimer2(Timer2Config);
    OpenEPWM2(0xFF,ECCP_2_SEL_TMR12);
    SetDCEPWM2(1000);

}

//void LedSteurung (unsigned char Leistun)
//{
  //  SetDCEPWM2( (unsigned int)((Leistun*1024)/100) );
//}

void LedKont(unsigned char POW)
{
    SetDCEPWM2( (unsigned int)((POW*1024)/100) );
}