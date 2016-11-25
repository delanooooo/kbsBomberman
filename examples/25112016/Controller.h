#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <SSD1331.h>
#include <S65L2F50.h>
#include <S65LPH88.h>
#include <S65LS020.h>
#include <MI0283QT2.h>
#include <MI0283QT9.h>
#include <DisplaySPI.h>
#include <DisplayI2C.h>
#include "nunchuck_funcs.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include "Bomberman.h"
// lcd 240*360

#define blockSize 21

typedef enum {
	EMPTY, WALL, BARREL, PLAYER, BOMB
} object;

object levelGrid[11][11] = {
	{WALL, WALL,   WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL},
	{WALL, PLAYER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, WALL,   WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL}
};

int playLocX = 1;
int playLocY = 1;
int bombLocX = 0;
int bombLocY = 0;
int bombStrenght = 1;
int bombAmount = 3;
int bombsPlaced = 0;


MI0283QT9 lcd; //MI0283QT9 Adapter v1
byte zBut, cBut, joyX, joyY;

//prototyping
void gameLoop();
void drawBarrel(int x, int y);
void drawEmpty();
void drawExplosion();
void drawPlayer();
void drawWall(int x, int y);
void placeBomb();

Bomberman player1;

void initController() {
	nunchuck_setpowerpins();
	nunchuck_init(); // send the initilization

	//init display
	lcd.begin();
	lcd.led(225);
	lcd.fillScreen(RGB(255, 255, 255));
	Serial.begin(9600);
	/////////////////////////////////////////////////////////////////////////////////////
	lcd.fillRect(240, 5, 76, 231, RGB(0, 0, 0));

	int pixelPosX = 4;//offset by 4 pixels
	int pixelPosY = 5;//offset by 5 pixels

	for (int y = 0; y < 11; y++) {
		//row
		for (int x = 0; x < 11; x++) {
			if (levelGrid[x][y] == WALL) {
				drawWall(pixelPosX,pixelPosY);
				} else if (levelGrid[x][y] == BARREL) {
				drawBarrel(pixelPosX,pixelPosY);
				} else if (levelGrid[x][y] == BOMB) {
				placeBomb();
				} else if (levelGrid[x][y] == PLAYER) {
				drawPlayer();
			}
			pixelPosY += blockSize;
		}
		pixelPosY = 5;
		pixelPosX += blockSize;
	}
	player1 = Bomberman();
}


//blast radius//
//To the left
//if (levelGrid[bombLocY][bombLocX - bombStrength] == BARREL || levelGrid[bombLocY][bombLocX - bombStrength] == EMPTY || levelGrid[bombLocY][bombLocX - bombStrength] == PLAYER){
//  drawExplosion();
//}
//To the right
//if (levelGrid[bombLocY][bombLocX + bombStrength] == BARREL || levelGrid[bombLocY][bombLocX + bombStrength] == EMPTY || levelGrid[bombLocY][bombLocX + bombStrength] == PLAYER){
//  drawExplosion();
//}
//To the top
//if (levelGrid[bombLocY - bombStrenght][bombLocX] == BARREL || levelGrid[bombLocY - bombStrenght][bombLocX] == EMPTY || levelGrid[bombLocY - bombStrength][bombLocX] == PLAYER){
//  drawExplosion();
//}
//To the bottom
//if (levelGrid[bombLocY + bombStrenght][bombLocX] == BARREL || levelGrid[bombLocY + bombStrenght][bombLocX] == EMPTY || levelGrid[bombLocY + bombStrength][bombLocX] == PLAYER){
//  drawExplosion();
//}

void gameLoop() {
	while(1){
		nunchuck_get_data();

		zBut = nunchuck_zbutton();
		joyX = nunchuck_joyx();
		joyY = nunchuck_joyy();

		Serial.print(joyX);
		Serial.print("  ");
		Serial.println(joyY);
		
		if (joyX == 255 && joyY == 255) {
			continue;
		}

		//        for (int o = 0; o < 11; o++) {
		//            for (int p = 0; p < 11; p++) {
		//                Serial.print(levelGrid[o][p]);
		//                Serial.print(" | ");
		//                if (levelGrid[o][p] == PLAYER) {
		//                    playLocX = p;
		//                    playLocY = o;
		//
		//                }
		//            }
		//            Serial.println("");
		//        }
		//  Serial.print(playLocX);
		//  Serial.print(" , ");
		//  Serial.println(playLocY);

		if (zBut == 1) {
			if (levelGrid[player1.y][player1.x] != BOMB) {
				placeBomb();
				bombLocX = player1.x;
				bombLocY = player1.y;
			}
		}

		if (joyX < 100)//to the left
		{
			player1.walkLeft();
		} else if (joyX > 160) // to the right
		{
			player1.walkRight();
		} else if (joyY > 160)// to the top
		{
			player1.walkUp();
		} else if (joyY < 100) // to the bottom
		{
			player1.walkDown();
		}

		//          Serial.print(playLocX);
		//          Serial.print(" , ");
		//          Serial.println(playLocY);

		Serial.print(bombLocX);
		Serial.print("    ");
		Serial.println(bombLocY);
		Serial.println(bombsPlaced);
		Serial.println(bombAmount);
	}
}

void drawBarrel(int x, int y) {
	lcd.fillRect(x, y, blockSize, blockSize, RGB(255, 0, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(0, 0, 0));
}

void drawWall(int x, int y) {
	lcd.fillRect(x, y, blockSize, blockSize, RGB(153, 76, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(102, 51, 0));
}

void drawPlayer() {
	lcd.fillRect(player1.x * blockSize + 4, player1.y * blockSize + 5, blockSize, blockSize, RGB(0, 0, 0));
}

void drawEmpty() {
	lcd.fillRect(player1.x * blockSize + 4, player1.y * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
}

void placeBomb() {
	if (bombsPlaced < bombAmount) {
		levelGrid[playLocY][playLocX] = BOMB;
		lcd.fillRect(player1.x * blockSize + 4, player1.y * blockSize + 5, blockSize, blockSize, RGB(200, 200, 0));
		bombsPlaced += 1;
	}

}

void drawExplosion() {
	lcd.fillRect(bombLocX * blockSize + 4, bombLocY * blockSize + 5, blockSize, blockSize, RGB(200, 200, 0));
}
#endif