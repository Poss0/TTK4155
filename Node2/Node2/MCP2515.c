#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "MCP251DEFS.h"

void MCP2515_RESET()
{
	SPI_on();
	SPI_write(MCP_RESET);
	SPI_off();
}

char MCP2515_READ(uint8_t address)
{
	SPI_on(); 
	SPI_write(MCP_READ);
	SPI_write(address);
	char data = SPI_read();
	SPI_off();
	return data;
}

void MCP2515_WRITE(uint8_t address, uint8_t data)
{
	SPI_on();
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	SPI_off();
	
}

void MCP2515_REQUEST_TO_SEND()
{
	SPI_on();
	SPI_write(MCP_RTS_TX0);
	SPI_off();
}

char MCP2515_READ_STATUS()
{
	SPI_on();
	SPI_write(MCP_READ_STATUS);
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