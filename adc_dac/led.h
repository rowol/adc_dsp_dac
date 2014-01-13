//
// LED.H
//

#ifndef LED_H
#define LED_H

typedef enum {LED1=0, LED2=1, LED3=2, LED4=3} LED;

#define LED_MIN LED1
#define LED_MAX LED4


void led_init(LED i);
void led_on(LED i);
void led_off(LED i);
void led_toggle(LED i);

#endif //LED_H
