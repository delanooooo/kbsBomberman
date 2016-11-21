#include <avr/io.h>
#include <util/delay.h>
//#include <Arduino.h>

uint16_t millisecond = 0;
int main(void) {
    //    init();

    cli();
	DDRB |= (1 << PINB3);
	DDRB &= ~(1 << PINB0);
	TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22) | (1 << CS20);
	OCR2A = 210;
    //timer();
    sei();

    Serial.begin(9600);
    Serial.println(1);

    for(;;) {
        uint8_t rec;
        rec = PINB & (1 << PINB0);
        Serial.println(rec);
    }
}
/*
ISR(TIMER2_OVF_vect) {
    millisecond++;
    if(millisecond > 26) {
        DDRB &= ~(1 << PINB3);
    }
}
*/
//int timer(){
//}
