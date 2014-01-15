//
// TMR_SAMPLE.CPP
// Uses basic timer TIM6 to generate an interrupt at a rate of approximately 44Khz
//

#include "stm32f4xx.h"

#include "hw.h"
#include "tmr_sample.h"


// TIM6 configuration is based on CPU @168MHz and APB1 @84MHz
// TIM6 Update event occurs each 84Mhz/44Khz ~= 1909 ticks (44,002 Hz)
void tmr_sample_init(void)
{
   /* TIM6 Periph clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

   /* Time base configuration */
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   TIM_TimeBaseStructure.TIM_Period = (int)(APB1_FREQ/SAMPLE_FREQ);     //44Khz DAC update rate, 16 bit period on TIM6                 
   TIM_TimeBaseStructure.TIM_Prescaler = 0;       //Divide by 1
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //No division
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

   //Set up TIM6 interrrupt
   NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
  
   /* TIM6 enable counter and interrupt */
   TIM_Cmd(TIM6, ENABLE);
   TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);  
}



