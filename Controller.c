/**
 * Controller implementation for Space Invaders game
 * 
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "Controller.h"
#include "Model.h"
#include "Nokia5110.h"
#include "Utils.h"
#include "stdlib.h"
#include "tm4c123gh6pm.h"
#include "Switch.h"
#include "Sound.h"
#include <string.h>
#include "View.h"

// calculates the number of enemies and the number of shots
void calculateNumEnemiesAndNumShots(void) {
	game_num_enemies = NUM_BONUS_ENEMY + game_enemy_columns * game_enemy_rows;
	game_num_shots = game_num_enemies;
}

// initializes game_XXX variables based on value of level global variable
void initLevel(void) {
	switch (level) {
		case 0: 
				game_enemy_columns = 3;
				game_enemy_rows = 1;
				game_num_bunkers = 1;
		    game_EnemyRowType[0] = GAME_ENEMY10_TYPE;
				game_enemy_fire_rate = 240;
				framesWaitForEnemyMoveReload = 15;
		    minFramesWaitForEnemyMoveReload = 10;
				game_missile_update_frames = 10;
			break;
		case 1: 
				game_enemy_columns = 4;
				game_enemy_rows = 1;
				game_num_bunkers = 1;
				game_EnemyRowType[0] = GAME_ENEMY20_TYPE;
				game_enemy_fire_rate = 220;
				framesWaitForEnemyMoveReload = 14;
		    minFramesWaitForEnemyMoveReload = 9;
    		game_missile_update_frames = 9;
			break;
		case 2: 
				game_enemy_columns = 5;
				game_enemy_rows = 1;
				game_num_bunkers = 1;
		    game_EnemyRowType[0] = GAME_ENEMY30_TYPE;
				game_enemy_fire_rate = 200;
				framesWaitForEnemyMoveReload = 13;
				    minFramesWaitForEnemyMoveReload = 8;

				game_missile_update_frames = 8;
			break;
		case 3: 
				game_enemy_columns = 3;
				game_enemy_rows = 2;
				game_num_bunkers = 1;
				game_EnemyRowType[0] = GAME_ENEMY10_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY10_TYPE;
				game_enemy_fire_rate = 180;
				framesWaitForEnemyMoveReload = 12;
				    minFramesWaitForEnemyMoveReload = 7;

				game_missile_update_frames = 7;
			break;
		case 4: 
				game_enemy_columns = 4;
				game_enemy_rows = 2;
				game_num_bunkers = 2;
				game_EnemyRowType[0] = GAME_ENEMY20_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY10_TYPE;
				game_enemy_fire_rate = 160;
				framesWaitForEnemyMoveReload = 11;
				    minFramesWaitForEnemyMoveReload = 6;

				game_missile_update_frames = 6;
			break;
		case 5: 
				game_enemy_columns = 5;
				game_enemy_rows = 2;
				game_num_bunkers = 2;
				game_EnemyRowType[0] = GAME_ENEMY30_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY10_TYPE;
		    game_enemy_fire_rate = 140;
				framesWaitForEnemyMoveReload = 10;
				    minFramesWaitForEnemyMoveReload = 5;

				game_missile_update_frames = 5;
			break;
		case 6: 
				game_enemy_columns = 5;
				game_enemy_rows = 2;
				game_num_bunkers = 2;
				game_EnemyRowType[0] = GAME_ENEMY10_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY20_TYPE;
		    game_enemy_fire_rate = 100;
				framesWaitForEnemyMoveReload = 9;
				    minFramesWaitForEnemyMoveReload = 4;

				game_missile_update_frames = 4;
			break;
		case 7: 
				game_enemy_columns = 5;
				game_enemy_rows = 2;
				game_num_bunkers = 3;
				game_EnemyRowType[0] = GAME_ENEMY20_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY20_TYPE;
		    game_enemy_fire_rate = 50;
				framesWaitForEnemyMoveReload = 8;
				    minFramesWaitForEnemyMoveReload = 3;

				game_missile_update_frames = 3;
			break;
		case 8: 
				game_enemy_columns = 5;
				game_enemy_rows = 2;
				game_num_bunkers = 3;
				game_EnemyRowType[0] = GAME_ENEMY30_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY20_TYPE;
        game_enemy_fire_rate = 40;
	      framesWaitForEnemyMoveReload = 7;
				    minFramesWaitForEnemyMoveReload = 2;

				game_missile_update_frames = 2;
		break;
				case 9: 
				game_enemy_columns = 5;
				game_enemy_rows = 2;
				game_num_bunkers = 3;
			  game_EnemyRowType[0] = GAME_ENEMY30_TYPE;
		    game_EnemyRowType[1] = GAME_ENEMY30_TYPE;
        game_enemy_fire_rate = 30;
	      framesWaitForEnemyMoveReload = 6;
						    minFramesWaitForEnemyMoveReload = 1;

				game_missile_update_frames = 1;
			break;
	}
				calculateNumEnemiesAndNumShots();
				framesWaitForEnemyMove = framesWaitForEnemyMoveReload;
}

// initializes enemies
void initEnemies(void) {
	int i;
	int x = SCREENW/2-(ENEMY10W*game_enemy_columns)/2;
	int y = ENEMY_AREA_MIDDLE - (game_enemy_rows * ENEMY10H)/2;
	
	const unsigned char* image0 = 0;
	const unsigned char* image1 = 0;
	int index;
	BTyp bounds;
	int er; // enemy row
	int ec; // enemy column
	
	enemyDirection = 1;
	enemyMoveCounter = 0;
	
		// Enemy #0 is bonus enemy, initially hided
  Enemy[0].life = 0;
  Enemy[0].image[0] = SmallEnemyBonus0;
  Enemy[0].image[1] = SmallEnemyBonus0;
  Enemy[0].image[2] = SmallExplosion0;
  Enemy[0].phase = 0; 
  Enemy[0].x = 0;
  Enemy[0].y = ENEMY10H; // bonus enemy height is only 7 pixels, but the explosion is 10 pixel
	Enemy[0].bounds = bEnemyBonus;
	Enemy[0].score = 100;
	
	
		i = 0;	
	// Enemy #1-NUM_ENEMIES-1 are normal enemies
	for (er=0; er<game_enemy_rows; er++) {
		for (ec=0; ec<game_enemy_columns; ec++) {
		int score=0;
    i++; // first index should be 1
		
		Enemy[i].displayDelay = i * (game_setup_time/(game_num_enemies-1));
    Enemy[i].x = x + ec * ENEMY10W; 
    Enemy[i].y = y + (er + 1) * ENEMY10H; 
	
		index = game_EnemyRowType[er];
		if (index == 0) {
		  image0 = SmallEnemy10PointA;
			image1 = SmallEnemy10PointB;
			bounds = bEnemy10;
			score = 10;
		}
		else if (index == 1) {
		  image0 = SmallEnemy20PointA;
			image1 = SmallEnemy20PointB;
			bounds = bEnemy20;
			score = 20;
		}
		else if (index == 2) {
		  image0 = SmallEnemy30PointA;
			image1 = SmallEnemy30PointB;
			bounds = bEnemy30;
			score = 30;
		}
		
    Enemy[i].image[0] = image0;
    Enemy[i].image[1] = image1;
    Enemy[i].image[2] = SmallExplosion0;

    Enemy[i].life = 1;
		Enemy[i].phase = 0;
		Enemy[i].bounds = bounds;
		Enemy[i].score = score;
		Enemy[i].animationPhase = 0;
   } 
	}
	
	specialEnemyBonus = 100;
}

// initializes bunkers
void initBunkers(void) {
		int i;
		int step = SCREENW/game_num_bunkers;
   	int x = SCREENW/(game_num_bunkers*2)-BUNKERW/2;

	if (game_num_bunkers > 0) {
		 for (i=0; i<game_num_bunkers; i++) {
	   Bunker[i].image[2] = Bunker2;
	   Bunker[i].image[1] = Bunker1;
	   Bunker[i].image[0] = Bunker0;

		 
	   Bunker[i].x = x;
	   Bunker[i].y = SCREENH - PLAYERH-1;
	   Bunker[i].life = BUNKER_MAX_LIFE;
	   Bunker[i].phase = 0;
	   Bunker[i].bounds = bBunker;
		 x += step;		
	 }
 }
}

// initializes player
void initPlayer(void) {
	 Player.displayDelay = game_setup_time;
	 Player.image[0] = PlayerShip0;
	 Player.image[1] = BigExplosion0;
	 Player.image[2] = BigExplosion1;
	 Player.y = SCREENH-1;
	 Player.x = SCREENW/2-PLAYERW/2; // will be set by the POT
	 Player.life = 1;
	 Player.phase = 0;
	 Player.bounds = bPlayer;
}

// initializes every shot
void initShots(void) {
	int i;
		 // init shots to off, they will initialized when player or enemy will shot
	 for (i=0; i<game_num_shots; i++) {
			Shot[i].life = 0;
			Shot[i].phase = 0;
	 }
	 
	 // player shot #0
	 PLAYERSHOT.image[0] = Laser0;
	 PLAYERSHOT.image[1] = LaserExplosion;
   PLAYERSHOT.bounds = bLaser;

	 
	 // enemy shots #1-NUM_SHOTS-1
	 for (i=1; i<game_num_shots; i++) {
			Shot[i].image[0] = Missile0;
		  Shot[i].image[1] = Missile1;
		  Shot[i].bounds = bMissile;
	 }
}

// initializes the model before every level
void InitModel(void) {
	initLevel();
	initEnemies();
	initBunkers();
	initPlayer();
	initShots();
	 
	 TimerCount = 0;
	 button_fire = 0;
	 button_fire_special = 0;
	 laserRecharge = 0; // laser can fire
	 specialEnemyBonus = 10;
}

/**
 * Updates the Model based on user Inputs
 */
