//
// HW.H
// General hardware definitions
//

#ifndef HW_H
#define HW_H

#define APB1_FREQ        	84000000
#define SAMPLE_FREQ           44000

//Define the 12 bit DAC converter's range
#define DAC_MIN    0
#define DAC_MID    (1<<11)         //Middle
#define DAC_MAX    ((1<<12) - 1)


#endif //HW_H
