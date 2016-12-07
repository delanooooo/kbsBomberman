#pragma once

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
// lcd 240*360

#define blockSize 21

struct Bomb{
	int x;
	int y;
	int countdown;
	struct Bomb *next;
};

struct Bomberman{
	int x;
	int y;
	int bombRadius;
	int bombMaxAmount;
	int bombsPlaced;
	struct Bomb *head;
}player1, player2;

typedef enum {
	EMPTY, WALL, BARREL, PLAYER, BOMB
} object;

object levelGrid[11][11] = {
	{WALL, WALL,   WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,   WALL},
	{WALL, PLAYER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY,  WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY,  WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY,  WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  WALL},
	{WALL, EMPTY,  WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY, WALL,  EMPTY,  WALL},
	{WALL, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PLAYER, WALL},
	{WALL, WALL,   WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,  WALL,   WALL}
};

MI0283QT9 lcd; //MI0283QT9 Adapter v1
byte zBut, cBut, joyX, joyY;

//prototyping
void gameLoop();
void drawBarrel(int x, int y);
void drawEmpty(Bomberman *player);
void checkExplosions(Bomberman *player);
void drawExplosion(Bomb *b);
void drawPlayer(Bomberman *player);
void drawWall(int x, int y);
void placeBomb(Bomberman *player);
void BombermanInit();
void walkDown(Bomberman *player);
void walkLeft(Bomberman *player);
void walkRight(Bomberman *player);
void walkUp(Bomberman *player);
void debugMap();
void drawTime();

uint16_t timer;
uint8_t secondsTimer = 110;

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

	//init timer
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);//1024 prescale
	OCR0A = 156; //approximately every 10 microseconds
	TIMSK0 |= (1 << OCIE0A); //enable timer compare match interupt

	BombermanInit();//init player1 & player2

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
				placeBomb(&player1);
				} else if (x == player1.x && y == player1.y) {
				drawPlayer(&player1);
				} else if (x == player2.x && y == player2.y){
				drawPlayer(&player2);
			}
			pixelPosY += blockSize;
		}
		pixelPosY = 5;
		pixelPosX += blockSize;
	}
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
		
		_delay_ms(10);
		//Serial.print(joyX);
		//Serial.print("  ");
		//Serial.println(joyY);
		
		if (joyX == 255 && joyY == 255) {
			continue;
		}

		if (zBut == 1) {
			placeBomb(&player1);
			placeBomb(&player2);
		}

		if (joyX < 100)//to the left
		{
			walkLeft(&player1);
			walkLeft(&player2);
		} else if (joyX > 160) // to the right
		{
			walkRight(&player1);
			walkRight(&player2);
		} else if (joyY > 160)// to the top
		{
			walkUp(&player1);
			walkUp(&player2);
		} else if (joyY < 100) // to the bottom
		{
			walkDown(&player1);
			walkDown(&player2);
		}

		checkExplosions(&player1);
		checkExplosions(&player2);

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

void drawPlayer(Bomberman *player) {
	lcd.fillRect(player->x * blockSize + 4, player->y * blockSize + 5, blockSize, blockSize, RGB(0, 0, 0));
}

void drawEmpty(Bomberman *player) {
	lcd.fillRect(player->x * blockSize + 4, player->y * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
}

void placeBomb(Bomberman *player) {
	if (levelGrid[player->y][player->x] != BOMB) {
		if (player->bombsPlaced < player->bombMaxAmount) {
			levelGrid[player->y][player->x] = BOMB;
			lcd.fillRect(player->x * blockSize + 4, player->y * blockSize + 5, blockSize, blockSize, RGB(200, 200, 0));
			
			struct Bomb *current = player->head;
			while (current->next != NULL) {
				current = current->next;
			}

			struct Bomb *temp = (struct Bomb*)malloc(sizeof(struct Bomb));
			temp->x = player->x;
			temp->y = player->y;
			temp->countdown = 100;
			temp->next = NULL;
			current->next = temp;

			player->bombsPlaced += 1;
		}
	}
}

void checkExplosions(Bomberman *player){

	struct Bomb *current = player->head->next;
	struct Bomb *prev = player->head;
	while (current != NULL) {

		current->countdown--;
		//Serial.println(current->countdown);
		
		if (current->countdown <= 0) {
			drawExplosion(current);
			levelGrid[current->y][current->x] = EMPTY;
			prev->next = current->next;
			
			free(current);
			current = prev->next;
			player->bombsPlaced--;
		}
		else {
			prev = current;
			current = current->next;
		}
	}
}

void drawExplosion(Bomb *b) {
	lcd.fillRect(b->x * blockSize + 4, b->y * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
	//lcd.fillRect(.... * blockSize + 4, .... * blockSize + 5, blockSize, blockSize, RGB(200, 200, 0));
}

void BombermanInit(){
	player1.x = 1;
	player1.y = 1;
	player1.bombRadius = 2;
	player1.bombMaxAmount = 10;
	player1.bombsPlaced = 0;
	struct Bomb *ptr = (struct Bomb*)malloc(sizeof(struct Bomb));
	player1.head = ptr;
	player1.head->next = NULL;


	player2.x = 9;
	player2.y = 9;
	player2.bombRadius = 2;
	player2.bombMaxAmount = 10;
	player2.bombsPlaced = 0;
	struct Bomb *ptr2 = (struct Bomb*)malloc(sizeof(struct Bomb));
	player2.head = ptr2;
	player2.head->next = NULL;
}


void walkLeft(Bomberman *player){
	if (levelGrid[player->y][player->x - 1] == EMPTY) {
		if (levelGrid[player->y][player->x] == BOMB) {
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player);
		}
		levelGrid[player->y][player->x - 1] = PLAYER;
		player->x -= 1;
		drawPlayer(player);
	}
}

void walkRight(Bomberman *player){
	if (levelGrid[player->y][player->x + 1] == EMPTY) {
		if (levelGrid[player->y][player->x] == BOMB) {
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player);
		}
		levelGrid[player->y][player->x + 1] = PLAYER;
		player->x += 1;
		drawPlayer(player);
	}
}

void walkDown(Bomberman *player){
	if (levelGrid[player->y + 1][player->x] == EMPTY) {
		if (levelGrid[player->y][player->x] == BOMB) {
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player);
		}
		levelGrid[player->y + 1][player->x] = PLAYER;
		player->y += 1;
		drawPlayer(player);
	}
}

void walkUp(Bomberman *player){
	if (levelGrid[player->y - 1][player->x] == EMPTY) {
		if (levelGrid[player->y][player->x] == BOMB) {
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player);
		}
		levelGrid[player->y - 1][player->x] = PLAYER;
		player->y -= 1;
		drawPlayer(player);
	}
}





void debugMap(){
	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 11; x++) {
			Serial.print(levelGrid[y][x]);
			Serial.print(" | ");
		}
		Serial.println("");
	}
}

void drawTime(){
	lcd.setTextColor(RGB(255,255,255), RGB(0,0,0));
	lcd.setCursor(245, 50);
	lcd.print("Time:");
	lcd.setCursor(285, 50);

	if(secondsTimer == 99){
		lcd.fillRect(285,50,30,10,RGB(0,0,0));
	}

	lcd.println(secondsTimer);
}

ISR(TIMER0_COMPA_vect){
	timer++;
	if(timer % 100 == 0){
		secondsTimer--;
		drawTime();
	}
}