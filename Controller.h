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
#include <MI0283QT9.h>;
#include <DisplaySPI.h>
#include <DisplayI2C.h>
#include "nunchuck_funcs.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
// lcd 240*360

#define blockSize 16
#define levelSizeY 20
#define levelSizeX 15

struct Bomb {
	int x;
	int y;
	uint16_t detonateTime;
	struct Bomb *next;
};

struct ExplosionTile {
	int x;
	int y;
	uint16_t removeTime;
	struct ExplosionTile *next;
};

struct ExplosionTile *ExplosionHead;

typedef enum {
	FALSE, TRUE
} bool;

struct Bomberman {
	int x;
	int y;
	int bombRadius;
	int bombMaxAmount;
	int bombsPlaced;
	struct Bomb *head;
	bool state;
	int deaths = 0;
	uint16_t invinsibleTimer = 0;
	uint16_t movementTimer = 0;
} player1, player2;

typedef enum {
	EMPTY, WALL, BARREL, PLAYER, BOMB, EXPLOSION
} object;

object levelGrid[levelSizeX][levelSizeY] = {
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,  WALL,  WALL, WALL, WALL, WALL, WALL, WALL},
	{WALL, EMPTY,  EMPTY,  EMPTY,  BARREL, BARREL,  EMPTY,  BARREL,  BARREL, EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,  EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, BARREL, BARREL, EMPTY,  BARREL,  EMPTY,  BARREL,  EMPTY,  BARREL,  BARREL, EMPTY,  EMPTY,  EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  WALL,   BARREL,  WALL,   BARREL,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,  BARREL, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, BARREL, EMPTY,  BARREL, EMPTY,  BARREL, EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  WALL,   EMPTY, WALL,   EMPTY,  WALL,   BARREL,  WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  BARREL, BARREL, EMPTY,  BARREL, EMPTY,  EMPTY, BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,  BARREL,WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL, BARREL, EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, BARREL,  WALL,   BARREL, WALL,   EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL,  BARREL, EMPTY, EMPTY, WALL, EMPTY,EMPTY,EMPTY,EMPTY,WALL},
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,  WALL,  WALL, WALL, WALL, WALL, WALL, WALL}
};

MI0283QT9 lcd; //MI0283QT9 Adapter v1
byte zBut, cBut, joyX, joyY;

//prototyping
void gameLoop();
void BombermanInit();
void walkDown(Bomberman *player);
void walkLeft(Bomberman *player);
void walkRight(Bomberman *player);
void walkUp(Bomberman *player);

void drawBarrel(int x, int y);
void drawEmpty(int x, int y);
void drawPlayer(Bomberman *player);
void drawWall(int x, int y);
void drawTime();
void drawExplosion(int x, int y);
void drawBomb(int x, int y);
void drawScore();

void checkBombs(Bomberman *player);
void explodeBomb(int radius, int x, int y);
Bomb * findBombOnLocation(int x, int y);
void placeBomb(Bomberman *player);

bool createExplosion(int x, int y);
void checkExplosions();
void initExplosion(int x, int y);
void checkCollision(Bomberman *player);

void gameOver();
void highScores(int pl1, int pl2);
void getLetter();
void debugMap();


uint16_t timer;
uint8_t secondsTimer = 3;


void initController() {

	nunchuck_setpowerpins();
	nunchuck_init(); // send the initilization

	//init display
	lcd.begin();
	lcd.led(225);
	lcd.fillScreen(RGB(255, 255, 255));

	Serial.begin(9600); // serial monitor
	
	lcd.fillRect(240, 5, 76, 231, RGB(0, 0, 0)); // HUD
	drawScore(); // score on HUD

	//init timer
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00); //1024 prescale
	OCR0A = 156; //approximately every 10 microseconds
	TIMSK0 |= (1 << OCIE0A); //enable timer compare match interupt



	BombermanInit(); //init player1 & player2

	struct ExplosionTile *ptr = (struct ExplosionTile*) malloc(sizeof (struct ExplosionTile));
	ExplosionHead = ptr;
	ExplosionHead->next = NULL;

	int pixelPosX = 0; //offset by 4 pixels
	int pixelPosY = 0; //offset by 5 pixels	 
	for (int y = 0; y < levelSizeY; y++) {
		//row
		for (int x = 0; x < levelSizeX; x++) {
			if (levelGrid[x][y] == WALL) {
				drawWall(pixelPosX, pixelPosY);
				} else if (levelGrid[x][y] == BARREL) {
				drawBarrel(pixelPosX, pixelPosY);
				} else if (levelGrid[x][y] == BOMB) {
				placeBomb(&player1);
				} else if (x == player1.x && y == player1.y) {
				drawPlayer(&player1);
				} else if (x == player2.x && y == player2.y) {
				drawPlayer(&player2);
			}
			pixelPosY += blockSize;
		}
		pixelPosY = 0;
		pixelPosX += blockSize;
	}
}

