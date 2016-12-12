#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#define SWITCH       5000
#define START_SPACE   312
#define STOP_SPACE    403
#define ONE_SPACE     182
#define ZERO_SPACE     52
#define BIT_SPACE     494

void sendStart();
void sendStop();
void sendZero();
void sendZeroOld();
void sendOne();
void ir_setup();
void sendData(uint8_t data);

volatile uint16_t sensor = 0x00;
volatile uint16_t timer = 0;
volatile uint16_t sendtime = 0;

int main(void) {
    ir_setup();
    PORTB ^= (1 << PINB5);
    //for(uint8_t i = 0; i < 200; i++) { sendZero(); sendOne(); PORTB ^= (1 << PINB5); }
    PORTB ^= (1 << PINB5);

    sendData(0x43);
    _delay_ms(500);
    PORTB ^= (1 << PINB5);
    for(;;){
        sendZero();
        PORTB ^= (1 << PINB5);
        _delay_ms(500);
    }


}

ISR(TIMER2_COMPA_vect) {
    timer++;
    if(sendtime > timer) DDRB |= (1 << PINB3);
}

ISR(PCINT0_vect) {
    sensor++;
}

void sendData(uint8_t data){
    sendStart(); _delay_us(BIT_SPACE);
    for(uint8_t mask = 0x01 << 7; mask; mask >>= 1){
        if(mask & data) {
            sendOne();  _delay_us(BIT_SPACE);
        } else { 
            sendZero(); _delay_us(BIT_SPACE);
        }
    }
    sendStop(); _delay_us(BIT_SPACE);
}

void sendBit(){
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(START_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
}

void sendStart(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    sendtime = timer + (START_SPACE / 13);
}

void sendStop(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    sendtime = timer + (STOP_SPACE / 13) + 5;
}

void sendZero(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    sendtime = timer + (ZERO_SPACE / 13);
}

void sendZeroOld(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(ZERO_SPACE);
    DDRB |= (1 << PINB3);
}

void sendOne() {
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(ONE_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
    //    spacetimer = timer + ONE_SPACE;
}

void ir_setup() {
    cli();

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1<< PCINT0);
    EICRA = (1 << ISC11) | (1 << ISC10);

    DDRB = (1 << PINB3) | (1 << PINB5); //output pin for LED
    DDRB &= ~(1 << PINB0); //input pin for IR sensor
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 210; //approximately every 26 microseconds
    //   OCR2B = 16; //approximately every 16 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt
    sei();
}
