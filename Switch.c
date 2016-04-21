/**
 * Switch handler
 * 
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "Switch.h"

// Undefine NEGATIVE_LOGIC, if the switches are used as positive logic (3.3v = pushed, 0v = released)
// Define NEGATIVE_LOGIC, if the switches are used as negative logic (3.3v = released, 0v = pushed)
//#define NEGATIVE_LOGIC

// PE0 
#define FIRE 0x01

// PE1
#define SPECIAL_FIRE 0x02

// Bits 2,1,0 - input
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= PORTE;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTE_DIR_R = 0x00;          // 5) 
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
	
#ifdef NEGATIVE_LOGIC	
	
  GPIO_PORTE_PUR_R = 0x03;          // pull up enabled for the two switches when negative logic is used

#else
	
  GPIO_PORTE_PUR_R = 0x00;          // pull up disabled
	
#endif	
  GPIO_PORTE_DEN_R = 0x1F;          // 7) enable digital I/O 
}

void Switch_Init(void) {
	PortE_Init();
}

// switches are used as negative logic
#ifdef NEGATIVE_LOGIC

unsigned char Switch_is_Fire_Pushed(void) {
  return ((GPIO_PORTE_DATA_R & FIRE) == 0x00) 
		  || ((GPIO_PORTE_DATA_R & SPECIAL_FIRE) == 0x00);
}
	
unsigned char Switch_is_Special_Fire_Pushed(void) {
  return ((GPIO_PORTE_DATA_R & SPECIAL_FIRE) == 0x00);
}

// switches are used as positive logic
#else

unsigned char Switch_is_Fire_Pushed(void) {
  return ((GPIO_PORTE_DATA_R & FIRE) != 0x00) 
		  || ((GPIO_PORTE_DATA_R & SPECIAL_FIRE) != 0x00);
}
	
unsigned char Switch_is_Special_Fire_Pushed(void) {
  return ((GPIO_PORTE_DATA_R & SPECIAL_FIRE) != 0x00);
}

#endif
