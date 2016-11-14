#ifndef MCP2515_H_
#define MCP2515_H_
#include <stdint.h>


void MCP2515_reset();
char MCP2515_read(uint8_t address);
void MCP2515_write(uint8_t address, uint8_t data);
void MCP2515_request_to_send();
char MCP2515_read_status();
void MCP2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);


#endif /* MCP2515_H_ */