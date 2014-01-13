//
// TIMER.C
// 1ms timer, counter runs off SysTick ISR
//
// Written by Ross Wolin/Mystic Industries
//

#include "timer.h"

volatile uint32_t g_ctTicks = 0; 

//Called from the SysTick interrupt handler
inline void tmr_inc(void)
{
	g_ctTicks++;
}


//
//Returns tick count from event timer.  
//
//Could possibly change while we are reading the value, since this is interrupt driven. 
//(This is fairly common practice when it's desired not to stop the timer.)
//
//Returns 32 bit tick count
// 
static uint32_t tmr_get(void)
{
   uint32_t ctTicks;

   do {
      ctTicks = g_ctTicks;
   } while (ctTicks != g_ctTicks);        //Check if value changed
      
   return ctTicks;
}



inline bool tmr_isExpired(TMR* p)
{
   return (tmr_get() - (p->dwStart) > (p->dwInterval));
}



//Reset the timer, and change the interval if dwNewInterval is not zero
void tmr_setInterval(TMR* p, uint32_t dwNewInterval)
{
   if (dwNewInterval != 0)
      p->dwInterval = dwNewInterval;
   
   tmr_reset(p);
}


//Reset the timer
void tmr_reset(TMR* p)
{
   p->dwStart = tmr_get();
}



//Sit and spin for dwInterval milliseconds
void tmr_sleep(uint32_t dwInterval)
{
   TMR p;

   tmr_setInterval(&p, dwInterval);
   while (!tmr_isExpired(&p))
      ;
}



