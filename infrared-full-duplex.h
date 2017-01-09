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

void IR_setup();
uint8_t readValueOld();
uint8_t readValue();
