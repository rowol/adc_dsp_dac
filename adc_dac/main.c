//
// MAIN.C
//

#include <stdbool.h>

#include "stm32f4xx.h"

#include "hw.h"
#include "dac.h"
#include "adc.h"
#include "tmr_sample.h"
#include "timer.h"
#include "led.h"



#define LED_PERIOD_MS      250



int main(void)
{
	if (SysTick_Config(SystemCoreClock/1000)) {
	 	while (true)  // Capture error
          ;
	}

//   led_init(LED1);   //PC2, used to show main loop (below) is running
   ADC_init();
   DAC_init();
   tmr_sample_init();
   
   

   
   
   /*!< At this stage the microcontroller clock setting is already configured, 
     this is done through SystemInit() function which is called from startup
     file (startup_stm32f4xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f4xx.c file
   */    

   TMR tLED;
   tmr_setInterval(&tLED, LED_PERIOD_MS);

   while (true) {
      //Flash LED twice a second
      if (tmr_isExpired(&tLED)) {
         tmr_reset(&tLED);
//         led_toggle(LED1);
      }
   }
}




//Time to load the DACs!
//This interrupt routine is called from a timer interrupt, at a rate of 44Khz,
//which is a good sampling rate for audio
//(Although the default handler has 'DAC' in the name, we are just using this
// as generic timer interrupt)
void TIM6_DAC_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM6, TIM_IT_Update)) {

      //Process the ADC and DACs here...
      // LOAD THE DACs HERE!


      // Generate a sawtooth wave of approximately 880hz
      {
         static int nDacIn = DAC_MIN;

         DAC1_set(nDacIn);

         //Set up for the next update
         nDacIn += ((DAC_MAX-DAC_MIN) * 880)/SAMPLE_FREQ;
         if (nDacIn > DAC_MAX)
            nDacIn = DAC_MIN;

         
         //TESTING
         //HACK, should get if this is -1...
         int n = ADC_get();
         ADC_start();         //Start a new conversion
         DAC2_set(n);
         //TESTING

      }

      TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
   }
}
