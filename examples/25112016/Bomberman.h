#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include "Controller.h"

class Bomberman{
	public:
		int x;
		int y;
		int bombRadius;
		int bombMaxAmount;
		Bomberman();
		void walkDown();
		void walkLeft();
		void walkRight();
		void walkUp();
		void draw();
};

Bomberman::Bomberman(){
	this->x = 1;
	this->y = 1;
	this->bombRadius = 2;
	this->bombMaxAmount = 3;
}


void Bomberman::walkLeft(){
	if (levelGrid[y][x - 1] == EMPTY) {
		if (levelGrid[y][x] == BOMB) {
			} else {
			levelGrid[y][x] = EMPTY;
			drawEmpty();
		}
		levelGrid[y][x - 1] = PLAYER;
		x -= 1;
		drawPlayer();
	}
}

void Bomberman::walkRight(){
	if (levelGrid[y][x + 1] == EMPTY) {
		if (levelGrid[y][x] == BOMB) {
			} else {
			levelGrid[y][x] = EMPTY;
			drawEmpty();
		}
		levelGrid[y][x + 1] = PLAYER;
		x += 1;
		drawPlayer();
	}
}

void Bomberman::walkDown(){
	if (levelGrid[y + 1][x] == EMPTY) {
		if (levelGrid[y][x] == BOMB) {
			} else {
			levelGrid[y][x] = EMPTY;
			drawEmpty();
		}
		levelGrid[y + 1][x] = PLAYER;
		y += 1;
		drawPlayer();
	}
}

void Bomberman::walkUp(){
	if (levelGrid[y - 1][x] == EMPTY) {
		if (levelGrid[y][x] == BOMB) {
			} else {
			levelGrid[y][x] = EMPTY;
			drawEmpty();
		}
		levelGrid[y - 1][x] = PLAYER;
		y -= 1;
		drawPlayer();
	}
}

#endif /* BOMBERMAN_H_ */