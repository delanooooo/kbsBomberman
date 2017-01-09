#pragma once

#include <avr/io.h>
#include <MI0283QT9.h>

#define blockSize 16
#define levelSizeY 20
#define levelSizeX 15

extern char player1Name[];
extern char player2Name[];

extern int highScore1;
extern char nameHigh1[];
extern int highScore2;
extern char nameHigh2[];
extern int highScore3;
extern char nameHigh3[];

typedef enum {
	FALSE, TRUE
} bool;

typedef struct Bomberman {
	int x;
	int y;
	int bombRadius;
	int bombMaxAmount;
	int bombsPlaced;
	struct Bomb *head;
	bool state;
	int deaths;
	uint16_t invinsibleTimer;
	uint16_t movementTimer;
};

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
	Start, Instructions, Highscores
} menuItems;
extern menuItems currentItem;

typedef enum {
	EMPTY, WALL, BARREL, PLAYER, BOMB, EXPLOSION
} object;

extern object levelGrid[levelSizeX][levelSizeY];

extern MI0283QT9 lcd; //MI0283QT9 Adapter v1
extern byte zBut, cBut, joyX, joyY;

//prototyping
void showMenu();
void initMain();
void initGame();
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
void showHighScores();
void showInstr();
void setHighScore(int getScore, int i);
void getLetter(int w);

void debugMap();

extern uint16_t timer;
extern uint8_t secondsTimer;
