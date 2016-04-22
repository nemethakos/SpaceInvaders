#include "tm4c123gh6pm.h"

#ifndef __SWITCH_H__
#define __SWITCH_H__

#define PORTE 0x10

/**
 * Initialize switches on PE0 - normal fire, PE1 - special fire
 */
void Switch_Init(void);

/**
 * Returns !0, if PE0 is pushed (high), otherwise returns 0
 */
unsigned char Switch_is_Fire_Pushed(void);

/**
 * Returns !0, if PE1 is pushed (high), otherwise returns 0
 */
unsigned char Switch_is_Special_Fire_Pushed(void);

#endif
