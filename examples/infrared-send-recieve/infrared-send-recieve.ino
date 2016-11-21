#include <avr/io.h>
#include <util/delay.h>
//#include <Arduino.h>

void sendZero();

uint16_t time = 0;
int main(void) {
    cli();
    DDRB |= (1 << PINB3);													//output pin for LED
    DDRB &= ~(1 << PINB0);													//input pin for IR sensor
    TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);	
    TCCR2B |= (1 << WGM22) | (1 << CS20);									
    OCR2A = 210;															//approximately every 26 microseconds
    TIMSK2 |= (1 << OCIE2A);												//enable timer compare match interupt
    sei();

//     Serial.begin(9600);
//     Serial.println("Starting...");

    for(;;) {
        //    uint8_t rec;
        //   rec = PINB & (1 << PINB0);
        //  Serial.println(rec);
/*        Serial.println(time);*/
        //sendZero();
		//_delay_ms(3000);

    }
}

ISR(TIMER2_COMPA_vect) {
    time++;
}

void sendZero(){								//sends a zero by turning of the LED for approximately 60 microseconds
    DDRB ^= (1 << PINB3);
    time = 0;
    while(time < 3){
        DDRB ^= (1 << PINB3);
/*        Serial.println("You sent a zero");*/
    }
}
	
void sendOne(){									//sends a one by turning of the LED for approximately 120 microseconds
	 DDRB ^= (1 << PINB3);
	 time = 0;
	 while(time < 6){
		 DDRB ^= (1 << PINB3);
/*		 Serial.println("You sent a one");*/
	 }
}
