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
	
	/* Initialize CAN */
	CAN_init();
	
	/* Send and receive a CAN message */
	MSG test_message;
	test_message.ID = 173;
	test_message.length = 1;
	test_message.data[0] = 153;
	CAN_send(&test_message);
	_delay_ms(1000);
	MSG received_test = CAN_receive();
	printf("ID: %d, length: %d, data: %d\n", received_test.ID, received_test.length, received_test.data[0]);
	
	while(1);	
}