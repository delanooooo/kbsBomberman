#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>


//Functions
void IR_setup();
uint8_t readValueOld();
uint8_t readValue();

//Variables
char data;
uint8_t startCollecting;
volatile uint16_t measuredTime;
volatile uint16_t timer = 0;
