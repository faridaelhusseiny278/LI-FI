#include "tm4c123gh6pm.h"
#include "DIO.h"
#include "bitwise_operation.h"
#include "types.h"
#include "lcd.h"
#include "magnetic.h"
#include "ultrasonic.h"
//#include "fume.h"
//#include <stdlib.h>
#include <stdio.h>
#include "Bluetooth.h"
#include <string.h>
#include <stdbool.h>

//------------------Blueetooth------------------- //
void Delay(unsigned long counter); // used to add delay
void HC05_init(void); // Initialize UART5 module for HC-05
char Bluetooth_Read(void); //Read data from Rx5 pin of TM4C123
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin 
void Bluetooth_Write_String(char *str);
//------------------Blueetooth ends---------------------
uint32 measureDistanceOnce(void);
void Timer0ACapture_init(void);
void Delay_MicroSecond(int time);
void OutSignal(uint32 value);
void InitRegisters();
int32 measureD();
void US_Timer0_init(void);
//void printstring(char *str);
void Delay(unsigned long counter);

uint32 time; /*stores pulse on time */
int32 distance; /* stores measured distance value */
char mesg[20]; 

int main()
{
   US_Timer0_init();
    UART1_init();
DIO_InitF();
DIO_Init();
 uint16 pin_magnetic=0;
 uint16 pin_fume=0 ;
volatile long ADC_Output= 0 ;
volatile long temp_c= 0;
ADC_Init();



while(1){
 uint16 pin_sw2 = DIO_ReadPin(&GPIO_PORTF_DATA_R,4);// start system
uint32 maskedValue = pin_sw2 & (1u << 4);
if (maskedValue ==0) //constantly check start button 
{
   while(1)
   {
   uint16 pin_sw1 = DIO_ReadPin(&GPIO_PORTF_DATA_R,0); //stop the system
  uint32 maskedValue_stop = pin_sw1 & (1u << 0);
  if (maskedValue_stop== 0)
  {
  break;
  }

  
  pin_fume=DIO_ReadPin(&GPIO_PORTE_DATA_R,1);
  
  if(pin_fume==2)
  {
//    Timer_init(200);
//    GPIO_PORTF_DATA_R= 0xE;
//    while(1)
//    {
//         if(SysTick_Is_Time_out()==true){
//         GPIO_PORTF_DATA_R= 0x0;
//          break;
//         }
//         
//          
//       }
    printf("NF");
      printf("\n");
       //Bluetooth_Write_String("Fume \n");
    
  }
  
  else if(pin_fume==0)
  {
     
    Timer_init(200);
    GPIO_PORTF_DATA_R= 0xE;
    while(1)
    {
         if(SysTick_Is_Time_out()==true){
         GPIO_PORTF_DATA_R= 0x0;
          break;
         }
         
          
       }
    
    
       printf("F");
    printf("\n");
     Bluetooth_Write_String("Fume\n");
    
    
  }
   pin_magnetic = DIO_ReadPin(&GPIO_PORTE_DATA_R,3);
      
   if (pin_magnetic ==8)
   {
     Timer_init(600);
      GPIO_PORTF_DATA_R= 0xE;
      while(1)
       {
         if(SysTick_Is_Time_out()==true){
         GPIO_PORTF_DATA_R= 0x0;
         break; 
       }
       }
      
    printf("M");
    printf("\n");
     Bluetooth_Write_String("Magnet \n");

   }
   else if (pin_magnetic ==0) 
   {

   
     GPIO_PORTF_DATA_R= 0x0;
       printf("NM");
    printf("\n");
       //Bluetooth_Write_String("\n");
   }
   
  
  

       time = measureDistanceOnce();
       
       distance = (time* 10625)/10000000;
       if (distance<=10){
       
       Timer_init(400);
       GPIO_PORTF_DATA_R= 0xE;
        bool x;
       while(1)
         
       {
         x = SysTick_Is_Time_out();
         if(x==true){
         GPIO_PORTF_DATA_R= 0x0;
         break; 
       }
       }
      
      printf("U");
      printf("\n");
      char concatenatedString[]="ultrasonic\n";
      Bluetooth_Write_String(concatenatedString);
       }
       else{
       
        GPIO_PORTF_DATA_R= 0x0;
       }
        

    int number=(int)distance;
    char stringRep[20];
  
     printstring(mesg);
     
 }
}
}
}
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}

//void Farida_Hassan_Mazen_Mariam(void)
//{
//  GPIO_PORTF_DATA_R= 0x0;
//}