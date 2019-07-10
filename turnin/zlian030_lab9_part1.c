/*	Author: zlian030
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
//#ifdef _SIMULATE_
//#include "simAVRHeader.h"
//#endif

enum State{start, sounds, off} state;
unsigned char button1;
unsigned char button2;
unsigned char button3;



void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A =	0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0;
		current_frequency = frequency;		
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void tick() {
	button1 = ~PINA & 0x01;
	button2 = ~PINA & 0x02;
	button3 = ~PINA & 0x04;
	switch(state) {
		case start:
			state = off;
			break;
		case sounds:
			if ((button1 && !button2 && !button3) || (!button1 && button2 && !button3) || (!button1 && !button2 && button3)) {
				state = sounds;
			}
			else {
				state = off;
			}
			break;
			
		case off:
			if ((button1 && !button2 && !button3) || (!button1 && button2 && !button3) || (!button1 && !button2 && button3)) {
				state = sounds;
			}
			else {
				state = off;
			}
			break;
			
		default:
			break;
	}
	switch(state) {
		case start:
			state = off;
			break;
		case sounds:
			if (button1 && !button2 && !button3) { set_PWM(2616.3); }
			if (!button1 && button2 && !button3) { set_PWM(2936.6); }
			if (!button1 && !button2 && button3) { set_PWM(3296.3); }
			break;
			
		case off:
			set_PWM(0);
			break;
			
		default:
			break;
			
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
    /* Insert your solution below */
    while (1) {
		
		tick();
    }
    return 1;
}