void gameLoop() {
	
		while (secondsTimer != 0) {
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

			if (timer >= player1.movementTimer + 20) {
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
				player1.movementTimer = timer;
			}

			checkBombs(&player1);
			checkBombs(&player2);
			checkExplosions();

			if (timer % 100 == 0) {
				secondsTimer--;
				drawTime();

				checkCollision(&player1);
				checkCollision(&player2);
			}
		

		
}
//debugMap();
		gameOver();

}

void BombermanInit() {
	player1.x = 1;
	player1.y = 1;
	player1.bombRadius = 4;
	player1.bombMaxAmount = 3;
	player1.bombsPlaced = 0;
	player1.state = FALSE;
	struct Bomb *ptr = (struct Bomb*) malloc(sizeof (struct Bomb));
	player1.head = ptr;
	player1.head->next = NULL;

	player2.x = 13;
	player2.y = 13;
	player2.bombRadius = 4;
	player2.bombMaxAmount = 3;
	player2.bombsPlaced = 0;
	player2.state = FALSE;
	struct Bomb *ptr2 = (struct Bomb*) malloc(sizeof (struct Bomb));
	player2.head = ptr2;
	player2.head->next = NULL;
}

void placeBomb(Bomberman *player) {
	if (levelGrid[player->y][player->x] != BOMB) {
		if (player->bombsPlaced < player->bombMaxAmount) {
			levelGrid[player->y][player->x] = BOMB;
			/////////////////////////////!!!
			drawBomb(player->x, player->y);

			struct Bomb *current = player->head;
			while (current->next != NULL) {
				current = current->next;
			}

			struct Bomb *temp = (struct Bomb*) malloc(sizeof (struct Bomb));
			temp->x = player->x;
			temp->y = player->y;
			temp->detonateTime = timer + 200; //2 seconden
			temp->next = NULL;
			current->next = temp;

			player->bombsPlaced += 1;
		}
	}
}

void checkBombs(Bomberman *player) {

	struct Bomb *current = player->head->next;
	struct Bomb *prev = player->head;
	while (current != NULL) {

		if (timer >= current->detonateTime) {
			levelGrid[current->y][current->x] = EMPTY;
			drawEmpty(current->x, current->y);
			explodeBomb(player->bombRadius, current->x, current->y);
			prev->next = current->next;

			free(current);
			current = prev->next;
			player->bombsPlaced--;
			} else {
			prev = current;
			current = current->next;
		}
	}
}

void explodeBomb(int radius, int x, int y) {

	createExplosion(x, y);
	for (int i = 1; i < radius; i++) {
		if (createExplosion(x, y + i) == FALSE) {
			break;
		}
	}
	for (int i = 1; i < radius; i++) {
		if (createExplosion(x, y - i) == FALSE) {
			break;
		}
	}
	for (int i = 1; i < radius; i++) {
		if (createExplosion(x + i, y) == FALSE) {
			break;
		}
	}
	for (int i = 1; i < radius; i++) {
		if (createExplosion(x - i, y) == FALSE) {
			break;
		}
	}
}

Bomb * findBombOnLocation(int x, int y) {
	struct Bomb *current = player1.head->next;
	while (current != NULL) {

		if (current->x == x && current->y == y) {
			return current;
			} else {
			current = current->next;
		}
	}
	current = player2.head->next;
	while (current != NULL) {

		if (current->x == x && current->y == y) {
			return current;
			} else {
			current = current->next;
		}
	}
	return NULL;
}

