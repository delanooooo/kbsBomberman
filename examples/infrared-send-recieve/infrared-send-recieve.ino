#include <avr/io.h>
#include <util/delay.h>
//#include <Arduino.h>

void sendZero();

uint16_t time = 0;
int main(void) {
    //    init();

    cli();
    DDRB |= (1 << PINB3);
    DDRB &= ~(1 << PINB0);
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 210;
    TIMSK2 |= (1 << OCIE2A);
    //timer();
    sei();

    Serial.begin(9600);
    Serial.println("Starting...");

    for(;;) {
        //    uint8_t rec;
        //   rec = PINB & (1 << PINB0);
        //  Serial.println(rec);

        //_delay_ms(3000);
        //sendZero();
        Serial.println(time);
        sendZero();
    //    _delay_ms(3000);
    }
}

ISR(TIMER2_COMPA_vect) {
    time++;
}

void sendZero(){
    DDRB ^= (1 << PINB3);
    Serial.println("NO");
    time = 0;
    while(time < 3){
        DDRB ^= (1 << PINB3);
        Serial.println("YES");
    }
}
