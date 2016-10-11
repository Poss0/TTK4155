#include "MCP2515.h"
#include "MCP251DEFS.h"

void CAN_init()
{
	MCP2515_RESET();
	
	volatile uint8_t canStat = MCP2515_READ(MCP_CANSTAT);
	
	/*loop back mode*/
/*
	char data = MCP2515_READ(MCP_CANSTAT);
	data &= 0x1F;
	data |= (1<<6);*/
	MCP2515_BIT_MODIFY(MCP_CANCTRL, 0b11100000, 0b01000000); //MCP2515_WRITE(0x0F,data);
	
	/*enable interrupts*/
/*
	data = MCP2515_READ(0x2B);
	data &= 0xE3;
	data |= 0x1C;
	MCP2515_WRITE(0x2B,data);*/
	MCP2515_BIT_MODIFY(MCP_CANINTE, 0b11, 0b11);
	
}

void CAN_send(int buffer, int ID,int length, char *message)
{
	/*initialization of the first address of the buffer*/
	uint8_t address = 16*(buffer+3)+6;
	
	/*write the message in the chosen buffer*/
	for (int i=0; i<length; i++)
	{
		MCP2515_WRITE(address,message[i]);
		
		if ((address & 0x0F) < 0x0D)
		{
			address++;
		}
		
		else
		{
			address &= 0xF0;
			address |= 0x06;
		}
	}
	
	/*buffer "choice"*/
	char data = MCP2515_READ((buffer+3)*16);
	data &= 0xF7;
	data |= 0x08;
	MCP2515_WRITE((buffer+3)*16,data);
	
	if (buffer ==0)	
	{
		MCP2515_REQUEST_TO_SEND(1,0,0);
	}
	else if (buffer==1)
	{
		MCP2515_REQUEST_TO_SEND(0,1,0);
	}
	else
	{
		MCP2515_REQUEST_TO_SEND(0,0,1);	
	}
	
	/*configuration mode*/
	data = MCP2515_READ(0x0F);
	data &= 0x1F;
	data |= (1<<7);
	MCP2515_WRITE(0x0F,data);
	
	/*set BnRTS low*/
	data = MCP2515_READ(0x0D);
	data &= ~(1<<(buffer+3));
	data |= (1<<(buffer+3));
	MCP2515_WRITE(0x0D,data);
	
	/*loop back mode*/
	data = MCP2515_READ(0x0F);
	data &= 0x1F;
	data |= (1<<6);
	MCP2515_WRITE(0x0F,data);
	
}

char CAN_receive(int length, int buffer)
{
	char message [length];
	uint8_t address = 16*(buffer+3)+6;
	
	for (int i=0; i<length; i++)
	{
		 message [i] = MCP2515_READ(address);
		
		if ((address & 0x0F) < 0x0D)
		{
			address++;
		}
		
		else
		{
			address &= 0xF0;
			address |= 0x06;
		}
		
		return message;
	}
}