#include "types.h"
#ifndef DIO_H
#define DIO_H


//void DIO_Init();
void DIO_WritePin(uint64_vol_ptr, uint16, uint32);
void DIO_WritePort(uint64_vol_ptr, uint32);
uint16 DIO_ReadPin( uint64_vol_ptr, uint16);//, uint32);
uint64_vol_ptr DIO_ReadPort(uint64_vol_ptr );
void delay();
void DIO_InitF();
#endif
