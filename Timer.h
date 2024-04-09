#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#ifndef TIMER_H
#define TIMER_H

void Timer_init(int Time);
 void SysTickDisable (void);
 void SysTickEnable (void);
 uint32 SysTickPeriodGet (void);
 void SysTickPeriodSet (uint32 Period);
 uint32 SysTickValueGet (void);
 bool SysTick_Is_Time_out(void);

#endif
