#pragma once

#include <Wire.h>
//#include <SPI.h>
//#include <digitalWriteFast.h>
//#include <GraphicsLib.h>
//#include <SSD1331.h>
//#include <S65L2F50.h>
//#include <S65LPH88.h>
//#include <S65LS020.h>
//#include <MI0283QT2.h>
#include <MI0283QT9.h>;
//#include <DisplaySPI.h>
//#include <DisplayI2C.h>
#include "nunchuck_funcs.h"
//#include <avr/io.h>
//#include <stdlib.h>
//#include <stdint.h>
// lcd 240*360

#define blockSize 21
#define levelSize 11

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

object levelGrid[levelSize][levelSize] = {
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL},
	{WALL, PLAYER, EMPTY,  EMPTY,  BARREL, BARREL, EMPTY,  BARREL, BARREL, EMPTY,  WALL},
	{WALL, EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,   BARREL, WALL},
	{WALL, BARREL, BARREL, BARREL, EMPTY,  BARREL, BARREL, BARREL, BARREL, EMPTY,  WALL},
	{WALL, BARREL, WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL},
	{WALL, BARREL, EMPTY,  BARREL, BARREL, BARREL, EMPTY,  BARREL, BARREL, BARREL, WALL},
	{WALL, EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   BARREL, WALL},
	{WALL, BARREL, BARREL, BARREL, EMPTY,  BARREL, EMPTY,  EMPTY,  BARREL, BARREL, WALL},
	{WALL, EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL},
	{WALL, BARREL, BARREL, EMPTY,  BARREL, BARREL, BARREL, EMPTY,  EMPTY,  PLAYER, WALL},
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL}
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

void checkBombs(Bomberman *player);
void explodeBomb(int radius, int x, int y);
Bomb * findBombOnLocation(int x, int y);
void placeBomb(Bomberman *player);

bool createExplosion(int x, int y);
void checkExplosions();
void initExplosion(int x, int y);
void checkCollision(Bomberman *player);

void debugMap();


uint16_t timer;
uint8_t secondsTimer = 14;

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
	TCCR0B = (1 << CS02) | (1 << CS00); //1024 prescale
	OCR0A = 156; //approximately every 10 microseconds
	TIMSK0 |= (1 << OCIE0A); //enable timer compare match interupt



	BombermanInit(); //init player1 & player2

	struct ExplosionTile *ptr = (struct ExplosionTile*) malloc(sizeof (struct ExplosionTile));
	ExplosionHead = ptr;
	ExplosionHead->next = NULL;

	int pixelPosX = 4; //offset by 4 pixels
	int pixelPosY = 5; //offset by 5 pixels

	for (int y = 0; y < 11; y++) {
		//row
		for (int x = 0; x < 11; x++) {
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
		pixelPosY = 5;
		pixelPosX += blockSize;
	}
}

