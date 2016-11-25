void drawBarrel() {
    lcd.fillRect(x + 1, y + 1, 19, 19, RGB(102, 0, 0));
    lcd.drawRect(x, y, blockSize, blockSize, RGB(0, 0, 0));
    lcd.drawRect(x, y + 7, blockSize, 8, RGB(0, 0, 0)); 
}

void drawWall() {
    lcd.fillRect(x, y, blockSize, blockSize, RGB(153, 76, 0));
    lcd.drawRect(x, y, blockSize, blockSize, RGB(102, 51, 0));
}

void drawPlayer() {
    lcd.fillRect(playLocX * blockSize + 10, playLocY * blockSize + 6, 11, 16, RGB(255, 255, 0));   // body
    lcd.fillRect(playLocX * blockSize + 13, playLocY * blockSize + 9, 2, 2, RGB(0, 0, 0));       // eye
    lcd.fillRect(playLocX * blockSize + 16, playLocY * blockSize + 9, 2, 2, RGB(0, 0, 0));      // eye
    lcd.drawEllipse(playLocX * blockSize + 15, playLocY * blockSize + 13, 5, 2, RGB(0, 0, 0));   // smile
    lcd.fillRect(playLocX * blockSize + 10, playLocY * blockSize + 10, 11, 4, RGB(255, 255, 0));    // smile
    
    lcd.drawLine(playLocX * blockSize + 11, playLocY * blockSize + 22, playLocX * blockSize + 11, playLocY * blockSize + 25, RGB(0, 0, 0));    // left leg
    lcd.drawLine(playLocX * blockSize + 19, playLocY * blockSize + 22, playLocX * blockSize + 19, playLocY * blockSize + 25, RGB(0, 0, 0));  // right leg
    lcd.drawLine(playLocX * blockSize + 9, playLocY * blockSize + 14, playLocX * blockSize + 9, playLocY * blockSize + 19, RGB(0, 0, 0));     // left arm
    lcd.drawLine(playLocX * blockSize + 21, playLocY * blockSize + 14, playLocX * blockSize + 21, playLocY * blockSize + 19, RGB(0, 0, 0));   // right arm
    lcd.drawLine(playLocX * blockSize + 10, playLocY * blockSize + 22, playLocX * blockSize + 20, playLocY * blockSize + 22, RGB(0, 0, 0));    // belt    
}

void drawEmpty() {
    lcd.fillRect(playLocX * blockSize + 4, playLocY * blockSize + 5, blockSize, blockSize, RGB(255, 255, 255));
}

void placeBomb() {
    if (bombsPlaced < bombAmount) {
        levelGrid[playLocY][playLocX] = BOMB;
        drawEmpty();
        lcd.fillEllipse(playLocX * blockSize + 7 + 4, playLocY * blockSize + 7 + 5, 7, 7, RGB(0,0,0));
        lcd.fillRect(playLocX * blockSize + 7 + 4, playLocY * blockSize + 5 + 5, 8, 5, RGB(0,0,0)); 
        lcd.drawLine(playLocX * blockSize + 15 + 4, playLocY * blockSize + 6 + 5, playLocX * blockSize + 17 + 4, playLocY * blockSize + 3 + 5, RGB(255,0,0));
        bombsPlaced += 1;
    }
}
