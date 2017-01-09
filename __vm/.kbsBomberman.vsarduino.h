/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 106011
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 106011
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR

//
//
void initMain();
void showMenu();
void initGame();
void gameLoop();
void BombermanInit();
void placeBomb(Bomberman *player);
void checkBombs(Bomberman *player);
void explodeBomb(int radius, int x, int y);
Bomb * findBombOnLocation(int x, int y);
bool createExplosion(int x, int y);
void initExplosion(int x, int y);
void checkExplosions();
void walkLeft(Bomberman *player);
void walkRight(Bomberman *player);
void walkDown(Bomberman *player);
void walkUp(Bomberman *player);
void checkCollision(Bomberman *player);
void debugMap();
void drawBomb(int x, int y);
void drawBarrel(int x, int y);
void drawWall(int x, int y);
void drawPlayer(Bomberman *player);
void drawEmpty(int x, int y);
void drawExplosion(int x, int y);
void drawTime();
void drawScore();
void gameOver();
void showHighScores();
void highScores(int pl1Score, int pl2Score);
void getLetter(int highScore);
void setHighScore(int getScore, int i);
void showInstr();
void sendData(uint8_t d);
uint8_t readValue();
uint8_t readValueOld();
void ir_setup();

#include "pins_arduino.h" 
#include "arduino.h"
#include "kbsBomberman.ino"
#include "Controller.ino"
#include "infrared-full-duplex.ino"
