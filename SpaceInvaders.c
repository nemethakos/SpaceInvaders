/**
 * Main file
 * for Space Invaders game for UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "ADC.h"
#include "Switch.h"
#include "Sound.h"
#include "Timer2.h"
#include "Controller.h"
#include "TExaS.h"

// function prototypes
void SysTick_Init(unsigned long period);
void PLL_Init(void);


int main(void){
	TExaS_Init(  NoLCD_NoScope);
	//PLL_Init();
	ADC0_Init();
	Switch_Init(); // PE0, PE1 - input
  Nokia5110_Init();
  Sound_Init();
	SysTick_Init(80000000/30);
	
  gameLoop();
}

/**
* Updates Player position and button states
*/
void updateInputs() {
	playerX = (ADC0_In()*PLAYER_MAX_X)/ADC_MAX;
	button_fire = Switch_is_Fire_Pushed();
	button_fire_special = Switch_is_Special_Fire_Pushed();
}

// Initialize SysTick interrupts to trigger at 30 Hz
void SysTick_Init(unsigned long period){
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

// executes 30Hz
void SysTick_Handler(void){ 
	updateInputs();
	TimerCount++;
	Semaphore = 1;
}

