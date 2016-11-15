#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "MCP251DEFS.h"

void MCP2515_reset()
{
	SPI_on();
	SPI_write(MCP_RESET);
	SPI_off();
}

char MCP2515_read(uint8_t address)
{
	SPI_on(); 
	SPI_write(MCP_READ);
	SPI_write(address);
	char data = SPI_read();
	SPI_off();
	return data;
}

void MCP2515_write(uint8_t address, uint8_t data)
{
	SPI_on();
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	SPI_off();
	
}

void MCP2515_request_to_send()
{
	SPI_on();
	SPI_write(MCP_RTS_TX0);
	SPI_off();
}

char MCP2515_read_status()
{
	SPI_on();
	SPI_write(MCP_READ_STATUS);
	char data = SPI_read();
	SPI_off();
	return data;
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_on();
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask);
	SPI_write(data);
	SPI_off();	
}