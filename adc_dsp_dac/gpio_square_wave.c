//
// GPIO_SQUARE_WAVE.C
// Configure PC.4 as a GPIO output, used to manually generate a square wave
//
// Written by Ross Wolin
//

#include <stdbool.h>

#include "stm32f4xx.h"

#include "gpio_square_wave.h"



#define IO_PIN       GPIO_Pin_4
#define IO_PORT      GPIOC
#define IO_PORT_CLK  RCC_AHB1Periph_GPIOC


void gsw_init(void)
{
  
   /* Enable the clock */
   RCC_AHB1PeriphClockCmd(IO_PORT_CLK, ENABLE);

   /* Configure the pin */
   GPIO_InitTypeDef  GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = IO_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(IO_PORT, &GPIO_InitStructure);

   //Start with signal low
   IO_PORT->BSRRH = IO_PIN;  
}



void gsw_toggle(void)
{
   IO_PORT->ODR ^= IO_PIN;  
}

