#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

int number = 0;
uint8_t sensor = 0x00;
int main(void){
	own_init();
	Serial.println("Starting...");
	while(1){
		Serial.print("");
		if(sensor > 0){
			number++;
			Serial.print(number);
			Serial.println("  SIGNAAAAl");	
		}
	}	
}



void own_init(){
	sei();
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1<< PCINT0);
	EICRA = (1 << ISC11) | (1 << ISC10);
	Serial.begin(9600);
}
ISR(PCINT0_vect){
	sensor = PINB & (1<<PINB0);
}