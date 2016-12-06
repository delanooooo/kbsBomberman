#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#define SWITCH 500

void sendZero();
void sendOne();
void ir_setup();

volatile uint16_t sensor = 0x00;
volatile uint16_t timer = 0;
volatile uint16_t timer2 = 0;
volatile uint16_t squarewave_timer = 0;

int main(void) {
    Serial.begin(9600);
    ir_setup();
    for(;;) {
		_delay_ms(SWITCH);
        sendZero();
        PORTB &= ~(1 << PINB5);
 		_delay_ms(SWITCH);
 		sendOne();
        PORTB |= (1 << PINB5);
    }
}

ISR(TIMER2_COMPA_vect) {
    timer++;
}

ISR(TIMER2_COMPB_vect) {
    timer2++;
    Serial.println(timer2);
    if(squarewave_timer > timer2) DDRB |= (1 << PINB3);
}

ISR(PCINT0_vect) {
    sensor++;
}

void sendZero(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    squarewave_timer = timer2 + 5;
}

void sendOne() {
	DDRB &= ~(1 << PINB3); // Turn IR led off
	_delay_us(182);
	DDRB |= (1 << PINB3);
}

void ir_setup() {
    cli();

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1<< PCINT0);
    EICRA = (1 << ISC11) | (1 << ISC10);

    DDRB = (1 << PINB3) | (1 << PINB5); //output pin for LED
    DDRB &= ~(1 << PINB0); //input pin for IR sensor
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS21);
    OCR2A = 0x1A; //approximately every 26 microseconds
    OCR2B = 16; //approximately every 26 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt
    sei();
}
