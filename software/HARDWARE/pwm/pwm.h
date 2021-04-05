#ifndef __PWM_H
#define __PWM_H

#include "sys.h"


typedef struct 
{
    int16_t yaw;
    int16_t throttle;
    int16_t roll;
    int16_t pith;
}RCData_t;


void PWM_DeModulation(void);


#endif