void updateModel() {
	int i;
	int j;
	
	 checkForNextLevel();
	
	if (Player.life == 0) {
		if (Player.phase % 5 == 0) {
		  Player.animationPhase ^= 0x01;
		}	

		Player.phase--;
		if (Player.phase <= 0) {
			// resurrect player
			if (playerLifes == 0) {
				gameOver();
			}
			else {
				Player.life = 1;
				Player.phase = 0;
			}
		}
	}
	else {
	  Player.x = playerX;
	}
	
   updateSpecialEnemy();
	

	
	// player fires
	if (Player.life > 0 && button_fire) {
		if (PLAYERSHOT.life == 0 && PLAYERSHOT.phase == 0&&laserRecharge==0) {
			  PLAYERSHOT.x = Player.x + PLAYERW/2-LASERW/2+1;
				PLAYERSHOT.y = Player.y - PLAYERH + LASER_SPEED + LASERH;
			  PLAYERSHOT.life = 1;
			  PLAYERSHOT.phase = 3;
			  laserRecharge = LASER_RECHARGE_FRAMES;
			  Sound_Shoot();
		}
	}
	
	if (laserRecharge>0) {
	  laserRecharge--;
	}
	
	// player shot move
	if (PLAYERSHOT.life > 0) {
		if (PLAYERSHOT.y > LASERH + LASER_SPEED) {
				PLAYERSHOT.y -= LASER_SPEED;
		}
		else {
			laserExplosion(0);
		}
	}	else if (PLAYERSHOT.phase > 0) {
		PLAYERSHOT.phase -= 1;		
	}

	
	// enemy shot move
	if (TimerCount % game_missile_update_frames == 0) {
		
	  for (i=1; i<game_num_shots; i++) {
	  	if (Shot[i].life > 0) {
				if (Shot[i].y < SCREENH - MISSILE_SPEED - 1) {
				  Shot[i].y += MISSILE_SPEED;
				  Shot[i].animationPhase ^= 0x01; // animate
			  }	else {
				  laserExplosion(i);
			  }
		  } else if (Shot[i].phase > 0) {
			  Shot[i].phase--;
		  }
	  }
		
  }
	
	// update enemy shot explosions at FPS rate
	for (i=1; i<game_num_shots; i++) {
	  	if (Shot[i].life == 0 && Shot[i].phase > 0) {
			  Shot[i].phase--;
		  }
	  }
	
	// check enemy shots+player+bunker
	for (i=1; i<game_num_shots; i++) {
	  if (Shot[i].life>0) {
			
			for (j=0; j<game_num_bunkers; j++) {
				
			  if (Bunker[j].life>0 && isCollide(Shot[i], Bunker[j])) {
				  Bunker[j].life -= 1;
					laserExplosion(i);
			  }
			}
			
			// only check player+enemy shot if player is not already exploded
			if (Player.phase==0 && isCollide(Player, Shot[i])) {
				Shot[i].life = 0;
				Shot[i].phase = 0;
				playerDies();// player death
			}

	  }
	}
	
	// check shots+enemies+bunker and laser
	if (PLAYERSHOT.life > 0) {
	
		for (i=0; i<game_num_bunkers; i++) {
			if (Bunker[i].life > 0) {
				if (isCollide(PLAYERSHOT, Bunker[i])) {
					Bunker[i].life -= 1;
					laserExplosion(0);
				}
			}
		}
		
		for (i=1; i<game_num_shots; i++) {
		  if (Shot[i].life > 0) {
		    if (isCollide(PLAYERSHOT, Shot[i])) {
				  Shot[i].life = 0;
					laserExplosion(0);
				}
		  }
		}
		

		
		for (i=0; i<game_num_enemies; i++) {
			if (Enemy[i].life > 0) {

					if (isCollide(PLAYERSHOT,  Enemy[i])) {
						PLAYERSHOT.life = 0;
						PLAYERSHOT.phase = 0;
						
						Enemy[i].life = 0;
						Enemy[i].phase = 5;
						playerScore += Enemy[i].score;
						// speed up enemies
						if (framesWaitForEnemyMoveReload > minFramesWaitForEnemyMoveReload) {
						  framesWaitForEnemyMoveReload--;
						}
						if (i==0) {
						// bonus life
							playerLifes++;
							specialEnemyBonus *= 2;
						}
						break;
					}

		  }
    }
		
   }
	
	 
	
	 // update phase on exploded enemies
  for (i=0; i<game_num_enemies; i++) {
		if (Enemy[i].life == 0 && Enemy[i].phase > 0) {
			Enemy[i].phase--;
		}
	} 
 
   // delay shooting	
	if (shotCount > DELAY_ENEMY_SHOTS_FRAMES) {
	// shot by enemies
	for ( i=1; i<game_num_enemies; i++) {
		// if an enemy is alive but there is no shot fired yet
		if (isEnemyCanFire(i) && Enemy[i].life > 0 && Shot[i].life == 0 && Shot[i].phase==0) {
			// fire
			if (rand() % game_enemy_fire_rate == 0) {
				   Shot[i].x = Enemy[i].x + Enemy[i].bounds.width / 2 - Shot[i].bounds.width / 2;
				   Shot[i].y = Enemy[i].y + 1; //Shot[i].bounds.height;
				   Shot[i].life = 1;
				   Shot[i].animationPhase = 0;
				   Shot[i].phase = 0;
			}
		}
	}
 }
	
 if (framesWaitForEnemyMove == 0) {
	 
    framesWaitForEnemyMove = framesWaitForEnemyMoveReload;
 		
	 // move enemies
		calculateEnemyLeftRight();
 
    // if enemies reach top of player, game ends
		if (enemyBottom >= SCREENH - PLAYERH) {
			gameOver();
		}
 
		// right
		if (enemyDirection > 0) {
		  if (enemyRight >= SCREENW - 1) {
				// step down
				enemyDirection = -1;
				moveEnemies(0, 1);

			}
			else {
				// step right
				moveEnemies(enemyDirection, 0);

			}
		}
		// left
		else {
			if (enemyLeft <= 0) {
				// step down
				enemyDirection = 1;
				moveEnemies(0, 1);

			}
			else {
				// step left
				moveEnemies(enemyDirection, 0);

			}
		}
		
		// enemies destroys bunker when touched
			for (i=0; i<game_num_enemies; i++) {
			if (Enemy[i].life > 0) {

				for (j=0; j<game_num_bunkers; j++) {
				
					if (Bunker[j].life > 0) {
					if (isCollide(Enemy[i],  Bunker[j])) {
						// Enemy is destroyed
						Enemy[i].life = 0;
						Enemy[i].phase = 5;
						// Bunker is destroyed
						Bunker[j].life = 0;
						Bunker[j].phase = 0;	
						break;
					}
				}
		  }}
    }
			
	}
 framesWaitForEnemyMove--;
		shotCount++;
}

