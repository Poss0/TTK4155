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
	
	/* Initialize ADC */
	ADC_init();
	
	/* Initialize CAN */
	CAN_init();
	
	/* Initialize PWM */
	Servo_init();
	
	int score = 0;
	
	/* Send and receive a CAN message */
	/*MSG test_message;
	test_message.ID = 20;
	test_message.length = 1;
	test_message.data[0] = 20;*/
	MSG received_test;
	while(1){
		/*CAN_send(&test_message);
		printf("Sending: ID: %d, length: %d, data: %d\n", test_message.ID, test_message.length, test_message.data[0]);
		_delay_ms(1000);*/
		received_test = CAN_receive();
		//printf("Receiving: ID: %d, length: %d, x: %i, y:%i\n", received_test.ID, received_test.length, received_test.data[0], received_test.data[1]);
		Servo_pwm(received_test.data[0]);
		score = get_score(score);
		printf("Score: %d\n", score);
	}
}