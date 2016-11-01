#include <stdlib.h>
#include <stdio.h>
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
	
	/*loop back mode*/
	//MCP2515_BIT_MODIFY(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
	
	/*normal mode*/
	MCP2515_BIT_MODIFY(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	printf("Stat = %d\n",MCP2515_READ(MCP_CANSTAT));
	
	/*enable interrupts*/
	MCP2515_BIT_MODIFY(MCP_CANINTE, 0b00000001, 0b00000001);
}

void CAN_send(MSG* message)
{	
	/*wait until the previous message is sent*/
	while(MCP2515_READ(MCP_TXB0CTRL) & 0x04 == 0x4)
	;
	
	/*write ID*/
	int ID_l = (message->ID & 0x07) << 5;
	MCP2515_BIT_MODIFY(MCP_TXB0SIDL,0xE0,ID_l);
	
	int ID_h = (message->ID & 0xFFF8) >> 3;
	MCP2515_WRITE(MCP_TXB0SIDH,ID_h);
	
	/*write data length*/
	int length = message->length;
	MCP2515_BIT_MODIFY(MCP_TXB0DLC,0x0F,length);
	
	/*write the message in the chosen buffer*/
	for (int i=0; i<message->length; i++)
	{
		MCP2515_WRITE(MCP_TXB0D0+i,message->data[i]);
	}
	
	MCP2515_REQUEST_TO_SEND();
}

MSG CAN_receive()
{
	/*check if there is a message*/
	if(MCP2515_READ(MCP_CANINTF) & 0x01 == 0x1){
		MSG message;
		
		int ID_l = MCP2515_READ(MCP_RXB0SIDL);
		int ID_h = MCP2515_READ(MCP_RXB0SIDH);
		message.ID = ((ID_h << 3) + ((ID_l >> 5) & 0x7)) & 0xFF;
		
		int length = MCP2515_READ(MCP_RXB0DLC) & 0x0F;
		message.length = length;
		
		for (int i=0; i < length || i < 8; i++)
		{
			message.data[i] = MCP2515_READ(MCP_RXB0D0+i);
		}
		
		MCP2515_BIT_MODIFY(MCP_CANINTF,0b00000001,0b00000000);
		
		return message;
	}
	else return;
}