bool createExplosion(int x, int y) {
	if (x < 0 || x > levelSizeX - 1 || y < 0 || y > levelSizeY - 1) {
		return false;
	}

	if (levelGrid[y][x] == EMPTY) {
		initExplosion(x, y);
		} else if (levelGrid[y][x] == PLAYER) {
		initExplosion(x, y);
		checkCollision(&player1);
		checkCollision(&player2);
		} else if (levelGrid[y][x] == WALL) {
		return false;
		} else if (levelGrid[y][x] == BARREL) {
		initExplosion(x, y);
		return false;
		} else if (levelGrid[y][x] == BOMB) {
		findBombOnLocation(x, y)->detonateTime = 0; //explode the bomb hit by the explosion
		return false;
	}
	return true;
}

void initExplosion(int x, int y) {
	levelGrid[y][x] = EXPLOSION;
	drawExplosion(x, y);
	struct ExplosionTile *current = ExplosionHead;
	while (current->next != NULL) {
		current = current->next;
	}

	struct ExplosionTile *temp = (struct ExplosionTile*) malloc(sizeof (struct ExplosionTile));
	temp->x = x;
	temp->y = y;
	//Serial.print("x: ");
	//Serial.println(x);
	//Serial.print("y: ");
	//Serial.println(y);
	temp->removeTime = timer + 100; //1 seconde
	temp->next = NULL;
	current->next = temp;
}

void checkExplosions() {
	struct ExplosionTile *current = ExplosionHead->next;
	struct ExplosionTile *prev = ExplosionHead;
	while (current != NULL) {

		if (timer >= current->removeTime) {
			levelGrid[current->y][current->x] = EMPTY;
			drawEmpty(current->x, current->y);
			prev->next = current->next;

			free(current);
			current = prev->next;
			drawPlayer(&player1);
			drawPlayer(&player2);
			} else {
			prev = current;
			current = current->next;
		}
	}
}

void walkLeft(Bomberman *player) {
	if (levelGrid[player->y][player->x - 1] == EMPTY || levelGrid[player->y][player->x - 1] == EXPLOSION) {
		if (levelGrid[player->y][player->x] == BOMB) {
			drawBomb(player->x, player->y);
			} else if (levelGrid[player->y][player->x] == EXPLOSION) {
			drawExplosion(player->x, player->y);
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player->x, player->y);
		}
		if (levelGrid[player->y][player->x - 1] != EXPLOSION) {
			levelGrid[player->y][player->x - 1] = PLAYER;
			player->x -= 1;
			drawPlayer(player);
			} else {
			levelGrid[player->y][player->x - 1] = EXPLOSION;
			player->x -= 1;
		}
	}
}

void walkRight(Bomberman *player) {
	if (levelGrid[player->y][player->x + 1] == EMPTY || levelGrid[player->y][player->x + 1] == EXPLOSION) {
		if (levelGrid[player->y][player->x] == BOMB) {
			drawBomb(player->x, player->y);
			} else if (levelGrid[player->y][player->x] == EXPLOSION) {
			drawExplosion(player->x, player->y);
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player->x, player->y);
		}
		if (levelGrid[player->y][player->x + 1] != EXPLOSION) {
			levelGrid[player->y][player->x + 1] = PLAYER;
			player->x += 1;
			drawPlayer(player);
			} else {
			levelGrid[player->y][player->x + 1] = EXPLOSION;
			player->x += 1;
		}
	}
}

void walkDown(Bomberman *player) {
	if (levelGrid[player->y + 1][player->x] == EMPTY || levelGrid[player->y + 1][player->x] == EXPLOSION) {
		if (levelGrid[player->y][player->x] == BOMB) {
			drawBomb(player->x, player->y);
			} else if (levelGrid[player->y][player->x] == EXPLOSION) {
			drawExplosion(player->x, player->y);
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player->x, player->y);
		}
		if (levelGrid[player->y + 1][player->x] != EXPLOSION) {
			levelGrid[player->y + 1][player->x] = PLAYER;
			player->y += 1;
			drawPlayer(player);
			} else {
			levelGrid[player->y + 1][player->x] = EXPLOSION;
			player->y += 1;
		}
	}
}

