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
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
// lcd 240*360
typedef enum { EMPTY, WALL, BARREL, PLAYER}object;

object levelGrid[11][11] = { 
                   {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}, 
                   {WALL, PLAYER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},  
                   {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL}, 
                   {WALL, BARREL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL}, 
                   {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},  
                   {WALL, BARREL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL}, 
                   {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},   
                   {WALL, BARREL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                   {WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL, EMPTY, WALL},   
                   {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                   {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL }};

MI0283QT9 lcd;  //MI0283QT9 Adapter v1
void setup() {
  //init display
  lcd.begin();
  lcd.led(225);
  lcd.fillScreen(RGB(255, 255, 255));
  Serial.begin(115200);
  
}
int k = 0;
int l = 0;
int x = 4;
int y = 5;
void loop() {  

lcd.fillRect(240, 5, 76 ,231 ,RGB(0,0,0));
  for(int k =0;k < 11;k++){
    //row
    for(int l = 0;l < 11;l++){
      if(levelGrid[l][k] == WALL){
      drawWall();
      }else if(levelGrid[l][k] == BARREL){
        drawBarrel();
      }else if(levelGrid[l][k] == PLAYER){
        drawPlayer();
      }
        y += 21;
    } 
    y = 5; 
    x+= 21;
  }
  x=4;
}

void drawBarrel(){
  lcd.fillRect(x, y, 21, 21, RGB(255,0,0));
  lcd.drawRect(x, y, 21, 21, RGB(0, 0, 0));
}

void drawWall(){
  lcd.fillRect(x, y, 21, 21, RGB(153,76,0));
  lcd.drawRect(x, y, 21, 21, RGB(102, 51, 0));
}

void drawPlayer(){
  lcd.fillTriangle(x,y,x+21,y,x+10,y+21, RGB(0,0,0));
}