void gameLoop() {
	while (1) {
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
		}

		checkCollision(&player1);
		checkCollision(&player2);
		//debugMap();
	}
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

	player2.x = 9;
	player2.y = 9;
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
	if (x < 0 || x > levelSize - 1 || y < 0 || y > levelSize - 1) {
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
	lcd.fillRect(x * blockSize + 4, y * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
	lcd.fillEllipse(x * blockSize + 7 + 4, y * blockSize + 7 + 5, 7, 7, RGB(0, 0, 0));
	lcd.fillRect(x * blockSize + 7 + 4, y * blockSize + 5 + 5, 8, 5, RGB(0, 0, 0));
	lcd.drawLine(x * blockSize + 15 + 4, y * blockSize + 6 + 5, x * blockSize + 17 + 4, y * blockSize + 3 + 5, RGB(255, 0, 0));

}

void drawBarrel(int x, int y) {
	lcd.fillRect(x + 1, y + 1, 19, 19, RGB(102, 0, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(0, 0, 0));
	lcd.drawRect(x, y + 7, blockSize, 8, RGB(0, 0, 0));
}

void drawWall(int x, int y) {
	lcd.fillRect(x, y, blockSize, blockSize, RGB(153, 76, 0));
	lcd.drawRect(x, y, blockSize, blockSize, RGB(102, 51, 0));
}

void drawPlayer(Bomberman *player) {
	if (player->state == FALSE) {

		lcd.fillRect(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 1, 11, 16, RGB(255, 255, 0)); // body
		lcd.fillRect(player->x * blockSize + 4 + 9, player->y * blockSize + 5 + 4, 2, 2, RGB(0, 0, 0)); // eye
		lcd.fillRect(player->x * blockSize + 4 + 12, player->y * blockSize + 5 + 4, 2, 2, RGB(0, 0, 0)); // eye
		lcd.drawEllipse(player->x * blockSize + 4 + 11, player->y * blockSize + 5 + 8, 5, 2, RGB(0, 0, 0)); // smile
		lcd.fillRect(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 5, 11, 4, RGB(255, 255, 0)); // smile

		lcd.drawLine(player->x * blockSize + 4 + 7, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 7, player->y * blockSize + 5 + 20, RGB(0, 0, 0)); // left leg
		lcd.drawLine(player->x * blockSize + 4 + 15, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 15, player->y * blockSize + 5 + 20, RGB(0, 0, 0)); // right leg
		lcd.drawLine(player->x * blockSize + 4 + 5, player->y * blockSize + 5 + 9, player->x * blockSize + 4 + 5, player->y * blockSize + 5 + 14, RGB(0, 0, 0)); // left arm
		lcd.drawLine(player->x * blockSize + 4 + 17, player->y * blockSize + 5 + 9, player->x * blockSize + 4 + 17, player->y * blockSize + 5 + 14, RGB(0, 0, 0)); // right arm
		lcd.drawLine(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 16, player->y * blockSize + 5 + 17, RGB(0, 0, 0)); // belt
		} else {
		lcd.fillRect(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 1, 11, 16, RGB(0, 0, 255)); // body
		lcd.fillRect(player->x * blockSize + 4 + 9, player->y * blockSize + 5 + 4, 2, 2, RGB(0, 0, 0)); // eye
		lcd.fillRect(player->x * blockSize + 4 + 12, player->y * blockSize + 5 + 4, 2, 2, RGB(0, 0, 0)); // eye
		lcd.drawEllipse(player->x * blockSize + 4 + 11, player->y * blockSize + 5 + 8, 5, 2, RGB(0, 0, 0)); // smile
		lcd.fillRect(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 5, 11, 4, RGB(0, 0, 255)); // smile

		lcd.drawLine(player->x * blockSize + 4 + 7, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 7, player->y * blockSize + 5 + 20, RGB(0, 0, 0)); // left leg
		lcd.drawLine(player->x * blockSize + 4 + 15, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 15, player->y * blockSize + 5 + 20, RGB(0, 0, 0)); // right leg
		lcd.drawLine(player->x * blockSize + 4 + 5, player->y * blockSize + 5 + 9, player->x * blockSize + 4 + 5, player->y * blockSize + 5 + 14, RGB(0, 0, 0)); // left arm
		lcd.drawLine(player->x * blockSize + 4 + 17, player->y * blockSize + 5 + 9, player->x * blockSize + 4 + 17, player->y * blockSize + 5 + 14, RGB(0, 0, 0)); // right arm
		lcd.drawLine(player->x * blockSize + 4 + 6, player->y * blockSize + 5 + 17, player->x * blockSize + 4 + 16, player->y * blockSize + 5 + 17, RGB(0, 0, 0)); // belt
	}
}

void drawEmpty(int x, int y) {
	lcd.fillRect(x * blockSize + 4, y * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
}

void drawExplosion(int x, int y) {
	//lcd.fillRect(x * blockSize + 4, y * blockSize + 5, blockSize, blockSize, RGB(255, 0, 0));
	////
	lcd.fillEllipse(x * blockSize + 10 + 4, y * blockSize + 10 + 5, 10, 10, RGB(255, 0, 0));
	lcd.drawEllipse(x * blockSize + 10 + 4, y * blockSize + 10 + 5, 8, 8, RGB(255, 162, 0));
	lcd.drawEllipse(x * blockSize + 10 + 4, y * blockSize + 10 + 5, 5, 5, RGB(255, 162, 0));
	lcd.fillEllipse(x * blockSize + 11 + 4, y * blockSize + 10 + 5, 3, 3, RGB(255, 255, 255));
}

void drawTime() {
	lcd.setTextColor(RGB(255, 255, 255), RGB(0, 0, 0));
	lcd.setCursor(245, 50);
	lcd.print("Time:");
	lcd.setCursor(285, 50);

	if (secondsTimer == 99) {
		lcd.fillRect(285, 50, 30, 10, RGB(0, 0, 0));
	}

	if (secondsTimer == 9) {
		lcd.fillRect(285, 50, 30, 10, RGB(0, 0, 0));
	}
	lcd.println(secondsTimer);
}