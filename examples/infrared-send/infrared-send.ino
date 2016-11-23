#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void sendZero();
void own_init();

uint16_t sensor = 0x00;
uint16_t timer = 0;

int main(void) {
    own_init();

    Serial.begin(9600);
    Serial.println("Starting...");
    _delay_ms(3000);

    for(;;) {
		_delay_ms(1000);
        sendZero();
// 		_delay_ms(1000);
// 		sendOne();
    }
}

ISR(TIMER2_COMPA_vect) {
    timer++;
}

ISR(PCINT0_vect) {
    sensor++;
}

void sendZero(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
	_delay_us(65);
	DDRB |= (1 << PINB3);
//     while(timer > 3){
//         DDRB |= (1 << PINB3);
//         PORTB |= (1 << PINB5);
//         timer = 0;
//     }
}

void sendOne() {
	DDRB &= ~(1 << PINB3); // Turn IR led off
	_delay_us(120);
	DDRB |= (1 << PINB3);
}

void own_init() {
    cli();

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1<< PCINT0);
    EICRA = (1 << ISC11) | (1 << ISC10);

    DDRB = (1 << PINB3) | (1 << PINB5); //output pin for LED
    DDRB &= ~(1 << PINB0); //input pin for IR sensor
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS21);
    OCR2A = 26; //approximately every 26 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt
    sei();
}
