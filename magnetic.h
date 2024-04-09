#ifndef MAGNETIC_H
#define MAGNETIC-H
#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"
#include "tm4c123gh6pm.h"
#include "types.h"

void DIO_Init();
uint16 DIO_ReadPin(uint64_vol_ptr port, uint16 pin );
#endif