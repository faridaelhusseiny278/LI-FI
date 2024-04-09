#include "fume.h"


void fume_init()
{ 
 
  
SYSCTL_RCGCGPIO_R |= 0x02;
  GPIO_PORTB_CR_R = 0x01;
  GPIO_PORTB_DIR_R = 0x00;
  GPIO_PORTB_DEN_R = 0x01;



}

uint16 fume_get_value()
{
  
     return GET_BIT(GPIO_PORTB_DATA_R, 0);
   //return (GPIO_PORTB_DATA_R & 0x01); 
}