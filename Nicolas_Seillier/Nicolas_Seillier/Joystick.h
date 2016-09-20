#ifndef JOYSTICK_H_
#define JOYSTICK_H_


typedef struct position
{
	float x;
	float y;
} Position;

Position Joystick_Position(void);

char* Joystick_Direction(Position* joystick);


#endif