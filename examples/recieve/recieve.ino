#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

void led1();
void led2();
void own_init();
uint8_t readValue();

uint8_t data = 0x00;
uint8_t startCollecting;
volatile uint16_t measuredTime;
volatile uint16_t timer1 = 0;
volatile uint8_t sensor = 0x00;
int main(void){
	IR_setup();
	led1();
	led2();
	led3();
	PORTB &= ~(1 << PINB1);
	while(1){
	}
}

uint8_t readValue(){
//  	Serial.print(measuredTime);
//  	Serial.print("\n");
	 
//			Deze werkt
// 	if(measuredTime >= 145 && measuredTime <= 200){
// 		startCollecting = 1;
// 	}
// 	if(startCollecting > 0){
// 		if(measuredTime > 110 && measuredTime < 145){
// 			data <<= 1;
// 			data |= 0x01;
// 			} else if(measuredTime <= 110){
// 			data <<= 1;
// 			} else if(measuredTime > 200 && measuredTime < 260){
// 			startCollecting = 0;
// 			Serial.print(data);
// 			Serial.print("\n");
// 			return data;
// 		}


	if(measuredTime >= 140 && measuredTime <= 160){
		startCollecting = 1;
	}
	if(startCollecting > 0){
		if(measuredTime >= 110 && measuredTime < 140){
			data <<= 1;
			data |= 0x01;
			} else if(measuredTime < 110){
			data <<= 1;
			} else if(measuredTime > 160 && measuredTime < 260){
			startCollecting = 0;
			Serial.print(data, HEX);
			Serial.print("\n");
			return data;
		}
	}
}

void led1(){
	PORTB |= (1 << PINB2);
	PORTB &= ~(1 << PINB1);
	PORTB &= ~(1 << PINB3);
	_delay_ms(100);
}

void led2(){
	PORTB |= (1 << PINB1);
	PORTB &= ~(1 << PINB2);
	PORTB &= ~(1 << PINB3);
	_delay_ms(100);
}

void led3(){
	PORTB |= (1 << PINB3);
	PORTB &= ~(1 << PINB2);
	PORTB &= ~(1 << PINB1);
	_delay_ms(100);
}

void IR_setup(){
	cli();
	Serial.begin(9600);
	DDRB = (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB3);
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1<< PCINT0);
	EICRA = (1 << ISC11) | (1 << ISC00);

	TCCR2A = (1 << COM2A0) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22) | (1 << CS20);
	OCR2A = 40;
	TIMSK2 |= (1 << OCIE2A);
	sei();
}
ISR(PCINT0_vect){
	sensor = PINB & (1<<PINB0);
	if(sensor > 0){
		/*		timer1 = 0;*/
		measuredTime = timer1;
		} else {
		/*		measuredTime = timer1;*/
		if(timer_start_counting > timer1){
			measuredTime = 65535 - measuredTime + timer1;
			// 			Serial.println("mT > timer");
			// 			Serial.println(timer_start_counting);
			// 			Serial.println(timer1);
			} else {
			measuredTime = timer1 - measuredTime;
		}
		readValue();
	}
}
ISR(TIMER2_COMPA_vect){
	timer1++;
}