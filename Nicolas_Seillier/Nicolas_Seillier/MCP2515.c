#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "MCP251DEFS.h"

void MCP2515_RESET ()
{
	SPI_on();
	SPI_write(0xC0);
	SPI_off();
}

char MCP2515_READ(uint8_t address)
{
	SPI_on(); 
	SPI_write(0x03);
	SPI_write(address);
	char data = SPI_read();
	SPI_off();
	return data;
}

void MCP2515_WRITE(uint8_t address, uint8_t data)
{
	SPI_on();
	SPI_write(0x02);
	SPI_write(address);
	SPI_write(data);
	SPI_off();
	
}

void MCP2515_REQUEST_TO_SEND(int TXB0, int TXB1, int TXB2)
{
	SPI_on();
	uint8_t RTS = 0x80 | TXB0 | (TXB1<<1) | (TXB2<<2);
	SPI_write(RTS);
	SPI_off();
}

char MCP2515_READ_STATUS()
{
	SPI_on();
	SPI_write(0xA0);
	char data = SPI_read();
	SPI_off();
	return data;
}

void MCP2515_BIT_MODIFY(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_on();
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask);
	SPI_write(data);
	SPI_off();	
}

void MCP2515_INIT()
{
	SPI_Init();
	
	char result = MCP2515_READ(0x0E);
	printf("MCP_CANSTAT avant reset= %c \n",result);
	
	MCP2515_RESET();
	
	result = MCP2515_READ(0x0E);
	printf("MCP_CANSTAT= %c \n",result);
}