#ifndef CAN_H_
#define CAN_H_


typedef struct msg
{
	int ID;
	int length;
	char data[8];
} MSG;


void CAN_init();
void CAN_send(MSG* message);
MSG CAN_receive();


#endif /* CAN_H_ */