#include "receive.h"

int main(void){
	IR_setup();
	while(1){
	}
}

uint8_t readValue(){
/*	Serial.println(measuredTime);*/

//measuredTime decides what kind of signal we just received
//this signal can be a start, stop, 1 or 0 bit
//only start processing measuredTime if we actually received a start bit

	if(measuredTime >= 155 && measuredTime <= 170){					//start bit
		startCollecting = 1;
		/*Serial.println("(");*/
		data = 0;
	}
	if(startCollecting > 0){
		if(measuredTime >= 110 && measuredTime < 155){				//'1' bit
			data <<= 1;												//shift in a new bit
			data |= 0x01;											//set the newly added bit to 1
			/*Serial.print("1");*/
			} else if(measuredTime < 110){							//'0' bit
			data <<= 1;												//shift in a new 0 bit
			/*Serial.print("0");*/
			} else if(measuredTime > 170 && measuredTime < 300){	//stop bit
			startCollecting = 0;									//wait for a new start signal, discard every other signal
			Serial.print(data);
			Serial.print("\n");										
			return data;											//received byte
		}
	}
}

void IR_setup(){
	cli();
	Serial.begin(9600);
	
	/*Pin change interrupt*/
	PCICR |= (1 << PCIE2);					//pin group for PORTD
	PCMSK2 |= (1 << PCINT19);				//PIND3 / digital pin 3
	EICRA = (1 << ISC11) | (1 << ISC00);	//create interrupt on any logical change

	/*Timer*/
	TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);	//listen for interrupts on compare match a
	TCCR2B |= (1 << WGM22) | (1 << CS20);					//no prescaler
	OCR2A = 40; // 210/40									//210 corresponds to 13 microseconds
	TIMSK2 |= (1 << OCIE2A);								
	sei();
}

//This interrupt triggers if there is any change coming from the sensor
ISR(PCINT2_vect){
	if(PIND & (1 << PIND3)){								//check what state the sensor is in, rising or falling edge
		measuredTime = timer;								//rising edge means we have a new bit incoming, so we timestamp the value our timer is on
		} else {											//falling edge means the bit is completed, so we can look at our current time
		if(measuredTime > timer){							//if measuredTime is bigger the timer has reached it's maximum and overflowed
			measuredTime = 0xFFFF - measuredTime + timer;	//our measuredTime is off by 0xFFFF or 65535
			} else {
			measuredTime = timer - measuredTime;			//elapsed time
		}
		readValue();
	}
}
ISR(TIMER2_COMPA_vect){
	timer++;
}

uint8_t readValueOld(){
	/*Deze werkt*/
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
			data = 0;
			return data;
		}
	}
}