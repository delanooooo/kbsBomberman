#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void led1();
void led2();
void own_init();

uint8_t measuredTime;
uint8_t number = 0;
uint8_t timer1 = 0;
uint8_t sensor = 0x00;
int main(void){
    own_init();
    led1();
    led2();
    Serial.println("Starting...");
    while(1){
        if(measuredTime > 0) led1();
        else led2();
    }
}	

void led1(){
    PORTB &= ~(1 << PINB2);
    PORTB &= ~(1 << PINB1);
    PORTB |= (1 << PINB1);
}
void led2(){
    PORTB &= ~(1 << PINB1);
    PORTB &= ~(1 << PINB2);
    PORTB |= (1 << PINB2);
    _delay_ms(500);
}

void own_init(){
    cli();
    DDRB = (1 << PORTB2) | (1 << PORTB1);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1<< PCINT0);
    EICRA = (1 << ISC11) | (1 << ISC10);

    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS21);
    OCR2A = 45; 
    TIMSK2 |= (1 << OCIE2A);
    Serial.begin(9600);
    sei();
}
ISR(PCINT0_vect){
    sensor = PINB & (1<<PINB0);
    timer1 = 0;
    while(!sensor) {
        sensor = PINB & (1<<PINB0);
    }
    measuredTime = timer1;
}
ISR(TIMER2_COMPA_vect){
    timer1++;
}
