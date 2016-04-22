#ifndef __LED_H__
#define __LED_H__

#define PORTB 0x02

// LED on PB4
#define LED1 0x10

// LED on PB5
#define LED2 0x20

void LED_Init(void);

// returns >0, if the specified led is switched on. use LED1 or LED2 from LED.h
unsigned char LED_Lights(unsigned int led);

void LED_Set(unsigned int led, unsigned char value);

void LED_Toggle(unsigned int led);

#endif
