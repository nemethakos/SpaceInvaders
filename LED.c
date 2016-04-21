/**
 * LED handler for Space Invaders game
 * 
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "LED.h"
#include "tm4c123gh6pm.h"

void LED_Init(void) {
	// DAC initializes PB5+PB4
}

// returns >0, if the specified led is switched on. use LED1 or LED2 from LED.h
unsigned char LED_Lights(unsigned int led) {
	return GPIO_PORTB_DATA_R &= led;
}

// lights the led if value > 0, or switches off, if value = 0
// use LED1 or LED2 from LED.h
void LED_Set(unsigned int led, unsigned char value) {
	if (value) {
		GPIO_PORTB_DATA_R |= led;
	}
	else {
		GPIO_PORTB_DATA_R &= ~led;
	}
}

// toggles the led, use constants LED1 or LED2 from LED.h
void LED_Toggle(unsigned int led) {
	GPIO_PORTB_DATA_R ^= led;
}

