# Space Invaders

Space Invaders game for Lab15/UTAustinX: UT.6.03x Embedded Systems -  Shape the World

### How to compile

Download the zippped source files in any directory in your local machine (Keil 4.x should have been already installed), open the SpaceInvaders.uvproj file and compile the source files.

### The game

The game has 9 levels, each with different types of enemies and bunker arrangement. To save the earth from aliens the player has to defeat all aliens in all levels. Player receives score for all enemies destroyed:
- 10 points for big invader 
- 20 points for middle invader 
- 30 for small invader
- 10 for enemy missiles

For destroying the bonus enemy on the top of the screen, player initially receives a bonus life and 100 points, which doubles after each succesfull attempt. Enemies can destroy the bunkers with multiple shots, but also player can destroy the bunkers by shooting at.
Enemies can also destroy bunkers when they touch them. When enemies reach the earth level the game is over. 

LED1 (red) flashes when an enemy missile is heading for the player. LED2 (green) lights when bonus enemy is on the screen.

### Game strategy

To earn the most points, leave one enemy in each level, and shoot as much bonus enemy as possible. Also shooting the enemy missiles adds 10 points to the player's score.

### Troubleshooting

In case the game doesn't compile or has runtime errors, please write to akos.space.invaders@gmail.com