void walkUp(Bomberman *player) {
	if (levelGrid[player->y - 1][player->x] == EMPTY || levelGrid[player->y - 1][player->x] == EXPLOSION) {
		if (levelGrid[player->y][player->x] == BOMB) {
			drawBomb(player->x, player->y);
			} else if (levelGrid[player->y][player->x] == EXPLOSION) {
			drawExplosion(player->x, player->y);
			} else {
			levelGrid[player->y][player->x] = EMPTY;
			drawEmpty(player->x, player->y);
		}
		if (levelGrid[player->y - 1][player->x] != EXPLOSION) {
			levelGrid[player->y - 1][player->x] = PLAYER;
			player->y -= 1;
			drawPlayer(player);
			} else {
			levelGrid[player->y - 1][player->x] = EXPLOSION;
			player->y -= 1;
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	timer++;
}

void checkCollision(Bomberman *player) {
	if (levelGrid[player->y][player->x] == EXPLOSION) {
		if (player->state == FALSE) {
			player->deaths++;
			drawScore();
			player->state = TRUE;
			player->invinsibleTimer = timer;
		}
	}

	if (timer >= player->invinsibleTimer + 300 && player->state == TRUE) {
		player->state = FALSE;
		drawPlayer(player);
	}
}

void debugMap() {
	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 11; x++) {
			Serial.print(levelGrid[y][x]);
			Serial.print(" | ");
		}
		Serial.println("");
	}
}

void drawBomb(int x, int y) {
	lcd.fillRect(x * blockSize, y * blockSize, blockSize, blockSize, RGB(255, 255, 255));
	lcd.fillEllipse(x * blockSize + 7, y * blockSize + 7, 7, 7, RGB(0, 0, 0));
	lcd.fillRect(x * blockSize + 7, y * blockSize + 5, 8, 5, RGB(0, 0, 0));
	lcd.drawLine(x * blockSize + 15, y * blockSize + 6, x * blockSize + 17, y * blockSize + 3, RGB(255, 0, 0));

}

