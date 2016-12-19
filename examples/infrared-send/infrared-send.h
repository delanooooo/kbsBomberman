#include <avr/io.h>
#include <util/delay.h>

// Variables
volatile uint16_t sensor =      0x00;
volatile uint16_t timer =       0x00;
volatile uint16_t buffer =      0x00;
volatile uint16_t sendtime =    0x00;
volatile uint8_t  nbit =        0x00;
volatile uint8_t  data =        0x00;

// Defines
#define SWITCH       5000
#define START_SPACE   312
#define STOP_SPACE    416
#define ONE_SPACE     234
#define ZERO_SPACE     52
#define BIT_SPACE     494

#define IR_DISABLE DDRB &= ~(1 << PINB3)
#define IR_ENABLE  DDRB |= (1 << PINB3)

#define PC_DISABLE DDRB &= ~(1 << PINB4)
#define PC_ENABLE  DDRB |= (1 << PINB4)

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
//  if(timer > buffer)   IR_DISABLE;
//  if(timer > sendtime) IR_ENABLE;
}

//void sendData(uint8_t d){
//    timer = 0;
//    nbit = (0x01 << 7);
//    data = d;
//    sendtime = timer + (START_SPACE / 13);
//    buffer = sendtime + (BIT_SPACE / 13);
//    IR_DISABLE;
//}


ISR(INT1_vect) {
    DDRB |= (1 << PINB5);
}

void sendData(uint8_t data){
    sendStart(); 
    uint8_t mask = 0x01 << 7;
    while(mask){
        if(mask & data) sendOne();
        else            sendZero();
        mask >>= 1;
        while(timer < buffer){}
    }
    sendStop();
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
    sendtime = timer + (BIT_SPACE / 13);
}

void sendStartOld(){                             
    IR_DISABLE;
    _delay_us(START_SPACE);
    IR_ENABLE;
}

void sendStop(){                             
    buffer = timer + (STOP_SPACE / 13);
    sendtime = buffer + (BIT_SPACE / 13);
}

void sendStopOld(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(STOP_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
}

void sendZero(){                             
    buffer = timer + (ZERO_SPACE / 13);
    sendtime = buffer + (BIT_SPACE / 13);
}

void sendZeroOld(){                             
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(ZERO_SPACE);
    DDRB |= (1 << PINB3);
}

void sendOne() {
    if(timer > sendtime){
        timer = 0;
        buffer = timer + (ONE_SPACE / 13);
        sendtime = buffer + (BIT_SPACE / 13);
    }
}

void sendOneOld() {
    DDRB &= ~(1 << PINB3); // Turn IR led off
    _delay_us(ONE_SPACE);
    DDRB |= (1 << PINB3); // Turn IR led off
}

void ir_setup() {
    cli();

    PCICR |= (1 << PCIE2);   // Enable pin change interrupt 0
    PCMSK0 |= (1 << PCINT0); // Listen to PINB4 for pin change interrupt
    EICRA |= (1 << ISC10);
    EIMSK |= (1 << INT1);

    DDRB |= (1 << PINB5); 
    DDRB |= (0 << PINB4); 
    DDRB |= (1 << PINB3); //output pin for LED
    DDRD |= (1 << PINB2);
    DDRB &= ~(1 << PINB0); //input pin for IR sensor
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 210; //approximately every 26 microseconds
    //   OCR2B = 16; //approximately every 16 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt
    sei();
}
