#ifndef UART_H_
#define UART_H_


void UART_transmit(unsigned char data);

unsigned char UART_receive(void);

void UART_init(unsigned int ubrr);


#endif