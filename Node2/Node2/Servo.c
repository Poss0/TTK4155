#include <avr/io.h>

#define FOSC 16000000
#define FPWM 50
#define N 8
#define TOP FOSC/FPWM/N - 1
#define BOTTOM_SERVO 0.0009
#define NEUTRAL_SERVO 0.0015
#define TOP_SERVO 0.0021
#define SLOPE_SERVO 0.000006

void Servo_init(){
	//Set OC1A as an output
	DDRB |= (1 << PB5);
	//Select fast PWM mode
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |=  (1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
	TCCR1A |= 1 << COM1A1;
	TCCR1A &= ~(1 << COM1A0);
	//Set top value
	ICR1 = TOP;
	//Set compare value1
	OCR1A = NEUTRAL_SERVO * (TOP + 1) * FPWM;
	//Set prescaler
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10);
	TCCR1B |= (1 << CS11);
}

void Servo_pwm(float x){
	float y = - SLOPE_SERVO * x + NEUTRAL_SERVO;
	
	if(y > TOP_SERVO){
		y = TOP_SERVO;
	}
	else if(y < BOTTOM_SERVO){
		y = BOTTOM_SERVO;
	}
	
	OCR1A = y * (TOP + 1) * FPWM;
}