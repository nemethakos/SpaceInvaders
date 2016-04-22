#ifndef __UTILS_H__
#define __UTILS_H__

#include "Model.h"

extern char tmp[10]; // temporary array for converting long->char[]
extern unsigned char tmpLength; // length of the string in tmp

void calculateEnemyLeftRight(void);
void playEnemyMoveSound(void);
void waitForFire(void);
void waitForNextFrame(void);
void longToChar(unsigned long number);
void getScore(char scoreStr[12]);
void wipeScreenEffect(unsigned int wipeWidth, unsigned char pattern);
void drawBlock(int x1, int x2, unsigned char color);

void happyEndAnimation(void);
void nextLevelAnimation(void);
void displayTextByFrame(int row, char text[12], int delay);
void drawBounds(STyp obj);
unsigned long characterLength(unsigned short number);

#endif

