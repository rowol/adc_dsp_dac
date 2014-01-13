//
// TIMER.H
// Can be used to generate ms delays
//

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>


typedef struct 
{
   uint32_t dwStart;
   uint32_t dwInterval;    //in milliseconds
} TMR;

   
void tmr_setInterval(TMR* p, uint32_t dwNewInterval);       //Also resets the timer

void tmr_sleep(uint32_t dwInterval);
void tmr_reset(TMR* p);                                     //Resets timer, keeps interval



//Check if timer interval has expired based on locally stored start time.
//Guarantees at least wTicks full ticks have passed since wStartTick
//
//(Note: > used rather than >= because when you grab the start tick, you
//have no idea how far into the tick the timer was, so you have to wait
//at least wTicks + the partial first tick to assure the wTicks interval
//has expired.
//
inline bool tmr_isExpired(TMR* p);


void tmr_inc();             //ISR calls this to increment stored global tick counter

#endif //TIMER_H
