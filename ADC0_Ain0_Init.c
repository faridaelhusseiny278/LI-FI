#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC0_Ain0_Init.h"
#include "types.h"

void ADC_Init(void)
{
volatile unsigned long delay;

SYSCTL_RCGC0_R |= 0x00010000;

delay = SYSCTL_RCGC2_R;
SYSCTL_RCGC0_R &= ~0x00000300;
ADC0_SSPRI_R &= ~0xF000;
ADC0_ACTSS_R &= ~0x8;
ADC0_EMUX_R &= ~0xF000;
ADC0_SSMUX3_R = 0x0;
ADC0_SSCTL3_R |= 0x0E;
ADC0_ACTSS_R |= 0x8;
}
