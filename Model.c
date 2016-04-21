/**
 * Model implementation of Space Invaders game
 * 
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "Model.h"

// type definitions, global variables

// global variables
STyp Enemy[MAX_ENEMIES];
STyp Shot[MAX_SHOTS];
STyp Player;
STyp Bunker[MAX_BUNKERS];

// definition of the bounding box of object types for collsion detection
BTyp bBunker =     {0,0-5,17,4-5,18,5};
BTyp bLaser =      {0,1-9,0,7-9,2,9};
BTyp bMissile =    {0,1-9,3,7-9,4,9};
BTyp bPlayer =     {2,1-8,15,7-8,18,8};
BTyp bEnemy10 =    {2,1-10,13,8-10,16,10};
BTyp bEnemy20 =    {1,1-10,14,8-10,16,10};
BTyp bEnemy30 =    {3,1-10,12,8-10,16,10};
BTyp bEnemyBonus = {2,0-7,17,6-7,20,7};

// user inputs updated by SYSTICK ISR
unsigned int playerX;
unsigned char button_fire;
unsigned char button_fire_special;
// updated by SYSTICK ISR
unsigned long TimerCount;
unsigned long Semaphore;

int playerScore; // points earned by player;
int playerLifes; // numer of lifes the player has
int laserRecharge; // should be zero to fire with the laser
GameState gameState;  // game state
int level; // game level
int enemyLeft; // leftmost point of enemy block
int enemyRight; // rightmost point of enemy block
int enemyDirection; // current direction (y) of enemy block
int enemyBottom; // bottom point of enemy block
int specialEnemyBonus; // points added to playerScore, when special enemy is hit


unsigned int minFramesWaitForEnemyMoveReload; // the wait frames can be decreased until this value is reached
unsigned int framesWaitForEnemyMoveReload; // number of frames to wait between enemy moves
unsigned int framesWaitForEnemyMove; // actual variable to count
unsigned int enemyMoveCounter; // incremented when the enemies move
unsigned int shotCount; // count frames to delay enemy shots to be fired

unsigned int game_num_bunkers; // number of bunkers in current level
unsigned int game_num_enemies; // number of enemies in current level
unsigned int game_enemy_columns; // number of enemy columns in current level
unsigned int game_enemy_rows; // number of enemy rows in current level
unsigned int game_num_shots; // number of concureent shots in current level (same as number of enemies)
unsigned int game_setup_time; // number of frames to gradually display level
unsigned char game_EnemyRowType[MAX_ENEMY_ROWS]; // type of enemies in each row
unsigned int game_enemy_fire_rate; // chance to fire an enemy (less value gives hire chance)
unsigned int game_missile_update_frames; // missile position updated after this number of frames
