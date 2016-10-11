#ifndef MCP2515_H_
#define MCP2515_H_
#include <stdint.h>


void MCP2515_RESET ();
char MCP2515_READ(uint8_t address);
void MCP2515_WRITE(uint8_t address, uint8_t data);
void MCP2515_REQUEST_TO_SEND(int TXB0, int TXB1, int TXB2);
char MCP2515_READ_STATUS();
void MCP2515_BIT_MODIFY(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_INIT();

#endif /* MCP2515_H_ */