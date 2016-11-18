/*
  GraphicsLib Demo

  This sketch demonstrates the functions of the GraphicsLib.
*/
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
#include <avr/interrupt.h>
int x = 50, y = 50;
int oldX = x; int oldY = y;
volatile int counter = 0;
volatile int sec = 58;
MI0283QT9 lcd;  //MI0283QT9 Adapter v1

byte zbut, cbut, joyx, joyy;
int varSpeed = 5;
void setup()
{
	nunchuck_setpowerpins();
	nunchuck_init(); // send the initilization

	//init display
	lcd.begin();
	lcd.led(225);
	lcd.fillScreen(RGB(255, 255, 255));
	Serial.begin(115200);
	timeFunc();
}

void loop()
{

	nunchuck_get_data();

	zbut = nunchuck_zbutton();
	if (zbut && varSpeed > 1) {
		varSpeed--;
	};
	cbut = nunchuck_cbutton();
	if (cbut) {
		varSpeed++;
	};

	joyx = nunchuck_joyx();
	joyy = nunchuck_joyy();

	if (joyx < 100)//to the left
	{
		x -= varSpeed;
		lcd.fillRect(oldY, oldX, 10, 10, RGB(255, 255, 255));
	}
	else if (joyx > 160) // to the right
	{
		x += varSpeed;
		lcd.fillRect(oldY, oldX, 10, 10, RGB(255, 255, 255));
	}
	if (joyy > 160)// to the top
	{
		y += varSpeed;
		lcd.fillRect(oldY, oldX, 10, 10, RGB(255, 255, 255));
	}
	else if (joyy < 100) // to the bottom
	{
		y -= varSpeed;
		lcd.fillRect(oldY, oldX, 10, 10, RGB(255, 255, 255));
	}

	if (x < 0) {//left
		x = 0;
	}
	if (x > 230) { //right
		x = 230;
	}
	if (y < 0) { //bottom
		y = 0;
	}
	if (y > 310) { // top
		y = 310;
	}
	lcd.fillRect(y, x, 10, 10, RGB(255, 0, 0));

	oldX = x;
	oldY = y;
	lcd.setOrientation(90);
	lcd.setCursor(0, 0);
	lcd.println(sec);

	Serial.println(sec);
	lcd.setOrientation(0);
	
}

void timeFunc()
{
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
	OCR2A = 125;
	TIMSK2 |= (1 << OCIE2A);
	sei();

}
ISR(TIMER2_COMPA_vect) {
	if (counter >= 125)
	{
		if (sec == 59)
		{
			sec = 0;
			cleanText();
				
		}
		else
		{
			sec++;
		}
				
		counter = 0;
	}
	counter++;

}

void cleanText()
{
	lcd.setOrientation(90);
	lcd.fillRect(0, 0, 20, 10, RGB(255, 255, 255));
	lcd.setOrientation(0);
}

