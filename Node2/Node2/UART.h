#ifndef UART_H_
#define UART_H_


void UART_Transmit(unsigned char data);
unsigned char UART_Receive(void);
void UART_Init(unsigned int ubrr);


#endif