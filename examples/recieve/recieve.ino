#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void led1();
void led2();
void own_init();

char data = 0x00;
uint8_t bits = 0;
uint8_t startCollecting;
volatile uint16_t measuredTime;
uint8_t number = 0;
volatile uint8_t timer1 = 0;
volatile uint8_t sensor = 0x00;
int main(void){
    IR_setup();
	led1();
	led2();
	PORTB &= ~(1 << PINB1);
    while(1){
/*		Serial.println(bits);*/
    }
}	

void readValue(){
	
	if(measuredTime > 170 && measuredTime <= 210){
		PORTB ^= (1 << PINB2);
		startCollecting = 1;
/*		Serial.print("s");*/
	}
	if(startCollecting > 0){
		if(measuredTime > 110 && measuredTime < 160){
			PORTB |= (1 << PINB2);
			PORTB &= ~(1 << PINB1);
			bits++;
/*			Serial.print("1");*/
			data <<= 1;
			data |= 0x01;
			} else if(measuredTime <= 110){
			PORTB |= (1 << PINB1);
			PORTB &= ~(1 << PINB2);
			bits++;
/*			Serial.print("0");*/
			data <<= 1;
		}
		if(measuredTime > 210 && measuredTime < 260){
			startCollecting = 0;
/*			Serial.print("\n-----\n");*/
			Serial.println(data);
/*			Serial.print("-----\n\n");*/
		}
	}
 }

void led1(){
	PORTB |= (1 << PINB2);
	PORTB &= ~(1 << PINB1);
	_delay_ms(100);
}

void led2(){
	PORTB |= (1 << PINB1);
	PORTB &= ~(1 << PINB2);
	_delay_ms(100);
}

void IR_setup(){
    cli();
	Serial.begin(9600);
    DDRB = (1 << PORTB2) | (1 << PORTB1);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1<< PCINT0);
    EICRA = (1 << ISC11) | (1 << ISC00);

    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 40; 
    TIMSK2 |= (1 << OCIE2A);
    sei();
}
ISR(PCINT0_vect){
    sensor = PINB & (1<<PINB0);
	if(sensor > 0){
		timer1 = 0;
	} else {
		measuredTime = timer1;
		readValue();
	}
}
ISR(TIMER2_COMPA_vect){
    timer1++;
}
