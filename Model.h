#ifndef __MODEL_H__
#define __MODEL_H__

#include "Nokia5110.h"
#include "Bitmaps.h"

#define MAX_ENEMY_COLUMNS 5
#define MAX_ENEMY_ROWS 3
#define NUM_BONUS_ENEMY 1
#define NUM_PLAYERS 1
#define MAX_ENEMIES (MAX_ENEMY_COLUMNS * MAX_ENEMY_ROWS + NUM_BONUS_ENEMY) // 4 enemies in one row * 2 rows + bonus enemy
#define MAX_BUNKERS 3
#define MAX_SHOTS (MAX_ENEMIES + NUM_PLAYERS)
#define PLAYERSHOT (Shot[0])
#define LASER_EXPLOSION_FRAMES 5 // laser explosion stays on screen for this number of frames
#define PLAYER_DIES_FRAMES 30
#define DELAY_ENEMY_SHOTS_FRAMES (15+PLAYER_DIES_FRAMES) // enemy shooting does not begin after player dies 
// *************************** Capture image dimensions out of BMP**********
#define BUNKERW     ((unsigned char)Bunker0[18])
#define BUNKERH     ((unsigned char)Bunker0[22])
#define ENEMY30W    ((unsigned char)SmallEnemy30PointA[18])
#define ENEMY30H    ((unsigned char)SmallEnemy30PointA[22])
#define ENEMY20W    ((unsigned char)SmallEnemy20PointA[18])
#define ENEMY20H    ((unsigned char)SmallEnemy20PointA[22])
#define ENEMY10W    ((unsigned char)SmallEnemy10PointA[18])
#define ENEMY10H    ((unsigned char)SmallEnemy10PointA[22])
#define ENEMYBONUSW ((unsigned char)SmallEnemyBonus0[18])
#define ENEMYBONUSH ((unsigned char)SmallEnemyBonus0[22])
#define LASERW      ((unsigned char)Laser0[18])
#define LASERH      ((unsigned char)Laser0[22])
#define MISSILEW    ((unsigned char)Missile0[18])
#define MISSILEH    ((unsigned char)Missile0[22])
#define PLAYERW     ((unsigned char)PlayerShip0[18])
#define PLAYERH     ((unsigned char)PlayerShip0[22])
#define SMALLEXPLOSIONH ((unsigned char)SmallExplosion0[22])	
#define LIFEW ((unsigned char)Life[18])
#define LIFEH ((unsigned char)Life[22])
#define LASEREXPLOSIONW	((unsigned char)LaserExplosion[18])
#define LASEREXPLOSIONH ((unsigned char)LaserExplosion[22])	
	
#define FPS 30 // frames per seconds
#define PLAYER_MAX_X (SCREENW-PLAYERW)

#define LASER_SPEED 2 // speed of laser per frame in pixels

#define MISSILE_SPEED 1 // speed of missile

//#define MISSILE_UPDATE_BY_FRAMES 10

#define SPECIAL_ENEMY_SPEED 1 // special enemy speed

#define SPECIAL_ENEMY_RATE (FPS * 10) // every 10 seconds

#define BUNKER_NORM_LEVEL 9
#define BUNKER_MID_LEVEL 6
#define BUNKER_LOW_LEVEL 3
#define BUNKER_MAX_LIFE BUNKER_NORM_LEVEL

#define LASER_RECHARGE_FRAMES 5 // number of frames to wait between two shots with the laser

#define GAME_ENEMY10_TYPE 0
#define GAME_ENEMY20_TYPE 1
#define GAME_ENEMY30_TYPE 2

#define ENEMY_AREA_TOP LIFEH
#define BUNKER_Y SCREENH - PLAYERH - 1
#define ENEMY_AREA_BOTTOM BUNKER_Y - BUNKERH
#define ENEMY_AREA_MIDDLE ((ENEMY_AREA_BOTTOM+ENEMY_AREA_TOP)/2)

// type definitions
typedef enum {WELCOME, SETUP, PLAY, GAME_OVER, GAMEPLAY_INIT} GameState;

struct Bounds {
  int x;
	int y;
	int x1;
	int y1;
	int width;
	int height;
};
typedef struct Bounds BTyp;

struct State {
	unsigned int displayDelay; // display is delayed until TimerCount > displayDelay
	int score; // points earned by destroying the enemy
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  const unsigned char *image[4]; // ptr->image
  long life;            // 0=dead, 1>=alive
	int phase; // phase will be decreased after life become 0. if phase will be 0, the enemy won't be displayed
	unsigned char animationPhase; // initialized to 0
	BTyp bounds;
};          
typedef struct State STyp;

// global variables
extern STyp Enemy[MAX_ENEMIES];
extern STyp Shot[MAX_SHOTS];
extern STyp Player;
extern STyp Bunker[MAX_BUNKERS];

// definition of the bounding box
extern BTyp bBunker;
extern BTyp bLaser;
extern BTyp bMissile;
extern BTyp bPlayer;
extern BTyp bEnemy10;
extern BTyp bEnemy20;
extern BTyp bEnemy30;
extern BTyp bEnemyBonus;

// user inputs updated by SYSTICK ISR
extern unsigned int playerX;
extern unsigned char button_fire;
extern unsigned char button_fire_special;
// updated by SYSTICK ISR
extern unsigned long TimerCount;
extern unsigned long Semaphore;

extern int playerScore; // points earned by player;
extern int specialEnemyBonus; // score for shooting down special enemy
extern int playerLifes; // numer of lifes the player has
extern int laserRecharge; // should be zero to fire with the laser
extern GameState gameState;
extern int level; // game level
extern int enemyLeft; // leftmost point of enemy block
extern int enemyRight; // rightmost point of enemy block
extern int enemyDirection; // current direction (y) of enemy block
extern int enemyBottom; // bottom point of enemy block


extern unsigned int minFramesWaitForEnemyMoveReload; // the wait frames can be decreased until this value is reached
extern unsigned int framesWaitForEnemyMoveReload; // number of frames to wait between enemy moves
extern unsigned int framesWaitForEnemyMove; // actual variable to count
extern unsigned int enemyMoveCounter; // incremented when the enemies move
extern unsigned int shotCount; // count frames to delay enemy shots to be fired

extern unsigned int game_num_bunkers; // number of bunkers in current level
extern unsigned int game_num_enemies; // number of enemies in current level
extern unsigned int game_enemy_columns; // number of enemy columns in current level
extern unsigned int game_enemy_rows; // number of enemy rows in current level
extern unsigned int game_num_shots; // number of concureent shots in current level (same as number of enemies)
extern unsigned int game_setup_time; // number of frames to gradually display level
extern unsigned char game_EnemyRowType[MAX_ENEMY_ROWS]; // type of enemies in each row
extern unsigned int game_enemy_fire_rate; // chance to fire an enemy (less value gives hire chance)
extern unsigned int game_missile_update_frames; // missile position updated after this number of frames

#endif
