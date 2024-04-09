#include "DIO.h"
#include "bitwise_operation.h"
#include "tm4c123gh6pm.h"
#include "types.h"
#include "Timer.h"
#include <stdbool.h>
#include <stdio.h>

#define FREQUENCY 16000

void Timer_init(int Time){
NVIC_ST_CTRL_R&=(~(0x1)); //disable timer
NVIC_ST_RELOAD_R =  (Time* FREQUENCY)-1;
NVIC_ST_CURRENT_R = 0x0;
NVIC_ST_CTRL_R= 0x05;

}
 void SysTickDisable (void)
{
 NVIC_ST_CTRL_R&=(~(0x1)); //disable timer
}

void SysTickEnable (void)
{
NVIC_ST_RELOAD_R = 500*FREQUENCY;
}

uint32 SysTickPeriodGet (void)
{
return (((NVIC_ST_RELOAD_R)+1) * FREQUENCY);
}

void SysTickPeriodSet (uint32 ui32Period)
{
  NVIC_ST_RELOAD_R =  500*FREQUENCY;
}

uint32 SysTickValueGet (void)
{
return(NVIC_ST_CURRENT_R);
}

bool SysTick_Is_Time_out(void)
{
  if (((NVIC_ST_CTRL_R) & (0x10000)) == 0x10000)  
  {
  return (true);
  }
  else {
  return (false);
  }
}