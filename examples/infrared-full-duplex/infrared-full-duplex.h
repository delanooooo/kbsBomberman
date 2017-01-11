#include <avr/io.h>
#include <util/delay.h>

//  Variables
volatile uint16_t timer = 0;
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

#define SEND_START   sendtime = timer + START_SPACE
#define SEND_STOP    sendtime = timer + STOP_SPACE
#define SEND_BUFFER  sendtime = timer + BIT_SPACE
#define SEND_ONE     sendtime = timer + ONE_SPACE
#define SEND_ZERO    sendtime = timer + ZERO_SPACE

#define IR_DISABLE DDRB &= ~(1 << PINB3)
#define IR_ENABLE  DDRB |= (1 << PINB3)

#define PC_DISABLE DDRB &= ~(1 << PINB4)
#define PC_ENABLE  DDRB |= (1 << PINB4)

// Prototypes
void ir_setup();
void sendData(uint8_t data);

void IR_setup();
void readValue();
