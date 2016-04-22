/**
 * View implementation of Space Invaders game
 *
 * Copyright 2016 by Ákos Németh
 * You may use, edit, run or distribute this file
 * as long as the above copyright notice remains.
 */

#include "View.h"
#include "Model.h"
#include "LED.h"
#include "Controller.h"
#include "Utils.h"

// draw a single shot (not in exploded state)
void drawShot(int i) {
    STyp shot = Shot[i];
    if (shot.life > 0) {
        Nokia5110_PrintBMP(shot.x, shot.y, shot.image[shot.animationPhase], 0, 1);
    }
}

// draw exploded shots
void drawExplosions() {
    int i;
    for (i=0; i<game_num_shots; i++) {
        STyp shot = Shot[i];
        if (shot.life ==0 && i == 0 && shot.phase > 0) {
            int x = shot.x + LASERW/2 -LASEREXPLOSIONW/2;
            int y = shot.y - LASERH + LASEREXPLOSIONH/2;
            if (x<0) {
                x=0;
            }
            if (y<LASEREXPLOSIONH) {
                y=LASEREXPLOSIONH;
            }
            Nokia5110_PrintBMP(x, y, LaserExplosion, 0,1);
        } else if (i > 0 && shot.phase > 0) {
            // enemy shot explosion
            int x = shot.x + LASERW/2 -LASEREXPLOSIONW/2;
            int y = shot.y - LASERH/2 + LASEREXPLOSIONH/2;
            if (x<0) {
                x=0;
            }
            if (y>SCREENH-1) {
                y=SCREENH-1;
            }
            Nokia5110_PrintBMP(x, y, LaserExplosion, 0,1);
        }
    }
}



// Red led (1) is flased when player is in danger (enemy shot is coming)
// Green Led (2) is lighting when bonus enemy is on the screen
void updateLEDs() {

    unsigned char blinkRedLed = playerIsInDanger();
    unsigned char redLedOn = 0;
    unsigned char greenLedOn = Enemy[0].life > 0;

    if (blinkRedLed && (TimerCount % 10 == 9)) {
        redLedOn = 1;
    }

    LED_Set(LED1, redLedOn);
    LED_Set(LED2, greenLedOn);
}


/**
Updates the display based on the Model

*/
void updateDisplay() {
    int i;
    int maxPlayerLife;

    Nokia5110_ClearBuffer();

    // draw shots
    for (i=1; i<game_num_shots; i++) {
        drawShot(i);
    }

    // draw enemies
    for(i=0; i<game_num_enemies; i++) {
        // draw enemies gradually in SETUP
        if (Enemy[i].displayDelay < TimerCount) {
            STyp enemy = Enemy[i];
            if(enemy.life > 0) {
                // image 0 & 1 are animation phases of enemies
                Nokia5110_PrintBMP(enemy.x, enemy.y, enemy.image[enemy.animationPhase], 0,1);
            } else {
                if (enemy.phase > 0) {
                    // image 2 is the explosion image
                    Nokia5110_PrintBMP(enemy.x, enemy.y, enemy.image[2], 0,1);
                }
            }
        }
    }
    // draw bunker
    for (i=0; i<game_num_bunkers; i++) {
        STyp bunker = Bunker[i];

        if (bunker.life>0) {
            int bunkerImageIndex;
            if (bunker.life >= BUNKER_NORM_LEVEL) {
                bunkerImageIndex = 0;
            } else if (bunker.life >= BUNKER_MID_LEVEL) {
                bunkerImageIndex = 1;
            } else {
                bunkerImageIndex = 2;
            }
            // bunker.life is used as an index to the image of bunker
            Nokia5110_PrintBMP(bunker.x, bunker.y, bunker.image[bunkerImageIndex], 0,1);
        }
    }



    if (Player.displayDelay < TimerCount) {
        // draw player
        if (Player.life>0) {
            Nokia5110_PrintBMP(Player.x, Player.y, Player.image[0], 0,1);

        } else {
            // explosion
            if (Player.phase>0) {
                // image 1&2 are explosion of player
                Nokia5110_PrintBMP(Player.x, Player.y, Player.image[Player.animationPhase + 1], 0,1);
            }
        }
    }

    maxPlayerLife = playerLifes;
    if (maxPlayerLife>10) {
        maxPlayerLife=10;
    }
    for (i=0; i<maxPlayerLife; i++) {
        Nokia5110_PrintBMP(SCREENW-1-(i+1)*LIFEW, LIFEH, Life, 0,1);
    }


    drawShot(0);
    drawExplosions();

    Nokia5110_DisplayBuffer();
    updateLEDs();

}

// Welcome screen animation
void doWelcomeScreen() {
    unsigned char button_state = 0;
    unsigned char exit = 0;
    unsigned char firePushed = 0;
    unsigned char fireReleased = 0;
    int i;
    const unsigned char* enemy1[2] = {SmallEnemy10PointA, SmallEnemy10PointB};
    const unsigned char* enemy2[2] = {SmallEnemy20PointA, SmallEnemy20PointB};

    unsigned char phase = 0;
    int y = 0;
    int dir = 1;
    int ufoX = 0;
    int ufoDir = 1;

    // copy title to display buffer
    for (i=0; i<sizeof(Screen); i++) {
        Screen[i] = title[i];
    }
    while (!exit) {

        button_state = button_fire;
        if (firePushed && !button_state) {
            fireReleased = 1;
        }
        if (button_state) {
            firePushed = 1;
        }

        exit = firePushed && fireReleased;

        if (TimerCount % (FPS/4) == 0) {
            phase = phase ^= 0x01;
            y+=dir;
            if (y>=ENEMY10H-1) {
                dir = -1;
                playEnemyMoveSound();
            }
            if (y<=0) {
                dir = 1;
                playEnemyMoveSound();
            }
        }
        if (TimerCount % 2 == 0) {
            ufoX += ufoDir;
            if (ufoX >= SCREENW-ENEMYBONUSW) {
                ufoDir = -1;
                playEnemyMoveSound();
            }
            if (ufoX <= 0) {
                ufoDir = 1;
                playEnemyMoveSound();
            }
        }

        Nokia5110_PrintBMP(0,y+ENEMY10H ,enemy1[phase], 0, 0);
        Nokia5110_PrintBMP(SCREENW-ENEMY10W,2*ENEMY10H-y,enemy2[phase], 0, 0);
        Nokia5110_PrintBMP(ufoX,39, SmallEnemyBonus0, 0, 0);
        Nokia5110_DisplayBuffer();

        waitForNextFrame();


    }

    wipeScreenEffect(2, 0x00);
};

// Game Over screen
void doGameOver() {
    int i;
    // char 8 is the level number
    char row0[12]  = "_Game_Over!_";
    char row1[12]  = "____________";
    char row2[12]  = "____________";

    char row4[12]  = "_Press_Fire_";
    char row5[12]  = "to_continue!";

    getScore(row2);

    displayTextByFrame(0, row0, 1);
    displayTextByFrame(1, row1, 1);
    displayTextByFrame(2, row2, 1);
    displayTextByFrame(3, row1, 1);
    displayTextByFrame(4, row4, 1);
    displayTextByFrame(5, row5, 1);

    for (i=0; i<30; i++) {
        Semaphore = 0;
        while (Semaphore==0);
    }

    waitForFire();

}
