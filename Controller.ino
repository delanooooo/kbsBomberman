#include "Controller.h"

char player1Name[] = "player 1";
char player2Name[] = "player 2";

Bomberman player1, player2;

object levelGrid[levelSizeX][levelSizeY] = {
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,  WALL,   WALL, WALL,  WALL,  WALL,  WALL,  WALL},
	{WALL, EMPTY,  EMPTY,  EMPTY,  BARREL, BARREL, EMPTY,  BARREL, BARREL, EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,  EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, BARREL, BARREL, EMPTY,  BARREL, EMPTY,  BARREL, EMPTY,  BARREL, BARREL, EMPTY,  EMPTY,  EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,  BARREL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, BARREL, EMPTY,  BARREL, EMPTY,  BARREL, EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  BARREL, BARREL, EMPTY,  BARREL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,   BARREL, WALL,   EMPTY,  WALL,   EMPTY,  WALL,  BARREL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL, BARREL, EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,   BARREL, WALL,   BARREL, WALL,   EMPTY,  WALL,  EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, EMPTY,  EMPTY,  BARREL, EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  EMPTY,  BARREL, BARREL, EMPTY, EMPTY,  WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
	{WALL, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   WALL,  WALL,   WALL, WALL,  WALL,  WALL,  WALL,  WALL}
};

byte zBut, cBut, joyX, joyY;
MI0283QT9 lcd; //MI0283QT9 Adapter v1

uint16_t timer;
uint8_t secondsTimer;

menuItems currentItem = Instructions;


void initMain() {
	nunchuck_setpowerpins();
	nunchuck_init(); // send the initilization

	//init display
	lcd.begin();
	lcd.led(225);

	ir_setup();

	//Serial.begin(9600); // serial monitor
	showMenu();
}

void showMenu() {
	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setTextColor(RGB(255, 255, 255), RGB(0, 0, 0));
	lcd.setCursor(50, 40);
	lcd.setTextSize(3);
	lcd.print("Bomberman");
	lcd.setCursor(50, 70);
	lcd.setTextSize(2);
	lcd.print("Start Game");
	lcd.setCursor(50, 100);
	lcd.print("Instructions");
	lcd.setCursor(50, 130);
	lcd.print("Highscores");
	lcd.setTextSize(1);
	lcd.setCursor(60, 200);
	lcd.print("Press c to select");

	int i = 0;
	while (i == 0) {
		_delay_ms(150);
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		joyY = nunchuck_joyy();
		//Serial.println(cBut);
		if (cBut == 1) {
			i++;
		}
		int i = 1;
		if (joyY > 160) {//up
			if (currentItem == Start) {
				currentItem = Highscores;
				} else if (currentItem == Instructions) {
				currentItem = Start;
				} else if (currentItem == Highscores) {
				currentItem = Instructions;
			}
			
			lcd.fillRect(240, 65, 40, 110, RGB(0, 0, 0));
			lcd.fillCircle(260, 75 + (currentItem * 30), 10, RGB(0, 255, 0));
			} else if (joyY < 100) {//down
			if (currentItem == Start) {
				currentItem = Instructions;
				} else if (currentItem == Instructions) {
				currentItem = Highscores;
				} else if (currentItem == Highscores) {
				currentItem = Start;
			}
			lcd.fillRect(240, 65, 40, 110, RGB(0, 0, 0));
			lcd.fillCircle(260, 75 + (currentItem * 30), 10, RGB(0, 255, 0));
		}
	}
	if (currentItem == Start) {
		initGame();
		} else if (currentItem == Instructions) {
		showInstr();
		} else if (currentItem == Highscores) {
		showHighScores();
	}
}

