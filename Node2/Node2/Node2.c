/*
 * Node2.c
 *
 * Created: 25.10.2016 15:40:20
 *  Author: paulinen
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "SPI.h"
#include "MCP2515.h"
#include "MCP251DEFS.h"
#include "CAN.h"
#include "Servo.h"
#include "ADC.h"
#include "Score.h"

#define F_CPU 16000000UL
#define FOSC 16000000
#define BAUD 9600
#define UART_BAUD FOSC/16/BAUD-1

int main(void)
{
	/* Initialize UART */
	UART_Init(UART_BAUD);
	printf("Reset\n");
	_delay_ms(1000);
	
	/* Initialize ADC, CAN, PWM */
	ADC_init();
	CAN_init();
	Servo_init();

	/* Initialize variables */
	MSG joystick_message;
	int score = 0;
	
	while(1){
		/* Read joystick position and calculate PWM to control servo */
		joystick_message = CAN_receive();
		Servo_pwm(joystick_message.data[0]);
		/* Update score */
		score = get_score(score);
	}
}