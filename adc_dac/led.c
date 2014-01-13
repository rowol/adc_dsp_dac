//
// LED.C
// Led driver for F4 Discovery
//
// Written by Ross Wolin/Mystic Industries
//

#include "stm32f4xx.h"

#include "led.h"


//                                     LED1                  LED2                  LED3                 LED4
const int LED_GPIO_PIN[]            = {GPIO_Pin_12,          GPIO_Pin_13,          GPIO_Pin_14,         GPIO_Pin_15,};
GPIO_TypeDef* const LED_GPIO_PORT[] = {GPIOD,                GPIOD,                GPIOD,                GPIOD,};
const int LED_GPIO_CLK[]            = {RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOD,};



      
// Configures specified LED GPIO.
void led_init(LED i)
{
// assert(LED_MIN<=i && i<=LED_MAX);
   
   GPIO_InitTypeDef  GPIO_InitStructure;
  
   /* Enable the GPIO_LED Clock */
   RCC_AHB1PeriphClockCmd(LED_GPIO_CLK[i], ENABLE);

   /* Configure the GPIO_LED pin */
   GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN[i];
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(LED_GPIO_PORT[i], &GPIO_InitStructure);

   //Start with LED off
   LED_GPIO_PORT[i]->BSRRH = LED_GPIO_PIN[i];  
}



void led_on(LED i)
{
// assert(LED_MIN<=i && i<=LED_MAX);
   LED_GPIO_PORT[i]->BSRRL = LED_GPIO_PIN[i];
}



void led_off(LED i)
{
// assert(LED_MIN<=i && i<=LED_MAX);
   LED_GPIO_PORT[i]->BSRRH = LED_GPIO_PIN[i];  
}



void led_toggle(LED i)
{
// assert(LED_MIN<=i && i<=LED_MAX);
   LED_GPIO_PORT[i]->ODR ^= LED_GPIO_PIN[i];
}
