#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Model.h"

void calculateNumEnemiesAndNumShots(void);
void initLevel(void);
void InitModel(void);
void initShots(void);
void initPlayer(void);
void initBunkers(void);
void initEnemies(void);
void initLevel(void);
void updateModel(void);
void updateSpecialEnemy(void) ;
void moveEnemies(int x, int y);
unsigned char isSpecialEnemyCanBeStarted(void);
void checkForNextLevel(void);
unsigned char isEnemyCanFire(int index);
void nextLevel(void);
void gameOver(void);
void playerDies(void);
void removeShots(void);
void laserExplosion(unsigned char index) ;
void gameLoop(void);
void doGamePlayInit(void);
unsigned char playerIsInDanger(void);
unsigned char isCollide(STyp obj1, STyp obj2);

#endif