void drawBarrel(int x, int y) {
	lcd.fillRect(x + 1, y + 1, blockSize - 2, blockSize - 2, RGB(102, 0, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(0, 0, 0));
	lcd.drawRect(x, y, blockSize, 8, RGB(0, 0, 0));
}

void drawWall(int x, int y) {
	lcd.fillRect(x, y, blockSize, blockSize, RGB(153, 76, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(102, 51, 0));
}

void drawPlayer(Bomberman *player) {
	if (player->state == FALSE) {
		lcd.fillRect(player->x * blockSize + 4 , player->y * blockSize + 3, 8, 8, RGB(255, 255, 0)); // body
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 2, 8, 1, RGB(0, 0, 0)); // hat
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 1, 6, 1, RGB(0, 0, 0)); // hat
		lcd.fillRect(player->x * blockSize + 5 , player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.fillRect(player->x * blockSize + 9, player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 6, 6, 1, RGB(0, 0, 0)); // smile
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 11, 8, 1, RGB(0, 0, 0)); // belt
		lcd.drawRect(player->x * blockSize + 3, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // left arm
		lcd.drawRect(player->x * blockSize + 12, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // right arm
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // left leg
		lcd.drawRect(player->x * blockSize + 10, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // right leg
		} else {
		lcd.fillRect(player->x * blockSize + 4 , player->y * blockSize + 3, 8, 8, RGB(0, 0, 255)); // body
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 2, 8, 1, RGB(0, 0, 0)); // hat
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 1, 6, 1, RGB(0, 0, 0)); // hat
		lcd.fillRect(player->x * blockSize + 5 , player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.fillRect(player->x * blockSize + 9, player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 6, 6, 1, RGB(0, 0, 0)); // smile
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 11, 8, 1, RGB(0, 0, 0)); // belt
		lcd.drawRect(player->x * blockSize + 3, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // left arm
		lcd.drawRect(player->x * blockSize + 12, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // right arm
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // left leg
		lcd.drawRect(player->x * blockSize + 10, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // right leg
	}
}

void drawEmpty(int x, int y) {
	lcd.fillRect(x * blockSize, y * blockSize , blockSize, blockSize, RGB(255, 255, 255));
}

void drawExplosion(int x, int y) {
	lcd.fillRect(x * blockSize, y * blockSize, blockSize, blockSize, RGB(255, 0, 0));
	//lcd.fillEllipse(x * blockSize + 8, y * blockSize + 8, 8, 8, RGB(255, 0, 0));
	lcd.drawEllipse(x * blockSize + 8, y * blockSize + 8, 6, 6, RGB(255, 162, 0));
	lcd.drawEllipse(x * blockSize + 8, y * blockSize + 8, 3, 3, RGB(255, 162, 0));
	lcd.fillEllipse(x * blockSize + 9, y * blockSize + 8, 1, 1, RGB(255, 255, 255));
}

void drawTime() {
	lcd.setTextColor(RGB(255, 255, 255), RGB(0, 0, 0));
	lcd.setCursor(250, 25);
	lcd.println("Time:");
	lcd.setCursor(255, 35);

	if (secondsTimer == 99) {
		lcd.fillRect(271, 35, 10, 10, RGB(0, 0, 0));
	}

	if (secondsTimer == 9) {
		lcd.fillRect(263, 35, 10, 10, RGB(0, 0, 0));
	}
	

	lcd.println(secondsTimer);
}

void drawScore(){
	int score1 = player2.deaths;  
	int score2 = player1.deaths;

		lcd.setTextColor(RGB(255, 255, 255), RGB(0, 0, 0));
		lcd.setCursor(250, 55);
		lcd.println("Score");
		lcd.setCursor(240, 65);
		lcd.println("speler 1");
		lcd.setCursor(265, 77);
		lcd.print(score1);

		lcd.setCursor(250, 115);
		lcd.println("Score");
		lcd.setCursor(240, 125);
		lcd.println("speler 2");
		lcd.setCursor(265, 137);
		lcd.print(score2);
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gameOver(){
		lcd.fillScreen(RGB(0,0,0));
		lcd.setCursor(80, 70);
		lcd.setTextSize(5);
		lcd.print("GAME");
		lcd.setCursor(80, 120);
		lcd.print("OVER");
		lcd.setTextSize(1);

		_delay_ms(3000);
		int score1 = player2.deaths;
		int score2 = player1.deaths;

		highScores(score1, score2);
		
		lcd.fillScreen(RGB(0,0,0));
		lcd.setCursor(50, 35);
		lcd.setTextSize(2);
		lcd.print("Score player 1");
		lcd.setCursor(150, 70);
		lcd.print(score1);
		lcd.setCursor(50, 120);
		lcd.print("Score player 2");
		lcd.setCursor(150, 155);
		lcd.print(score2);
		lcd.setTextSize(1);
		lcd.setCursor(60,200);
		lcd.print("Press c to go to the menu");	

		int i = 0;
		while(i == 0){
				nunchuck_get_data();
				cBut = nunchuck_cbutton(); 
				Serial.println(cBut);
				if(cBut == 1){		
					i++;
				}
		  }
		lcd.setCursor(10,10);		
		lcd.println("ShowMenu(); of zoiets");
	}

int highScore = -1;
void highScores(int pl1, int pl2){	
	if(pl1 > highScore){
		lcd.fillScreen(RGB(0,0,0));
		lcd.setCursor(100,60);
		lcd.setTextSize(2);
		lcd.print("PLAYER 1");
		lcd.setCursor(35,100);
		lcd.print("BROKE THE RECORD");
		_delay_ms(3000);
		getLetter();
		//print letter met score;
		highScore = pl1;
		
	}else if(pl2 > highScore){
		lcd.fillScreen(RGB(0,0,0));
		lcd.setCursor(100,60);
		lcd.setTextSize(2);
		lcd.print("PLAYER 2");
		lcd.setCursor(35,100);
		lcd.print("BROKE THE RECORD");
		_delay_ms(3000);
		getLetter();
		//print letter met score;
		highScore = pl2;
	}
} 

void getLetter(){
	nunchuck_get_data();
	cBut = nunchuck_cbutton();
	joyY = nunchuck_joyy();
	
	char letter[26];
	strcpy(letter, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	lcd.fillScreen(RGB(0,0,0));
	lcd.setCursor(45, 60);
	lcd.print("Choose a letter");

	int i = 0;
	int done = 0;
	while(done == 0){
		_delay_ms(150);
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		joyY = nunchuck_joyy();
		if (joyY > 160)// to the top
		{
			if (i > 26){
				i = 0;
			}
			i++;
		} else if (joyY < 100) // to the bottom
		{
			if (i < 0){
				i = 26;
			}
			i--;
		}
		lcd.setCursor(150,100);
		lcd.setTextSize(4);
		lcd.println(letter[i]);
		
		if(cBut == 1){
			done++;
			char winner = letter[i];
		}
	}	

}