// Update special enemy (on the top of the screen)
void updateSpecialEnemy(void) {
		// move special enemy
	if (Enemy[0].life > 0) {
		if (Enemy[0].x + ENEMYBONUSW + SPECIAL_ENEMY_SPEED < SCREENW-1) {
			Enemy[0].x += SPECIAL_ENEMY_SPEED;
		}
		else {
		  Enemy[0].life = 0;
			Enemy[0].phase = 0; // no explosion
		}
	}
	
		// spawn special enemy
	if (Enemy[0].life == 0 && isSpecialEnemyCanBeStarted()) {
	  if (rand() % SPECIAL_ENEMY_RATE == 0) {
			Enemy[0].score = specialEnemyBonus;
			Enemy[0].x = 0;
			Enemy[0].life = 1;
			Enemy[0].phase = 0;
			Sound_Highpitch();
		}
	}
}

// the special enemy can be started if there is enough space in the screen
// 1. the first enemy is below the first line
// 2. all enemies in the first row are dead
unsigned char isSpecialEnemyCanBeStarted() {
	int i;
	
	if (Enemy[1].y > 2 * ENEMY10H) {
		return 1;
	}
	for (i=1; i<game_enemy_columns; i++) {
		if (Enemy[i].life > 0 || Enemy[i].phase > 0) {
			return 0;
		}
	}
	return 1;
}

