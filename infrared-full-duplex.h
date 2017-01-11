#pragma once

extern volatile uint16_t datatimer;
extern volatile uint16_t sendtime;

extern volatile uint16_t measuredTime;

extern volatile uint8_t sentData;
extern volatile uint8_t nbit;

extern uint8_t receivedData;
extern uint8_t startCollecting;

/// Defines
// pins used for sending data
#define PIN_IR       PIND6
#define PIN_PCINT    PIND2

// amount of (timer)cycles set to send a specific signal
#define START_SPACE  65
#define STOP_SPACE   90
#define ONE_SPACE    40
#define ZERO_SPACE   20
#define BIT_SPACE    20

// defines used to start sending a specific type of signal
#define SEND_START   sendtime = datatimer + START_SPACE     // start sending a start signal
#define SEND_STOP    sendtime = datatimer + STOP_SPACE      // start sending a stop signal
#define SEND_ONE     sendtime = datatimer + ONE_SPACE       // start sending a one bit
#define SEND_ZERO    sendtime = datatimer + ZERO_SPACE      // start sending a zero bit
#define SEND_BUFFER  sendtime = datatimer + BIT_SPACE       // used inbetween other send_spaces to be able to differentiate

// enable/disable infrared led
#define IR_DISABLE   DDRD &= ~(1 << PIN_IR)
#define IR_ENABLE    DDRD |= (1 << PIN_IR)
// enable/disable ability to trigger pin change interrupts
#define PC_DISABLE   DDRD &= ~(1 << PIN_PCINT)  
#define PC_ENABLE    DDRD |= (1 << PIN_PCINT)

/// Prototypes
void initIR();                  // set the correct registers to be able to use infrared

void sendData(uint8_t data);    // send an 8bit byte
void readValue();               // read data sent by another infrared led
