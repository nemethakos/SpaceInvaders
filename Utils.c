/**
 * Utility functions 
 * for Space Invaders game for UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "Utils.h"
#include "Model.h"
#include "Nokia5110.h"
#include "Bitmaps.h"
#include "Sound.h"
#include "Switch.h"

char tmp[10]; // temporary array for converting long->char[]
unsigned char tmpLength; // length of the string in tmp

// calculates the left, right and bottom side of the enemy block
// should be called after an enemy is destroyed/enemies moved
void calculateEnemyLeftRight(void) {
int i;
  enemyLeft = SCREENW;
  enemyRight = 0;	
	enemyBottom = 0;
	for (i=1; i<game_num_enemies; i++) {
		if (Enemy[i].life > 0) {
			if (Enemy[i].x < enemyLeft) {
				enemyLeft = Enemy[i].x;
			}
			if (Enemy[i].x + ENEMY10W > enemyRight) {
				enemyRight = Enemy[i].x + ENEMY10W;
			}
			if (Enemy[i].y > enemyBottom) {
				enemyBottom = Enemy[i].y;
			}
	}}
}

// Plays the appropriate sound for the current phase of the enemy moves
void playEnemyMoveSound() {
int i = enemyMoveCounter % 4;
	switch (i) {
		case 0: Sound_Fastinvader1();
		break;
		case 1: Sound_Fastinvader2();
		break;
		case 2: Sound_Fastinvader3();
		break;
		case 3: Sound_Fastinvader4();
		break;
			
	}
}

// waits for fire to be pushed and released
void waitForFire() {
	// wait until fire is pushed
	while (!button_fire) {
	}
	// wait until fire is released
	while (button_fire) {
	}
}

// wait for the next frame to be displayed
void waitForNextFrame() {
		Semaphore = 0;
		while (Semaphore==0) {
		  // wait
		};
}

// converts number to the tmp array
void longToChar(unsigned long number) {
  int i;
  char fill = ' ';
  int digit;
	unsigned long num = number;
	int numberOfDigits = 0;
	
	for (i=0; i<10; i++) {
		tmp[i]=fill;
	}

	// calculate number of digits
	while (num != 0) {
		num = num / 10;
		numberOfDigits++;
	}
	
	if (numberOfDigits==0) {
		numberOfDigits=1;
	}
	
	for (i=numberOfDigits-1; i>=0; i--) {
		digit = number % 10;
		tmp[i] = (char)digit + 0x30;
		number = number / 10;
	}
	tmpLength = numberOfDigits;
}

// converts playerScore to 12 character long String
void getScore(char scoreStr[12]) {
	#define SCORE_TEXT "Score:"
	#define SCORE_LENGTH (sizeof(SCORE_TEXT)-1)
  #define MID_SCREEN_X 6
	int i;
	int startPos;
	int numPos;
	char scoreText[7] = SCORE_TEXT;
	if (playerScore>999999) {
		playerScore=999999;
	}
	longToChar(playerScore);
	
	startPos = MID_SCREEN_X - (tmpLength + SCORE_LENGTH) / 2;
	
	for (i=0; i<SCORE_LENGTH; i++) {
		scoreStr[i+startPos] = scoreText[i];
	}
	numPos = SCORE_LENGTH + startPos;
	
	for (i=0; i<tmpLength; i++) {
		scoreStr[i+numPos] = tmp[i];
	}	
}



// draws a block from column x1 to x2 with the given color (for the wipe effect)
void drawBlock(int x1, int x2, unsigned char color) {
	int x;
	int bank;
	if (x1>SCREENW-1) {
		x1 = SCREENW-1;
	}
	if (x2>SCREENW-1) {
		x2=SCREENW-1;
	}
  for (x = x1; x < x2; x++) {
		for (bank=0; bank <= 5; bank ++) {
			Screen[bank * SCREENW + x] = color;
		}
	}
}

// displays a wipe effect
void wipeScreenEffect(unsigned int wipeWidth, unsigned char pattern) {
	int x1 = 0;
	int x2 = wipeWidth;

	while (x2 < SCREENW) {
		
		drawBlock(x1, x2, pattern);
		x1 = x2;
		x2 += wipeWidth;
		Nokia5110_DisplayBuffer();
		waitForNextFrame();

}
}

// displays the 12 characters on screen frame by frame
// row - 0-3
// text - the text. Space characters don't overlap existing content. Use underscore for space
// delay - how much frames is delayed between characters
void displayTextByFrame(int row, char text[12], int delay) {
int i;
	int j;
	char c;
  for (i=0;i<12;i++) {
		Nokia5110_SetCursor(i, row);
		c = text[i];
		if (c != ' ') {
		if (c == '_') {
			c = ' ';
		}
		Nokia5110_OutChar(c);
		for (j=0; j<delay; j++) {
	  	Semaphore = 0;
		  while (Semaphore==0);	
		}			
	}
	}
}

// animation for next level
void nextLevelAnimation(void) {

	// char 8 is the level number
  char row1[12]  = "  Level_X   ";
  char row2[12]  = " Completed! ";
	char row3[12]  = "            ";
	char row4[12]  = "_Press_Fire_"; 
  char row5[12]  = "to_continue!"; 
	
  row1[8] = '1' + level;
	
	displayTextByFrame(1, row1, 1);
	displayTextByFrame(2, row2, 1);
	
	getScore(row3);
	displayTextByFrame(3, row3, 1);
	displayTextByFrame(4, row4, 1);
	displayTextByFrame(5, row5, 1);
	
	
	waitForFire();
	
}

// animation for happy end
void happyEndAnimation(void) {

	// char 8 is the level number__
  char row0[12]  = "__Earth_is__";
  char row1[12]  = "_saved_from_";
	char row2[12]  = "___aliens!__";
  char score[12] = "____________"; 
	char row4[12]  = "_Press_Fire_"; 
  char row5[12]  = "to_continue!"; 
	
	displayTextByFrame(0, row0, 1);
	displayTextByFrame(1, row1, 1);
	displayTextByFrame(2, row2, 1);
	
	getScore(score);
	displayTextByFrame(3, score, 1);
	displayTextByFrame(4, row4, 1);
	displayTextByFrame(5, row5, 1);
	
	
	waitForFire();
	
}

// draw the bound of obj for debugging
void drawBounds(STyp obj) {
     Nokia5110_rectangle(
	obj.x+obj.bounds.x, 
	obj.y+obj.bounds.y, 
	obj.x+obj.bounds.x1, 
	obj.y+obj.bounds.y1, 1);
}

// returns the length of the number in characters
unsigned long characterLength(unsigned short number) {
	if (number < 10) {
		return 1;
	} else if (number < 100) {
		return 2;
	} else if (number < 1000) {
		return 3;
	} else if (number < 10000){
		return 4;
	} else {
		return 5;
	}
}