// moves live enemies
void moveEnemies(int x, int y) {
	int i;
	enemyMoveCounter++;
	playEnemyMoveSound();
	for (i=1; i<game_num_enemies; i++) {
		if (Enemy[i].life > 0 && Enemy[i].phase == 0) {
			Enemy[i].x += x;
			Enemy[i].y += y;
			Enemy[i].animationPhase = enemyMoveCounter & 0x01;
		}
	}
}

// Returns 1 if the enemy at the given index could fire.
// This means the enemy at index + ENEMY_COLUMNS.life = 0 & phase = 0
unsigned char isEnemyCanFire(int index) {
	int positionToCheck = index + game_enemy_columns;
	if (positionToCheck >= game_num_enemies) {
		return 1;
	}
	if (Enemy[positionToCheck].life == 0 && Enemy[positionToCheck].phase == 0) {
	  return 1;
	}
	return 0;
}



// Check if the game should be switched to the next level
void checkForNextLevel(void) {
	int liveEnemyCount;
	int i;
	
	liveEnemyCount = 0;
	 for (i=0; i<game_num_enemies; i++) {
		 if (Enemy[i].life > 0 || Enemy[i].phase > 0) {
				  liveEnemyCount++;
		 }
	 }
	 if (liveEnemyCount == 0) {
	   nextLevel();
	 }
}

