#include <stdlib.h>
#include <stdio.h>
#define F_CPU 4915200U
#include <avr/delay.h>
#include "SPI.h"
#include "MCP2515.h"
#include "MCP251DEFS.h"
#include "CAN.h"

void CAN_init()
{
	SPI_Init();
	MCP2515_RESET();
	_delay_ms(100);
	
	/* no filter */
	//MCP2515_BIT_MODIFY(MCP_RXB0CTRL, 0b01100100, 0b11111111);
	//MCP2515_BIT_MODIFY(MCP_RXB1CTRL, 0b01100100, 0b11111111);
	volatile char state = MCP2515_READ(MCP_CANSTAT);
	while((state & MODE_MASK) != MODE_CONFIG)
	{
		printf("Error in CAN communication\n");
		printf("Mode is %c\n", state);
		state = MCP2515_READ(MCP_CANSTAT);
	}
	
	/*loop back mode*/
	MCP2515_BIT_MODIFY(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
	
	/*enable interrupts*/
	MCP2515_BIT_MODIFY(MCP_CANINTE, 0b00000001, 0b00000001);
	
	state = MCP2515_READ(MCP_CANSTAT);
	while((state & MODE_MASK) != MODE_LOOPBACK)
	{
		printf("Error in CAN communication\n");
		char state = MCP2515_READ(MCP_CANSTAT);
		printf("Mode is %c\n", state);
	}
	
}

void CAN_send(MSG* message)
{	
	/*write ID*/
	int ID_l = (message->ID & 0x07) << 5;
	int ID_h = (message->ID & 0xFFF8) >> 3;
	MCP2515_WRITE(MCP_TXB0SIDH,ID_h);
	MCP2515_BIT_MODIFY(MCP_TXB0SIDL,0xE0,ID_l);
	//MCP2515_WRITE(MCP_TXB0SIDH,message->ID >> 8);
	//MCP2515_WRITE(MCP_TXB0SIDL,message->ID & 0xE0);
	/*write data length*/
	int length = message->length & 0x0F;
	MCP2515_BIT_MODIFY(MCP_TXB0DLC,0x0F,length);
	
	/*write the message in the chosen buffer*/
	for (int i=0; i<message->length; i++)
	{
		MCP2515_WRITE(MCP_TXB0D0+i,message->data[i]);
	}
	
	/*buffer "choice"*/
	//MCP2515_BIT_MODIFY((buffer+3)*16,0b00001000, 0b00001000);
		
	/*configuration mode*/
	//MCP2515_BIT_MODIFY(MCP_CANCTRL,0b11100000,0b10000000);
		
	/*set BnRTS low*/
	//MCP2515_BIT_MODIFY(0x0D,0b00000111,0b00000111);
		
	/*loop back mode*/
	//MCP2515_BIT_MODIFY(MCP_CANCTRL,0b11100000,0b01000000);
	
	MCP2515_REQUEST_TO_SEND();
}

MSG CAN_receive()
{
	if((MCP2515_READ(MCP_CANINTF) | 0x01) == 0x01)
	{
		printf("Buffer 0\n");
		//int ID_h = MCP2515_READ(MCP_RXB0SIDH);
	
		//int ID_l = MCP2515_READ(MCP_RXB0SIDL);
	
		int length = MCP2515_READ(MCP_RXB0DLC);
	
		MSG message;
		for (int i=0; i<length; i++)
		{
			message.data[i] = MCP2515_READ(MCP_RXB0D0+i);
		
		}
		
		MCP2515_BIT_MODIFY(MCP_CANINTF,0b00000001,0b00000000);
		
		//message.ID = ((ID_h << 3) + ((ID_l >> 5) & 0x7)) & 0xFF;
		message.ID = MCP2515_READ(MCP_RXB0SIDH)<<8 |  MCP2515_READ(MCP_RXB0SIDL);
		message.length = length & 0x0F;
		
		return message;
	}
	
	else{
		 printf("Nothing in buffer\n");
		 return;
	}
}