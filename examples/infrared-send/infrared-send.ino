#include "infrared-send.h"

typedef struct queueSend {
    uint8_t data[QUEUE_SIZE];
    uint8_t front;
    uint8_t count;
} queueSend;

void queueAdd(queueSend *q, uint8_t d);
void queueCheck(queueSend *q);

queueSend *queue;

int main(void) {
    queue = (queueSend *) malloc(sizeof(queueSend));
    queue->front = 0;
    queue->count = 0;

    ir_setup();

    IR_ENABLE;
    PC_ENABLE;

    for(uint8_t i=0;i<15;i++) sendData(0x41);

    for(;;){
        queueCheck(queue);
        _delay_ms(50);
        queueAdd(queue, 0x37);
        _delay_ms(50);
        queueAdd(queue, 0x36);
        _delay_ms(50);
    }
}

void queueAdd(queueSend *q, uint8_t d) {
    if(q->count < QUEUE_SIZE) {
        if((q->front + q->count) < QUEUE_SIZE) q->data[q->front + q->count] = d;
        else q->data[q->front + q->count-QUEUE_SIZE] = d;
    }
    q->count++;
}

void queueCheck(queueSend *q) {
    if(!data) {
        sendData(q->data[q->front]);
        q->data[q->front] = 0x57;
        q->count--;
        if(q->front < (QUEUE_SIZE-1)) q->front += 1;
        else q->front = 0;
    }
}

ISR(TIMER2_COMPA_vect) {
    timer++;
    if(timer > sendtime) {
        PORTB ^= (1 << PINB4);
    }
}

ISR(PCINT0_vect) {
    if(data) {
        if(PINB & (1 << PINB4)) {
            IR_ENABLE;
            SEND_BUFFER;
            if(!nbit) data = 0x00;
        }
        else {
            IR_DISABLE;
            if(nbit) {
                if(data & nbit) SEND_ONE;
                else            SEND_ZERO;
            } else              SEND_STOP;

            nbit >>= 1;
        }
    }
    else {
        if(!(PINB & (1 << PINB4))) {
            PORTB ^= (1 << PINB4);
        }
    }
}

void sendData(uint8_t d){
    IR_DISABLE;
    SEND_START;
    data = d;
    nbit = 0x80;
}

void ir_setup() {
    cli();

    PCICR |= (1 << PCIE0); // Enable pin change interrupt 0
    PCMSK0 |= (1 << PCINT4); // Listen to PINB4 for pin change interrupt
    EICRA |= (1 << ISC10);

    DDRB |= (1 << PINB5); 
    DDRB |= (0 << PINB4); 
    DDRB |= (1 << PINB3); // infraredpin
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 210; //approximately every 26 microseconds
    //   OCR2B = 16; //approximately every 16 microseconds
    TIMSK2 |= (1 << OCIE2A); //enable timer compare match interupt
    sei();
}
