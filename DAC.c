// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "tm4c123gh6pm.h"
#define PORTB 0x02

// Bits 5,4,3,2,1,0 - output
void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= PORTB;     // 1) activate clock for Port B
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PB4-0
  GPIO_PORTB_DIR_R = 0x3F;          // 5) 
	GPIO_PORTB_DR8R_R = 0x3F;    // can drive up to 8mA out
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) disable alt funct on PB7-0
  GPIO_PORTB_PUR_R = 0x00;          // 
  GPIO_PORTB_DEN_R = 0x3F;          // 7) enable digital I/O 
}

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
  PortB_Init();
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
   unsigned long pb = GPIO_PORTB_DATA_R & 0xfff0;
	 pb = pb |= (data & 0xf);
	 GPIO_PORTB_DATA_R = pb;
}
