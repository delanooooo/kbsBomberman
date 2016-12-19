#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void led1();
void led2();
void own_init();
uint8_t readValue();

char data = 0;
uint8_t startCollecting;
volatile uint16_t measuredTime;
volatile uint16_t timer1 = 0;
volatile uint8_t sensor = 0;
volatile uint8_t count = 0;
int main(void){
	IR_setup();
	while(1){
	}
}

uint8_t readValue(){
	
//			Deze werkt
	if(measuredTime >= 145 && measuredTime <= 200){
		startCollecting = 1;
	}
	if(startCollecting > 0){
		if(measuredTime > 110 && measuredTime < 145){
			data <<= 1;
			data |= 0x01;
			} else if(measuredTime <= 110){
			data <<= 1;
			} else if(measuredTime > 200 && measuredTime < 260){
			startCollecting = 0;
			Serial.print(data);
			Serial.print("\n");
			return data;
		}


// 	if(measuredTime >= 140 && measuredTime <= 160){
// 		startCollecting = 1;
// /*		Serial.println("Start");*/
// 	}
// 	if(startCollecting > 0){
// 		if(measuredTime >= 110 && measuredTime < 140){
// 			data <<= 1;
// 			data |= 0x01;
// /*			Serial.println("EEN");*/
// 			} else if(measuredTime < 110){
// 			data <<= 1;
// /*			Serial.println("0");*/
// 			} else if(measuredTime > 160 && measuredTime < 260){
// 			startCollecting = 0;
// // 			Serial.print(data, HEX);
// // 			Serial.print("\n");
// 			return data;
// 		}
 	}
}

void IR_setup(){
	cli();
	Serial.begin(9600);
	//Pins
	DDRB = (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB5);
/*	DDRD |= (1 << PORTD3);*/
	
	//Pin change interrupt
	PCICR |= (1 << PCIE2);
/*	PCMSK2 |= (1 << PCINT19);*/
	EIMSK |= (1 << INT1);
	EICRA |=  (1 << ISC10);

	//Timer
	TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22) | (1 << CS20);
	OCR2A = 40;
	TIMSK2 |= (1 << OCIE2A);
	sei();
}
ISR(INT1_vect){
	sensor = PINB & (1 << PINB0);
	Serial.println("interrupt");
	count++;
	Serial.println(count);
	if(sensor > 0){
		measuredTime = timer1;
		} else {
		if(measuredTime > timer1){
			measuredTime = 65535 - measuredTime + timer1;
			} else {
			measuredTime = timer1 - measuredTime;
		}
		readValue();
	}
}
ISR(TIMER2_COMPA_vect){
	timer1++;
}

// void led1(){
// 	PORTB |= (1 << PINB2);
// 	PORTB &= ~(1 << PINB1);
// 	PORTB &= ~(1 << PINB3);
// 	_delay_ms(100);
// }
// 
// void led2(){
// 	PORTB |= (1 << PINB1);
// 	PORTB &= ~(1 << PINB2);
// 	PORTB &= ~(1 << PINB3);
// 	_delay_ms(100);
// }
// 
// void led3(){
// 	PORTB |= (1 << PINB3);
// 	PORTB &= ~(1 << PINB2);
// 	PORTB &= ~(1 << PINB1);
// 	_delay_ms(100);
//  }
