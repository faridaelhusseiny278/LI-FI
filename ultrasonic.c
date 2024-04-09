#include"ultrasonic.h"
#include<math.h>

//#include<stdio.h>

const double _16MHz_1clock = 0.0625; // Value of 1clock cycle in mikroseconds
 
#define ECHO (1U << 6) //PB6
#define TRIGGER (1U << 4) //PA4(OUTPUT)
#define BLUE_LED (1U << 2) //PF2 onboard Blue LED
#define GREEN_LED (1U << 3) //PF3 onboard Green LED
#define RED_LED (1U << 1) //PF1 onboard Red LED


uint32 measureDistanceOnce(void) { 
		uint32 lastEdge,thisEdge;
		
    GPIO_PORTA_DATA_R &=~(1<<4);
    delay_Microsecond(10);
    GPIO_PORTA_DATA_R |= (1<<4);
    delay_Microsecond(10);
    GPIO_PORTA_DATA_R &=~(1<<4);
    /*Capture firstEgde i.e. rising edge*/
    
    while(1)
    {
   TIMER0_ICR_R = 0x04; /* clear timer0A Capture flag */
   while((TIMER0_RIS_R & 4)==0){}; //Wait till captured
                if(GPIO_PORTB_DATA_R & (1<<6))
                {
		lastEdge =  TIMER0_TAR_R;

		/*Capture secondEdge i.e. falling edge */
		TIMER0_ICR_R =4; //clear timer capture flag
		while((TIMER0_RIS_R & 4)  ==0){};
		thisEdge = TIMER0_TAR_R;
		return (thisEdge - lastEdge);
		}
    }


}





void OutSignal(uint32 value)
{
GPIO_PORTF_DATA_R &= ~(RED_LED | GREEN_LED | BLUE_LED);
	
  if(value < 15) { 
    GPIO_PORTF_DATA_R |= RED_LED;
	}
  else if (value < 50) {
    GPIO_PORTF_DATA_R |= RED_LED | BLUE_LED;
	}
	else if (value < 100) {
		GPIO_PORTF_DATA_R |= RED_LED | GREEN_LED;
	}
	else if (value < 200) {
		GPIO_PORTF_DATA_R |= BLUE_LED | GREEN_LED;
	}
	else if (value < 300) {
		GPIO_PORTF_DATA_R |= BLUE_LED;
	}
	else if (value < 400) {
		GPIO_PORTF_DATA_R |= GREEN_LED;
	}
	else {
		GPIO_PORTF_DATA_R |= RED_LED | GREEN_LED | BLUE_LED;
	}
}

int32 measureD()
{
  
  uint32 counter = 0;
   int32 dist = 0;
	 int32 instantDist = 0;
	
	 for (counter = 0; counter < 100; ++counter) {
		 instantDist = measureDistanceOnce();
		 if (instantDist > 0 && instantDist < 9999) {
		    dist = (dist + instantDist) / 2;
		 }
	 }
	 
	 return dist;
}

void delay_Microsecond(uint32 time)
{
  int i;
    SYSCTL_RCGCTIMER_R |= 2; /*enable clock Timer1 subtimer A in run mode */
    TIMER1_CTL_R = 0x0; /* disable timer1 output */
    TIMER1_CFG_R = 0x04;  /*select 16-bit configuration option */
    TIMER1_TAMR_R = 0x02; /*select periodic down counter mode of timer1 */
    //TIMER1_TAPR_R = 250-1; /* TimerA prescaler value */
    TIMER1_TAILR_R = 16-1; /* TimerA counter starting count down value  */
    TIMER1_ICR_R = 0x1;  /* TimerA timeout flag bit clears*/
    TIMER1_CTL_R |= 0x01;  /* Enable TimerA module */
 
    for(i=0; i<time; i++)
    {
      while((TIMER1_RIS_R & 0x1)==0){};
      TIMER1_ICR_R = 0x1;
    }
    
    //TIMER1_IMR_R |=(1u<<0); /*enables TimerA time-out  interrupt mask */
    //NVIC_EN0_R |= (1u<<21);
}

void US_Timer0_init(void)
{
   SYSCTL_RCGCTIMER_R |= 1; /*enable clock Timer0 subtimer A in run mode */
   
   
   SYSCTL_RCGCGPIO_R |= 2;
   GPIO_PORTB_DIR_R &= ~0x40;
   GPIO_PORTB_DEN_R |= 0x40;
   GPIO_PORTB_AFSEL_R |= 0x40;
   GPIO_PORTB_PCTL_R &= ~0x0F000000;
    GPIO_PORTB_PCTL_R |= 0x07000000;
   /* PB2 as a digital output signal to provide trigger signal */
     SYSCTL_RCGCGPIO_R |= 1;  /* enable clock to PORTA */
     GPIO_PORTA_DIR_R |= (1u <<4);/* set PB2 as a digial output pin */
      GPIO_PORTA_DEN_R |= (1u <<4);  /* make PB2 as digital pin */
      
   TIMER0_CTL_R &= ~1; /* disable timer0 output */
    TIMER0_CFG_R = 0x04;  /*select 16-bit configuration option */
    TIMER0_TAMR_R = 0x17; /*select periodic down counter mode of timer0 */
    //TIMER0_TAPR_R = 250-1; /* TimerA prescaler value */
    //TIMER0_TAILR_R = 64000-1; /* TimerA counter starting count down value  */
    //TIMER0_ICR_R = 0x1;  /* TimerA timeout flag bit clears*/
   TIMER0_CTL_R |= 0x0C;
    TIMER0_CTL_R |= (1<<0);  /* Enable Timer0A module */
  //TIMER1_IMR_R |=(1u<<0); /*enables TimerA time-out  interrupt mask */
  //NVIC_EN0_R |= (1u<<21);
  	
}


void UART1_init(void)
{
	  SYSCTL_RCGCUART_R |= 0x20;  /* enable clock to UART1 */
    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    /* UART0 initialization */
    UART5_CTL_R = 0;         /* UART5 module disbable */
    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5_CC_R = 0;          /*select system clock*/
    UART5_LCRH_R = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTE_DEN_R = 0x30;      /* set PB0 and PB1 as digital */
    GPIO_PORTE_AFSEL_R = 0x30;    /* Use PB0,PB1 alternate function */
    GPIO_PORTE_AMSEL_R = 0;    /* Turn off analg function*/
    GPIO_PORTE_PCTL_R = 0x00110000;     /* configure PB0 and PB1 for UART */
}
void UART1_Transmitter(unsigned char data)  
{
    while((UART5_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5_DR_R = data;                  /* before giving it another byte */
}

void printstring(char *str)
{
  while(*str)
	{
		UART1_Transmitter(*(str++));
	}
}


void InitRegisters()
{
     SYSCTL_RCGCGPIO_R |=(1U << 0); //Enable clock for PORTA 
    SYSCTL_RCGCGPIO_R |=(1U << 1); //Enable clock for PORTB 
    SYSCTL_RCGCGPIO_R |=(1U << 5); //Enable clock for PORTF 
    GPIO_PORTA_DIR_R |= TRIGGER;   //1000
    GPIO_PORTF_DIR_R |= RED_LED | GREEN_LED | BLUE_LED; //01110
    GPIO_PORTA_DEN_R |= TRIGGER; //1000
	  GPIO_PORTB_DEN_R |= ECHO; //100000
    GPIO_PORTF_DEN_R |= RED_LED | GREEN_LED | BLUE_LED; 
}