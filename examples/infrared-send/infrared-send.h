#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// Variables
volatile uint16_t sensor = 0;
volatile uint16_t timer = 0;
volatile uint16_t sendtime = 0;

// Defines
#define SWITCH       5000
#define START_SPACE   325
#define STOP_SPACE    416
#define ONE_SPACE     234
#define ZERO_SPACE     65
#define BIT_SPACE     494

#define IR_DISABLE DDRB &= ~(1 << PINB3)
#define IR_ENABLE  DDRB |= (1 << PINB3)

// Prototypes
void ir_setup();
void sendBit();
void sendStart();            void sendStartOld();
void sendStop();             void sendStopOld();
void sendZero();             void sendZeroOld();
void sendOne();              void sendOneOld();
void sendData(uint8_t data); void sendDataOld(uint8_t data);

ISR(TIMER2_COMPA_vect) {
    timer++;
    if(timer > sendtime) IR_ENABLE;
}

ISR(PCINT0_vect) {
    sensor++;
}

void sendData(uint8_t data){
    sendStart(); 
    _delay_us(BIT_SPACE);
    for(uint8_t mask = 0x01 << 7; mask; mask >>= 1){
        if(mask & data) {
            sendOne();  
            _delay_us(BIT_SPACE);
        } else { 
            sendZero(); 
            _delay_us(BIT_SPACE);
        }
    }
    sendStop(); 
    _delay_us(BIT_SPACE);
}

void sendDataOld(uint8_t data){
    sendStartOld(); _delay_us(BIT_SPACE);
    for(uint8_t mask = 0x01 << 7; mask; mask >>= 1){
        if(mask & data) {
            sendOneOld();  _delay_us(BIT_SPACE);
        } else { 
            sendZeroOld(); _delay_us(BIT_SPACE);
        }
    }
    sendStopOld(); _delay_us(BIT_SPACE);
}

void sendBit(){
    IR_DISABLE;
    sendtime = timer + (BIT_SPACE / 13);
}

void sendStart(){                             
    IR_DISABLE;
    sendtime = timer + (START_SPACE / 13);
}

void sendStartOld(){                             
    IR_DISABLE;
    _delay_us(START_SPACE);
    IR_ENABLE;
}

void sendStop(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    sendtime = timer + (STOP_SPACE / 13);
}

void sendStopOld(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(STOP_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
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
    sendtime = timer + (ONE_SPACE / 13);
}

void sendOneOld() {
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(ONE_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
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
