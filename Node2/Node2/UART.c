#include <stdio.h>
#include <avr/io.h>

void UART_transmit(unsigned char data){
	/* Wait for empty transmit buffer */
	while(!(UCSR0A&(1<<UDRE0)))
		;
	/* Put data into buffer */
	UDR0 = data;
}

unsigned char UART_receive(void){
	/* Wait for data to be received */
	while(!(UCSR0A&(1<<RXC0)))
		;
	/* Get and return received data from buffer */
	return UDR0;
}

void UART_init(unsigned int ubrr){
	/* Set baud rate */
	UBRR0L = (unsigned char) ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Link printf to UART transmissions */	
	fdevopen(UART_transmit,UART_receive);
}
