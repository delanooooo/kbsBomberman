#include "infrared-full-duplex.h"

typedef struct queueSend {
    uint8_t data[QUEUE_SIZE];
    uint8_t front;
    uint8_t count;
} queueSend;

void queueAdd(queueSend *q, uint8_t d);
void queueCheck(queueSend *q);

queueSend *queue;

/*
int main(void){
    queue = (queueSend *) malloc(sizeof(queueSend));
    queue->front = 0;
    queue->count = 0;

    ir_setup();

    IR_ENABLE;
    PC_ENABLE;

    for(;;){
        sendData(0x41);
        _delay_ms(10);
    }
}
*/

void queueAdd(queueSend *q, uint8_t d) {
    if(q->count < QUEUE_SIZE) {
        if((q->front + q->count) < QUEUE_SIZE) q->data[q->front + q->count] = d;
        else q->data[q->front + q->count-QUEUE_SIZE] = d;
    }
    q->count++;
}

void queueCheck(queueSend *q) {
    if(!sentData) {
        sendData(q->data[q->front]);
        q->data[q->front] = 0x57;
        q->count--;
        if(q->front < (QUEUE_SIZE-1)) q->front += 1;
        else q->front = 0;
    }
}

void sendData(uint8_t d){
    IR_DISABLE;
    SEND_START;
    sentData = d;
    nbit = 0x80;
}

uint8_t readValue(){
    // Serial.println(measuredTime);

    //measuredTime decides what kind of signal we just received
    //this signal can be a start, stop, 1 or 0 bit
    //only start processing measuredTime if we actually received a start bit

    if(measuredTime >= 56 && measuredTime <= 80){   //start bit
        startCollecting = 1;
        //Serial.println("(");
        receivedData = 0;
    }
    if(startCollecting > 0){
        if(measuredTime >= 34 && measuredTime < 56){    //'1' bit
            receivedData <<= 1;     //shift in a new bit
            receivedData |= 0x01;   //set the newly added bit to 1
            //Serial.print("1");
        } else if(measuredTime < 34){   //'0' bit
            receivedData <<= 1;                 //shift in a new 0 bit
            //Serial.print("0");
        } else if(measuredTime > 80 && measuredTime < 110){ //stop bit
            //wait for a new start signal, discard every other signal
            startCollecting = 0;
            Serial.print(receivedData , HEX);
            Serial.print("\n");
            return receivedData;    //received byte
        }
    }
}

ISR(TIMER2_COMPA_vect){
    timer++;
    if(timer > sendtime) {
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
        //so we timestamp the value our timer is on
        measuredTime = timer;

    } else {
        //falling edge means the bit is completed,
        //so we can look at our current time
        //if measuredTime is bigger the timer has reached it's maximum and overflowed
        if(measuredTime > timer){
            //our measuredTime is off by 0xFFFF or 65535
            measuredTime = 0xFFFF - measuredTime + timer;
        } else {
            measuredTime = timer - measuredTime; //elapsed time
        }
        readValue();
    }
}

uint8_t readValueOld(){
    if(measuredTime >= 145 && measuredTime <= 200){
        startCollecting = 1;
    }
    if(startCollecting > 0){
        if(measuredTime > 110 && measuredTime < 145){
            receivedData <<= 1;
            receivedData |= 0x01;
        } else if(measuredTime <= 110){
            receivedData <<= 1;
        } else if(measuredTime > 200 && measuredTime < 260){
            startCollecting = 0;
            Serial.print(receivedData );
            Serial.print("\n");
            receivedData = 0;
            return receivedData;
        }
    }
}

void ir_setup(){
    cli();

    //Pin change interrupt
    PCICR = (1 << PCIE0) | (1 << PCIE2); //pin group for PORTD
    PCMSK0 |= (1 << PCINT4); // Listen to PINB4 for pin change interrupt
    PCMSK2 |= (1 << PCINT19); //PIND3 / digital pin 3

    // moet nog naar gekeken worden vvvvvv
    EICRA = (1 << ISC11) | (1 << ISC00); //create interrupt on any logical change
    EICRA |= (1 << ISC10);
    // moet nog naar gekeken worden ^^^^^^

    /*Timer*/
    //listen for interrupts on compare match a
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20); //no prescaler
    OCR2A = 210; //210 corresponds to 13 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt

    DDRB |= (0 << PINB4);
    DDRB |= (1 << PINB3); // infraredpin
    sei();
}
