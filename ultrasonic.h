#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include "tm4c123gh6pm.h"
#include "types.h"

void OutSignal(uint32 value);
int32 measureD();
uint32 measureDistanceOnce(void);
void InitRegisters();
void US_Timer0_init(void);
void delay_Microsecond(uint32 time);
void UART1_init(void);
void UART1_Transmitter(unsigned char data);
void printstring(char *str);
uint32 measureDistanceOnce(void);
#endif