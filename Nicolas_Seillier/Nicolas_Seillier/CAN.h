#ifndef CAN_H_
#define CAN_H_

void CAN_init();
void CAN_send(int ID,int length, char *message);
char CAN_receive(int length, int buffer);

#endif /* CAN_H_ */