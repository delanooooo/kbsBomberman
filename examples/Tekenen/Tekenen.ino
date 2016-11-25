void drawBarrel() {
    lcd.fillRect(x + 1, y + 1, 19, 19, RGB(102, 0, 0));
    lcd.drawRect(x, y, 21, 21, RGB(0, 0, 0));
    lcd.drawRect(x, y + 7, 21, 8, RGB(0, 0, 0)); 
}

void drawWall() {
    lcd.fillRect(x, y, 21, 21, RGB(153, 76, 0));
    lcd.drawRect(x, y, 21, 21, RGB(102, 51, 0));
}

void drawPlayer() {
    lcd.fillRect(playLocX * 21 + 10, playLocY * 21 + 6, 11, 16, RGB(255, 255, 0));   // body
    lcd.fillRect(playLocX * 21 + 13, playLocY * 21 + 9, 2, 2, RGB(0, 0, 0));       // eye
    lcd.fillRect(playLocX * 21 + 16, playLocY * 21 + 9, 2, 2, RGB(0, 0, 0));      // eye
    lcd.drawEllipse(playLocX * 21 + 15, playLocY * 21 + 13, 5, 2, RGB(0, 0, 0));   // smile
    lcd.fillRect(playLocX * 21 + 10, playLocY * 21 + 10, 11, 4, RGB(255, 255, 0));    // smile
    
    lcd.drawLine(playLocX * 21 + 11, playLocY * 21 + 22, playLocX * 21 + 11, playLocY * 21 + 25, RGB(0, 0, 0));    // left leg
    lcd.drawLine(playLocX * 21 + 19, playLocY * 21 + 22, playLocX * 21 + 19, playLocY * 21 + 25, RGB(0, 0, 0));  // right leg
    lcd.drawLine(playLocX * 21 + 9, playLocY * 21 + 14, playLocX * 21 + 9, playLocY * 21 + 19, RGB(0, 0, 0));     // left arm
    lcd.drawLine(playLocX * 21 + 21, playLocY * 21 + 14, playLocX * 21 + 21, playLocY * 21 + 19, RGB(0, 0, 0));   // right arm
    lcd.drawLine(playLocX * 21 + 10, playLocY * 21 + 22, playLocX * 21 + 20, playLocY * 21 + 22, RGB(0, 0, 0));    // belt    
}

void drawEmpty() {
    lcd.fillRect(playLocX * 21 + 4, playLocY * 21 + 5, 21, 21, RGB(255, 255, 255));
}

void placeBomb() {
    if (bombsPlaced < bombAmount) {
        levelGrid[playLocY][playLocX] = BOMB;
        lcd.fillRect(playLocX * 21 + 4, playLocY * 21 + 5, 21, 21, RGB(255, 255, 255));
        lcd.fillEllipse(playLocX * 21 + 7 + 4, playLocY * 21 + 7 + 5, 7, 7, RGB(0,0,0));
        lcd.fillRect(playLocX * 21 + 7 + 4, playLocY * 21 + 5 + 5, 8, 5, RGB(0,0,0)); 
        lcd.drawLine(playLocX * 21 + 15 + 4, playLocY * 21 + 6 + 5, playLocX * 21 + 17 + 4, playLocY * 21 + 3 + 5, RGB(255,0,0));
        bombsPlaced += 1;
    }
}
