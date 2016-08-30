/*
 * Nicolas_Seillier.c
 *
 * Created: 30.08.2016 11:43:56
 *  Author: marjolas
 */ 


#include <stdio.h>
#include <avr/io.h>
#include "UART.h"

#define F_CPU 4915200UL
#define FOSC 4915200
#include <avr/delay.h>
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1

int main(void)
{
	/* Enable port A */
	//DDRA = 0xFF;
	
	/* Initialize UART */
	UART_Init(MYUBBR);
	
    while(1)
    {
        /* Set pins of port A */
		//PORTA = 0xAA;
		
		/* Try to communicate via UART */
		//unsigned char sent = 'A';
		//UART_Transmit(sent);
		//unsigned char received = UART_Receive();
		printf("Test\n");
		_delay_ms(1000);
    }
}