void initGame() {
	lcd.fillScreen(RGB(255, 255, 255));
	lcd.fillRect(240, 5, 76, 231, RGB(0, 0, 0)); // HUD
	

	//init timer
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00); //1024 prescale
	OCR0A = 156; //approximately every 10 microseconds
	TIMSK0 |= (1 << OCIE0A); //enable timer compare match interupt

	secondsTimer = 15;

	
	BombermanInit(); //init player1 & player2
	drawScore(); // score on HUD
	struct ExplosionTile *ptr = (struct ExplosionTile*) malloc(sizeof (struct ExplosionTile));
	ExplosionHead = ptr;
	ExplosionHead->next = NULL;
	int pixelPosX = 0;
	int pixelPosY = 0;

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
	gameLoop();
}
uint8_t sendValue;
uint8_t receivedInstruction;
void gameLoop() {

	while (secondsTimer != 0) {
		
		nunchuck_get_data();

		zBut = nunchuck_zbutton();
		joyX = nunchuck_joyx();
		joyY = nunchuck_joyy();


		sendValue = 0;
		_delay_ms(10);
		//Serial.print(joyX);
		//Serial.print("  ");
		//Serial.println(joyY);

		if (joyX == 255 && joyY == 255) {
			continue;
		}
		//player1 movement
		if (timer >= player1.movementTimer + 20) {
			if (zBut == 1) {
				placeBomb(&player1);
				sendValue |= (1 << 7);
			}
			if (joyX < 100)//to the left
			{
				walkLeft(&player1);
				sendValue |= (1 << 3);
			} else if (joyX > 160) // to the right
			{
				walkRight(&player1);
				sendValue |= (1 << 1);
			} else if (joyY > 160)// to the top
			{
				walkUp(&player1);
				sendValue |= (1 << 0);
			} else if (joyY < 100) // to the bottom
			{
				walkDown(&player1);
				sendValue |= (1 << 2);
			}
			if(sendValue){
			sendData(sendValue);
			}
			player1.movementTimer = timer;
		}
		
		receivedInstruction = 0;
		//player2 movement
		readValue();
		if(!startCollecting){
		receivedInstruction = receivedData;
		}
		if (timer >= player1.movementTimer + 20) {
			if (receivedInstruction &= (1 << 7)) {
				placeBomb(&player2);
			}
			if (receivedInstruction &= (1 << 3))//to the left
			{
				walkLeft(&player2);
			} else if (receivedInstruction &= (1 << 1)) // to the right
			{
				walkRight(&player2);
			} else if (receivedInstruction &= (1 << 0))// to the top
			{
				walkUp(&player2);
			} else if (receivedInstruction &= (1 << 2)) // to the bottom
			{
				walkDown(&player2);
			}
			player2.movementTimer = timer;
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
	player1.deaths = 0;
	
	player2.x = 13;
	player2.y = 13;
	player2.bombRadius = 4;
	player2.bombMaxAmount = 3;
	player2.bombsPlaced = 0;
	player2.state = FALSE;
	struct Bomb *ptr2 = (struct Bomb*) malloc(sizeof (struct Bomb));
	player2.head = ptr2;
	player2.head->next = NULL;
	player2.deaths = 0;
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
	for (int y = 0; y < levelSizeY; y++) {
		for (int x = 0; x < levelSizeX; x++) {
			//Serial.print(levelGrid[y][x]);
			//Serial.print(" | ");
		}
		//Serial.println("");
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
		lcd.fillRect(player->x * blockSize + 4, player->y * blockSize + 3, 8, 8, RGB(255, 255, 0)); // body
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 2, 8, 1, RGB(0, 0, 0)); // hat
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 1, 6, 1, RGB(0, 0, 0)); // hat
		lcd.fillRect(player->x * blockSize + 5, player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.fillRect(player->x * blockSize + 9, player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 6, 6, 1, RGB(0, 0, 0)); // smile
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 11, 8, 1, RGB(0, 0, 0)); // belt
		lcd.drawRect(player->x * blockSize + 3, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // left arm
		lcd.drawRect(player->x * blockSize + 12, player->y * blockSize + 7, 1, 3, RGB(0, 0, 0)); // right arm
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // left leg
		lcd.drawRect(player->x * blockSize + 10, player->y * blockSize + 12, 1, 3, RGB(0, 0, 0)); // right leg
		} else {
		lcd.fillRect(player->x * blockSize + 4, player->y * blockSize + 3, 8, 8, RGB(0, 0, 255)); // body
		lcd.drawRect(player->x * blockSize + 4, player->y * blockSize + 2, 8, 1, RGB(0, 0, 0)); // hat
		lcd.drawRect(player->x * blockSize + 5, player->y * blockSize + 1, 6, 1, RGB(0, 0, 0)); // hat
		lcd.fillRect(player->x * blockSize + 5, player->y * blockSize + 4, 2, 1, RGB(0, 0, 0)); // eye
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
	lcd.fillRect(x * blockSize, y * blockSize, blockSize, blockSize, RGB(255, 255, 255));
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

void drawScore() {
	int score1 = player2.deaths;
	int score2 = player1.deaths;

	lcd.setTextColor(RGB(255, 255, 255), RGB(0, 0, 0));
	lcd.setCursor(250, 55);
	lcd.println("Score");
	lcd.setCursor(240, 65);
	lcd.println(player1Name);
	lcd.setCursor(265, 77);
	lcd.print(score1);

	lcd.setCursor(250, 115);
	lcd.println("Score");
	lcd.setCursor(240, 125);
	lcd.println(player2Name);
	lcd.setCursor(265, 137);
	lcd.print(score2);
}

void gameOver() {
	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setCursor(80, 70);
	lcd.setTextSize(5);
	lcd.print("GAME");
	lcd.setCursor(80, 120);
	lcd.print("OVER");
	lcd.setTextSize(1);

	_delay_ms(150);
	int score1 = player2.deaths;
	int score2 = player1.deaths;

	highScores(score1, score2);

	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setCursor(50, 35);
	lcd.setTextSize(2);
	lcd.print("Score ");
	lcd.print(player1Name);
	lcd.setCursor(150, 70);
	lcd.print(score1);

	lcd.setCursor(50, 120);
	lcd.print("Score ");
	lcd.print(player2Name);
	lcd.setCursor(150, 155);
	lcd.print(score2);
	lcd.setTextSize(1);
	lcd.setCursor(60, 200);
	lcd.print("Press c to go to the menu");

	int i = 0;
	while (i == 0) {
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		_delay_ms(150);
		if (cBut == 1) {
			i++;
		}
	}
	showMenu();
}
int highScore1 = -1;
char nameHigh1[] = "    ";
int highScore2 = -1;
char nameHigh2[] = "    ";
int highScore3 = -1;
char nameHigh3[] = "    ";

void showHighScores() {
	
	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setCursor(30, 40);
	lcd.setTextSize(3);
	lcd.print("High Scores");
	lcd.setTextSize(2);
	lcd.setCursor(70, 80);
	lcd.print("1.");
	lcd.println(nameHigh1);
	lcd.setCursor(70, 120);
	lcd.print("2.");
	lcd.println(nameHigh2);
	lcd.setCursor(70, 160);
	lcd.print("3.");
	lcd.println(nameHigh3);
	lcd.setCursor(210, 80);
	lcd.println(highScore1);
	lcd.setCursor(210, 120);
	lcd.println(highScore2);
	lcd.setCursor(210, 160);
	lcd.println(highScore3);

	lcd.setCursor(60, 200);
	lcd.setTextSize(1);
	lcd.print("Press c to go to the menu");

	int k = 0;
	while (k == 0) {
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		_delay_ms(150);
		if (cBut == 1) {
			k++;
			showMenu();
		}
	}
}
void highScores(int pl1Score, int pl2Score) {
	if (pl1Score > highScore3) {
		lcd.fillScreen(RGB(0, 0, 0));
		lcd.setCursor(100, 60);
		lcd.setTextSize(2);
		lcd.print(player1Name);
		lcd.setCursor(35, 100);
		lcd.print("BROKE THE RECORD");
		_delay_ms(150);
		getLetter(pl1Score);

		} else if (pl2Score > highScore3) {
		lcd.fillScreen(RGB(0, 0, 0));
		lcd.setCursor(100, 60);
		lcd.setTextSize(2);
		lcd.print(player2Name);
		lcd.setCursor(35, 100);
		lcd.print("BROKE THE RECORD");
		_delay_ms(2000);
		getLetter(pl2Score);
	}
}

void getLetter(int highScore) {
	int score1 = player2.deaths;
	int score2 = player1.deaths;
	nunchuck_get_data();
	cBut = nunchuck_cbutton();
	joyY = nunchuck_joyy();
	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setCursor(45, 60);
	lcd.print("Choose a letter");
	lcd.setCursor(80, 200);
	lcd.setTextSize(1);
	lcd.print("Press c to continue");

	int i = 0;
	int done = 0;
	while (done == 0) {
		_delay_ms(100);
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		joyY = nunchuck_joyy();
		_delay_ms(150);
		if (joyY > 160)// to the top
		{
			if (i > 24) {
				i = 0;
				} else {
				i++;
			}
		} else if (joyY < 100) // to the bottom
		{
			if (i < 1) {
				i = 25;
				} else {
				i--;
			}
		}
		lcd.setCursor(150, 100);
		lcd.setTextSize(4);
		lcd.println((char) (i + 'A')); // take the i-st letter
		if (cBut == 1) {
			if (highScore == score1) {
				*player1Name = i + 'A';
				player1Name[1] = 0;
				} else if (highScore == score2) {
				*player2Name = i + 'A';
				player2Name[1] = 0;
			}
			setHighScore(highScore, i);
			done++;
		}
	}
}

void setHighScore(int getScore, int i) {
	if (getScore > highScore1) {
		*nameHigh3 = *nameHigh2;
		*nameHigh2 = *nameHigh1;
		*nameHigh1 = i + 'A';
		nameHigh1[1] = 0;
		highScore3 = highScore2;
		highScore2 = highScore1;
		highScore1 = getScore;
		} else if (getScore > highScore2 && (getScore < highScore1 || getScore == highScore1)){
		*nameHigh3 = *nameHigh2;
		*nameHigh2 = i + 'A';
		nameHigh2[1] = 0;
		highScore3 = highScore2;
		highScore2 = getScore;
		} else if (getScore < highScore2 && (getScore > highScore3 || getScore == highScore3)){
		*nameHigh3 = i + 'A';
		nameHigh3[1] = 0;
		highScore3 = getScore;
	}
}

void showInstr(){
	lcd.fillScreen(RGB(0, 0, 0));
	lcd.setCursor(15, 40);
	lcd.setTextSize(3);
	lcd.print("Instructions");
	lcd.setTextSize(2);
	
	lcd.setCursor(15,90);
	lcd.println("move the bomberman");
	lcd.setCursor(15,110);
	lcd.print("with the joystick");
	lcd.setCursor(15, 150);
	lcd.println("place a bomb with");
	lcd.setCursor(15, 170);
	lcd.println("the z button");
	
	lcd.setCursor(60, 200);
	lcd.setTextSize(1);
	lcd.print("Press c to go to the menu");
	
	int k = 0;
	while (k == 0) {
		nunchuck_get_data();
		cBut = nunchuck_cbutton();
		_delay_ms(150);
		if (cBut == 1) {
			k++;
			showMenu();
		}
	}
}