// called when game is over
void gameOver() {
	gameState = GAME_OVER;
}



// switch to next level
void nextLevel() {
	if (level == 8) {
		happyEndAnimation();
		gameState = WELCOME;
	} else {
	nextLevelAnimation();
	level++;

	gameState = SETUP;
	InitModel();
	}
}

// explodes the shot given by the index
void laserExplosion(unsigned char index) {
	Shot[index].life = 0;
	Shot[index].phase = LASER_EXPLOSION_FRAMES;
	Sound_Explosion();
}

// remove enemy shots (after player died) to not to kill the resurrected player
void removeShots(void) {
int i;
	for (i=0; i<game_num_shots; i++) {
		Shot[i].life = 0;
		Shot[i].phase = 0;
	}
}

//called when player dies
void playerDies() {
	removeShots();
	shotCount = 0;
	Player.life = 0;
	Player.phase = PLAYER_DIES_FRAMES;
	Player.animationPhase =0;
	playerLifes--;
	Sound_Killed();
}

// Play loop
void doPlay(){
  while(gameState==PLAY){
		updateModel();
		updateDisplay();
    waitForNextFrame();
	}
};

// Displays the playfield gradually
void doSetup() {
	
	InitModel();
	while (TimerCount<game_setup_time) {
	  updateDisplay();
		waitForNextFrame();
	}
	gameState=PLAY;
	srand(NVIC_ST_CURRENT_R); // random seed
	while (button_fire) {}; // wait until fire is released
}

// initializes the gameplay (once in every game)
void doGamePlayInit(void) {
	level = 0;
	playerScore = 0;
	playerLifes = 3;
	gameState = SETUP;
	game_setup_time = 1 * FPS;
}

// main game loop
void gameLoop() {
	while(1) {
		switch(gameState) {
			case WELCOME: doWelcomeScreen();
				    gameState = GAMEPLAY_INIT;
						break;
			case GAMEPLAY_INIT: doGamePlayInit();
						break;
			case SETUP: doSetup();
						break;
			case PLAY: doPlay();
						break;
			case GAME_OVER: doGameOver();
				    gameState = WELCOME;
						break;
		}
	}
}

// returns 1, if enemy shot is on collision course with player
unsigned char playerIsInDanger(void) {
int i;
STyp obj;

	
	for (i=1; i<game_num_enemies; i++) {
		if (Shot[i].life > 0) {
		  memcpy(&obj, &Shot[i], sizeof(STyp)); // make copy of shot
		  obj.y = Player.y; // translate shot, to be on the same vertical position as the player
			if (isCollide(obj, Player)) {
			  return 1;
			}
		}
	}
	return 0;
}

// returns 1 if obj1 has overlapping part of obj2
unsigned char isCollide(STyp obj1, STyp obj2) {
	
	int ax =  obj1.x+obj1.bounds.x;
	int ay =  obj1.y+obj1.bounds.y;
	int ax1 = obj1.x+obj1.bounds.x1;
	int ay1 = obj1.y+obj1.bounds.y1;
	int aw =  ax1 - ax;
	int ah =  ay1 - ay;
	
	int bx =  obj2.x+obj2.bounds.x;
	int by =  obj2.y+obj2.bounds.y;
	int bx1 = obj2.x+obj2.bounds.x1;
	int by1 = obj2.y+obj2.bounds.y1;
	int bw =  bx1 - bx;
	int bh =  by1 - by;
	
	if ( !(ax > bx+bw || ax+aw < bx || ay > by+bh || ay+ah < by)) {
	  return 1;
	}
  else {
		return 0;
	}
	
}
