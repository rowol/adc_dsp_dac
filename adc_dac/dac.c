//
// DAC.C
// Digital to analog converter module, non-DMA, can be called from
// a timer interrupt for consistent sampling rate, etc
// Configures both DAC channel 1 (PA.4) and channel 2 (PA.5)
//
// Written by Ross Wolin
//

#include "stm32f4xx.h"

#include "hw.h"
#include "dac.h"







void DAC_init(void)
{
   // Enable clocks for port A and DAC
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
   
   // Set up PA.4 as DAC channel 1 output and PA.5 as DAC channel 2 output
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* DAC channel 1 Configuration */
   DAC_InitTypeDef  DAC_InitStructure;       //Was global in original code (?)
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
   DAC_Init(DAC_Channel_1, &DAC_InitStructure);

   /* Enable DAC Channel 1 */
   DAC_Cmd(DAC_Channel_1, ENABLE);

   
   /* DAC channel 2 Configuration */
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
   DAC_Init(DAC_Channel_2, &DAC_InitStructure);

   /* Enable DAC Channel 2 */
   DAC_Cmd(DAC_Channel_2, ENABLE);
}



//Set the ADC to the value in n, which should be 12 bits (i.e. 0 <= n <= 4095)
void DAC1_set(uint16_t n)
{
   if (DAC_MIN <= n  &&  n <= DAC_MAX) 
      DAC_SetChannel1Data(DAC_Align_12b_R, n);
}

void DAC2_set(uint16_t n)
{
   if (DAC_MIN <= n  &&  n <= DAC_MAX) 
      DAC_SetChannel2Data(DAC_Align_12b_R, n);
}

