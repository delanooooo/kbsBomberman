#include "infrared-full-duplex.h"

volatile uint16_t datatimer = 0;
volatile uint16_t sendtime = 0xFFFF;
volatile uint16_t measuredTime = 0;

uint8_t receivedData;
uint8_t startCollecting;

volatile uint8_t nbit = 0;
volatile uint8_t sentData = 0;

void sendData(uint8_t data){
    IR_DISABLE;         //disable IR because we're going to start sending data
    SEND_START;         //start out with a start signal
    sentData = data;    //set byte we want to send
    nbit = 0x80;        //used to determine what bit needs to be send
}

void readValue(){
    //measuredTime decides what kind of signal we just received
    //this signal can be a start, stop, 1 or 0 bit
    //only start processing measuredTime if we actually received a start bit

    if(measuredTime >= 56 && measuredTime <= 80){   //start bit
        startCollecting = 1;
        receivedData = 0;
    }

    if(startCollecting > 0){
        if(measuredTime >= 34 && measuredTime < 56){    //'1' bit
            receivedData <<= 1;     //shift in a new bit
            receivedData |= 0x01;   //set the newly added bit to 1
        } else if(measuredTime < 34){   //'0' bit
            receivedData <<= 1;                 //shift in a new 0 bit
        } else if(measuredTime > 80 && measuredTime < 110){ //stop bit
            //wait for a new start signal, discard every other signal
            startCollecting = 0;
        }
    }
}

ISR(TIMER0_COMPA_vect){
    datatimer++;                    
    if(datatimer > sendtime) {
        //whenever a signal to send is set, the pin for pin change interrupts will toggle,
        //which causes the INT0 interrupt service routine to be called
        PORTD ^= (1 << PIN_PCINT);  
    }
}

//this interrupt triggers whenever a signal is done sending and is used
//to determine which bit needs to be sent
ISR(INT0_vect){

    // only decide to check which signal needs to be sent if there's actually 
    // still data left to be sent
    if(sentData){

        //if this interrupt is triggered while we were not sending a BUFFER signal 
        if(PIND & (1 << PIN_PCINT)){
            IR_ENABLE;

            if(nbit) SEND_BUFFER; //if there are still bits left to send, continue to send buffer
            else {                //if there aren't any bits left to send, prevent INT0 from being triggered again
                sentData = 0x00;
                sendtime = 0xFFFF;
            }
        }

        // if this interrupt is triggerd while we were sending a BUFFER signal
        else {
            IR_DISABLE;

            //if there are still bits left to send, send either a one or a zero
            if(nbit){ 
                if(sentData & nbit) SEND_ONE;
                else SEND_ZERO;
            //if there are not any bits left to send, send a stop signal
            } else SEND_STOP; 

            nbit >>= 1; //make sure the next INT0 trigger will check the next bit
        }
    }
}

//this interrupt triggers if there is any change coming from the sensor
ISR(PCINT2_vect){
    //check what state the sensor is in, rising or falling edge
    if(PIND & (1 << PIND4)){
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

void initIR(){
    cli();

    //Pin change interrupt
    PCICR |= (1 << PCIE2); //pin group for PORTD
    PCMSK2 |= (1 << PCINT20); //PIND4 / digital pin 4

    EICRA |= (1 << ISC00); //create interrupt on any logical change

    //listen for interrupts on compare match a
    TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02) | (1 << CS00); //no prescaler 
    OCR0A = 210; //210 corresponds to 13 microseconds
    TIMSK0 |= (1 << OCIE0A); //enable datatimer compare match interrupt

    DDRD |= (1 << PIN_PCINT); // interrupt pin
    DDRD |= (1 << PIN_IR); // infrared pin

    sei();
}
