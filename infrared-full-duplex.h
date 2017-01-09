#include <avr/io.h>
#include <util/delay.h>

//  Variables
volatile uint16_t datatimer = 0;
volatile uint16_t sendtime = 0;
volatile uint16_t measuredTime = 0;

char receivedData;
uint8_t startCollecting;

volatile uint8_t nbit = 0;
volatile uint8_t sentData = 0;

//  Defines
//  Amount of cycles set to send a specific signal
#define START_SPACE    65
#define STOP_SPACE     90
#define ONE_SPACE      40
#define ZERO_SPACE     20

#define BIT_SPACE      20

#define SEND_START   sendtime = datatimer + START_SPACE
#define SEND_STOP    sendtime = datatimer + STOP_SPACE
#define SEND_BUFFER  sendtime = datatimer + BIT_SPACE
#define SEND_ONE     sendtime = datatimer + ONE_SPACE
#define SEND_ZERO    sendtime = datatimer + ZERO_SPACE

#define IR_DISABLE DDRB &= ~(1 << PINB3)
#define IR_ENABLE  DDRB |= (1 << PINB3)

#define PC_DISABLE DDRB &= ~(1 << PINB4)
#define PC_ENABLE  DDRB |= (1 << PINB4)

#define QUEUE_SIZE 100

// Prototypes
void ir_setup();
void sendData(uint8_t data);

uint8_t readValueOld();
uint8_t readValue();

// Interrupt service routines
ISR(TIMER2_COMPA_vect){
    datatimer++;
    if(datatimer > sendtime) {
        PORTB ^= (1 << PINB4);
    }
}

ISR(PCINT0_vect) {
    if(sentData) {
        if(PINB & (1 << PINB4)) {
            IR_ENABLE;
            if(nbit) SEND_BUFFER;
            else sentData = 0x00;
        }
        else {
            IR_DISABLE;
            if(nbit) {
                if(sentData & nbit) SEND_ONE;
                else            SEND_ZERO;
            } else              SEND_STOP;

            nbit >>= 1;
        }
    }
}

//This interrupt triggers if there is any change coming from the sensor
ISR(PCINT2_vect){
    //check what state the sensor is in, rising or falling edge
    if(PIND & (1 << PIND3)){
        //rising edge means we have a new bit incoming,
        //so we timestamp the value our datatimer is on
        measuredTime = datatimer;

    } else {
        //falling edge means the bit is completed,
        //so we can look at our current time
        //if measuredTime is bigger the datatimer has reached it's maximum and overflowed
        if(measuredTime > datatimer){
            //our measuredTime is off by 0xFFFF or 65535
            measuredTime = 0xFFFF - measuredTime + datatimer;
        } else {
            measuredTime = datatimer - measuredTime; //elapsed time
        }
        readValue();
    }
}
