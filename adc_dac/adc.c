//
// ADC.C
// Analog to digital converter module, non-DMA, can be called from
// a timer interrupt for consistent sampling rate, etc
// Configures ADC channel 11 on PC.1
//
// Written by Ross Wolin
//

#include <stdbool.h>

#include "stm32f4xx.h"

#include "hw.h"
#include "adc.h"



void ADC_init(void)
{
   /* Enable peripheral clocks */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 
   /* Configure ADC Channel 10 pin as analog input */
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
 
   /* ADC Common configuration *************************************************/
   ADC_CommonInitTypeDef ADC_CommonInitStructure;
   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
   ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
   ADC_CommonInit(&ADC_CommonInitStructure);
 
   /* ADC1 regular channel 10 to 15 configuration ************************************/
   ADC_InitTypeDef ADC_InitStructure;
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 1 Channel
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Conversions Triggered
   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // Manual
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfConversion = 1;
  
   ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_144Cycles);
 
   /* Enable ADC1 */
   ADC_Cmd(ADC1, ENABLE);

   ADC_start();
}



void ADC_start(void)
{
   ADC_SoftwareStartConv(ADC1);
}




//Get ADC value if the ADC is ready
//Returns value if successful, -1 if ADC wasn't done yet
int ADC_get(void)
{
   //HACK - TESTING
   while (ADC_GetSoftwareStartConvStatus(ADC1) != RESET)
      ;
   //HACK - TESTING
   
   return (ADC_GetSoftwareStartConvStatus(ADC1) == RESET)
      ? ADC_GetConversionValue(ADC1)
      : -1;
}
