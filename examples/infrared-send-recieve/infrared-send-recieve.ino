#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void own_init();
void sendZero();
uint16_t sensor = 0x00;

uint16_t timer = 0;
int main(void) {
    own_init();

     Serial.begin(9600);
     Serial.println("Starting...");

    for(;;) {

		Serial.println(sensor);
		Serial.println(timer);
        sendZero();
		
		//_delay_ms(1000);

    }
}

void sendZero(){
	timer = 0;
	DDRB |= (1 << PINB3);
}

void sendOne(){
	
}

void own_init(){
	cli();
	
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1<< PCINT0);
	EICRA = (1 << ISC11) | (1 << ISC10);
	
	DDRB = (1 << PINB3) | (1 << PINB5);										//output pin for LED
	DDRB &= ~(1 << PINB0);													//input pin for IR sensor
	TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22) | (1 << CS20);
	OCR2A = 210;															//approximately every 26 microseconds
	TIMSK2 |= (1 << OCIE2A);												//enable timer compare match interupt
	sei();
} 

ISR(TIMER2_COMPA_vect) {
	timer++;
}
ISR(PCINT0_vect){
	sensor++;
}


// void sendZero(){								//sends a zero by turning of the LED for approximately 60 microseconds
//     DDRB |= (1 << PINB3);
//     timer = 0;
// 	while(timer < 3){
// 	}
//     while(timer > 3){
//         DDRB |= (1 << PINB3);
// 		//timer = 0;
//         Serial.println("You sent a zero");
// // 		while(timer < 1){
// // 		}
//     }
// }
	
// void sendOne(){									//sends a one by turning of the LED for approximately 120 microseconds
// 	 DDRB ^= (1 << PINB3);
// 	 timer = 0;
// 	 while(timer > 6){
// 		 DDRB ^= (1 << PINB3);
// 		 timer = 0;
// // 		 /*Serial.println("You sent a one");*/
// 		 while(timer < 1){
// 		 }
// 	 }
// }
