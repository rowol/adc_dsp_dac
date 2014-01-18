//
// ADC.C
// Analog to digital converter module, non-DMA, can be called from
// a timer interrupt for consistent sampling rate, etc
// Configures ADC1 channel 15 on PC.5
//
// Written by Ross Wolin
//

#include <stdbool.h>

#include "stm32f4xx.h"

#include "hw.h"
#include "adc.h"



#define IO_PIN       GPIO_Pin_5
#define IO_PORT      GPIOC
#define IO_PORT_CLK  RCC_AHB1Periph_GPIOC

#define ADC_CLK      RCC_APB2Periph_ADC1
#define ADC_NUM      ADC1
#define ADC_CHANNEL  ADC_Channel_15


void ADC_init(void)
{
   /* Enable peripheral clocks */
   RCC_AHB1PeriphClockCmd(IO_PORT_CLK, ENABLE);
   RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
 
   /* Configure ADC Channel 10 pin as analog input */
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = IO_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
 
   /* ADC Common configuration *************************************************/
   ADC_CommonInitTypeDef ADC_CommonInitStructure;
   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   // APB2 clock is half the 168Mhz system clock (i.e. 84Mhz),
   // so with a div by 8, ADC PCLK would be 10.5Mhz.
   // F4 datasheet says ADC clock freq should be 0.6Mhz - 30Mhz for Vdda=3.3V
   ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
   //TwoSamplingDelay is only used in dual and triple modes)
   ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;   
   ADC_CommonInit(&ADC_CommonInitStructure);
 
   /* ADC1 regular channel 10 to 15 configuration ************************************/
   ADC_InitTypeDef ADC_InitStructure;
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 1 Channel
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Conversions Triggered
   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // Manual
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  //Unused for manual?
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfConversion = 1;
   ADC_Init(ADC1, &ADC_InitStructure);

   //The sample time is how long the input is sampled before the conversion is done.
   //Since PCLK is 10.5Mhz, 144 cycles is about 13.7uS and the DAC output rate is
   //running manually off a 44Khz timer interrupt (22uS), we should be fine
   //(once conversion starts, it takes about 16 cycles)
   ADC_RegularChannelConfig(ADC_NUM, ADC_CHANNEL, 1, ADC_SampleTime_144Cycles);
 
   /* Enable ADC1 */
   ADC_Cmd(ADC_NUM, ENABLE);

   ADC_start();
}



void ADC_start(void)
{
   ADC_SoftwareStartConv(ADC_NUM);
}




//Get ADC value if the ADC is ready
//Returns value if successful, -1 if ADC wasn't done yet
int ADC_get(void)
{
   //HACK - TESTING
   while (ADC_GetSoftwareStartConvStatus(ADC_NUM) != RESET)
      ;
   //HACK - TESTING
   
   return (ADC_GetSoftwareStartConvStatus(ADC_NUM) == RESET)
      ? ADC_GetConversionValue(ADC_NUM)
      : -1;
}
