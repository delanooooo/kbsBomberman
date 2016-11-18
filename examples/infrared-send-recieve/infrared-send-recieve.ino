#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    cli();
    DDRB |= (1 << PINB3) | (1 << PINB0);
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS20);
    OCR2A = 210;
    sei();
}
