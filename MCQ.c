#include <stdbool.h>
#include "stdint.h"


#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "tm4c123gh6pm.h"
#include "dio.h"
#include <stdio.h>
#include "types.h"
#include "lcd.h"

#define LED_RED (1u << 1) //0010
#define LED_BLUE (1u << 2) //0100
#define LED_GREEN (1u << 3) //1000


//*****************************************************************************
//
// Configure ADC0 for the temperature sensor input with a single sample.  Once
// the sample is done, an interrupt flag will be set
//*****************************************************************************
int main(void)
{
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    uint32_t ui32SysClock;
#endif

    //
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
    uint32_t pui32ADC0Value[1];


    //
    // Set the clocking to run at 20 MHz (200 MHz / 10) using the PLL.  When
    // using the ADC, you must either use the PLL or supply a 16 MHz clock
    // source.
    //
    
    
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    //
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_240), 20000000);
#else
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#endif

 
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a singal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample the temperature sensor
    // (ADC_CTL_TS) and configure the interrupt flag (ADC_CTL_IE) to be set
    // when the sample is done.  Tell the ADC logic that this is the last
    // conversion on sequence 3 (ADC_CTL_END).  Sequence 3 has only one
    // programmable step.  Sequence 1 and 2 have 4 steps, and sequence 0 has
    // 8 programmable steps.  Since we are only doing a single conversion using
    // sequence 3 we will only configure step 0.  For more information on the
    // ADC sequences and steps, reference the datasheet.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                             ADC_CTL_END);

    // Since sample sequence 3 is now configured, it must be enabled.
   
    ADCSequenceEnable(ADC0_BASE, 3);

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 3);
    // Sample the temperature sensor forever.  Display the value on the
    // console.
    
    LCD_init();
    while(1)
    {
        // Trigger the ADC conversion.
        ADCProcessorTrigger(ADC0_BASE, 3);
        // Wait for conversion to be completed.
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }

        // Clear the ADC interrupt flag.        
        ADCIntClear(ADC0_BASE, 3);
        // Read ADC Value.
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
    

        LCD_command(1); /* clear display */
        LCD_command(0x80); /* lcd cursor location */
        delayMs(20);
        LCD_data('f');
        LCD_data('a');
        LCD_data('r');
        LCD_data('i');
        if (pui32ADC0Value[0] >400){
        // Display the temperature value on the console.
        //
        LCD_data('s');
        LCD_data('m');
        LCD_data('o');
        LCD_data('k');
        LCD_data('e');
        delayMs(1000);
        }
        else{
        
        LCD_data('n');
        LCD_data('o');
        delayMs(1000);
       
        }

        //
        // This function provides a means of generating a constant length
        // delay.  The function delay (in cycles) = 3 * parameter.  Delay
        // 250ms arbitrarily.
        //
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
        SysCtlDelay(ui32SysClock / 12);
#else
        SysCtlDelay(SysCtlClockGet() / 12);
#endif
    }
}
