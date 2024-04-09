#include "DIO.h"
#include "bitwise_operation.h"
#include "tm4c123gh6pm.h"
#include "types.h"

void DIO_Init()
{

  SYSCTL_RCGCGPIO_R |= 0x10;
  while ((SYSCTL_PRGPIO_R &  0x10) != 0x10)
 {}
GPIO_PORTE_LOCK_R = 0x4C4F434B;
  GPIO_PORTE_CR_R |= (1<<3);
GPIO_PORTE_DIR_R &= (~(1<<2));
GPIO_PORTE_DIR_R = 0x0;
GPIO_PORTE_DEN_R |=(1<<3); //pin E3
GPIO_PORTE_DEN_R |=(1<<1); //pin E1
 GPIO_PORTE_PUR_R |= (1<<3);

}

void DIO_WritePin ( uint64_vol_ptr port , uint16 pin , uint32 value)
{
  if (value==0)
  clearbit(*port, pin);
  else
  setbit(*port,pin);   
}

void DIO_WritePort(uint64_vol_ptr port , uint32 value)
{
  *port = value; 
}

uint16 DIO_ReadPin(uint64_vol_ptr port, uint16 pin )
{ 
return get_bit(*port,pin);
}

uint64_vol_ptr DIO_ReadPort(uint64_vol_ptr port)
{
  return port;
}

void delay()
{
  for (int i=0; i<100000;i++)
  {}
}

void DIO_InitF(){ 
 SYSCTL_RCGCGPIO_R |= 0x20; 
   while ((SYSCTL_PRGPIO_R &  0x20) != 0x20)
 {}
 GPIO_PORTF_LOCK_R = 0x4C4F434B; 
 GPIO_PORTF_CR_R = 0x1F; 
 GPIO_PORTF_DIR_R = 0x0E; 
 GPIO_PORTF_DEN_R = 0x1F; 
 GPIO_PORTF_PUR_R = 0x11;
 
} 

