
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

typedef enum {
    EMPTY, WALL, BARREL, PLAYER, BOMB
} object;

object levelGrid[11][11] = {
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
    {WALL, PLAYER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
    {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},
    {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
    {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},
    {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
    {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},
    {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
    {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},
    {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
    {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
};

int x = 4;
int y = 5;
int playLocX = 1;
int playLocY = 1;
int k = 0;
int l = 0;
int bombLocX = 0;
int bombLocY = 0;
int bombStrenght = 1;
int bombAmount = 3;
int bombsPlaced = 0;


MI0283QT9 lcd; //MI0283QT9 Adapter v1
byte zBut, cBut, joyX, joyY;

void setup() {
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization

    //init display
    lcd.begin();
    lcd.led(225);
    lcd.fillScreen(RGB(255, 255, 255));
    Serial.begin(9600);
    /////////////////////////////////////////////////////////////////////////////////////
    lcd.fillRect(240, 5, 76, 231, RGB(0, 0, 0));

    for (int k = 0; k < 11; k++) {
        //row
        for (int l = 0; l < 11; l++) {
            if (levelGrid[l][k] == WALL) {
                drawWall();
            } else if (levelGrid[l][k] == BARREL) {
                drawBarrel();
            } else if (levelGrid[l][k] == BOMB) {
                placeBomb();
            } else if (levelGrid[l][k] == PLAYER) {
                drawPlayer();
            }
            y += 21;
        }
        y = 5;
        x += 21;
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

void loop() {
    nunchuck_get_data();

    zBut = nunchuck_zbutton();
    joyX = nunchuck_joyx();
    joyY = nunchuck_joyy();
    if (joyX == 255 && joyY == 255) {
        Serial.println("hi");
    } else {
        Serial.print(joyX);
        Serial.print("  ");
        Serial.println(joyY);

        int o = 0;
        int p = 0;

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
            if (levelGrid[playLocY][playLocX] != BOMB) {
                placeBomb();
                bombLocX = playLocX;
                bombLocY = playLocY;
            }
        }

        if (joyX < 100)//to the left
        {
            walkLeft();
        } else if (joyX > 160) // to the right
        {
           walkRight(); 
        } else if (joyY > 160)// to the top
        {
            walkUp();
        } else if (joyY < 100) // to the bottom
        {
            walkDown();
        }

        //          Serial.print(playLocX);
        //          Serial.print(" , ");
        //          Serial.println(playLocY); 

        y = 5;
        x = 4;

        Serial.print(bombLocX);
        Serial.print("    ");
        Serial.println(bombLocY);
        Serial.println(bombsPlaced);
        Serial.println(bombAmount);
    }
}

void drawBarrel() {
    lcd.fillRect(x, y, 21, 21, RGB(255, 0, 0));
    lcd.drawRect(x, y, 21, 21, RGB(0, 0, 0));
}

void drawWall() {
    lcd.fillRect(x, y, 21, 21, RGB(153, 76, 0));
    lcd.drawRect(x, y, 21, 21, RGB(102, 51, 0));
}

void drawPlayer() {
    lcd.fillRect(playLocX * 21 + 4, playLocY * 21 + 5, 21, 21, RGB(0, 0, 0));
}

void drawEmpty() {
    lcd.fillRect(playLocX * 21 + 4, playLocY * 21 + 5, 21, 21, RGB(255, 255, 255));
}

void placeBomb() {
    if (bombsPlaced < bombAmount) {
        levelGrid[playLocY][playLocX] = BOMB;
        lcd.fillRect(playLocX * 21 + 4, playLocY * 21 + 5, 21, 21, RGB(200, 200, 0));
        bombsPlaced += 1;
    }

}

void drawExplosion() {
    lcd.fillRect(bombLocX * 21 + 4, bombLocY * 21 + 5, 21, 21, RGB(200, 200, 0));
}

void walkLeft(){
  if (levelGrid[playLocY][playLocX - 1] == WALL || levelGrid[playLocY][playLocX - 1] == BARREL || levelGrid[playLocY][playLocX - 1] == BOMB) {
            } else {
                if (levelGrid[playLocY][playLocX] == BOMB) {
                } else {
                    levelGrid[playLocY][playLocX] = EMPTY;
                    drawEmpty();
                }
                levelGrid[playLocY][playLocX - 1] = PLAYER;
                playLocX -= 1;
                drawPlayer();
            }
}

void walkRight(){
  if (levelGrid[playLocY][playLocX + 1] == WALL || levelGrid[playLocY][playLocX + 1] == BARREL || levelGrid[playLocY][playLocX + 1] == BOMB) {
            } else {
                if (levelGrid[playLocY][playLocX] == BOMB) {
                } else {
                    levelGrid[playLocY][playLocX] = EMPTY;
                    drawEmpty();
                }
                levelGrid[playLocY][playLocX + 1] = PLAYER;
                playLocX += 1;
                drawPlayer();
            }
}

void walkDown(){
  if (levelGrid[playLocY + 1][playLocX] == WALL || levelGrid[playLocY + 1][playLocX] == BARREL || levelGrid[playLocY + 1][playLocX] == BOMB) {
            } else {
                if (levelGrid[playLocY][playLocX] == BOMB) {
                } else {
                    levelGrid[playLocY][playLocX] = EMPTY;
                    drawEmpty();
                }
                levelGrid[playLocY + 1][playLocX] = PLAYER;
                playLocY += 1;
                drawPlayer();
            }
}

void walkUp(){
  if (levelGrid[playLocY - 1][playLocX] == WALL || levelGrid[playLocY - 1][playLocX] == BARREL || levelGrid[playLocY - 1][playLocX] == BOMB) {
            } else {
                if (levelGrid[playLocY][playLocX] == BOMB) {
                } else {
                    levelGrid[playLocY][playLocX] = EMPTY;
                    drawEmpty();
                }
                levelGrid[playLocY - 1][playLocX] = PLAYER;
                playLocY -= 1;
                drawPlayer();
            }
}

