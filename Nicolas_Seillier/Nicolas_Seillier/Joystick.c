#include <stdlib.h>
#include <stdint.h>
#include "ADC.h"
#include "Joystick.h"

#define JOYSTICK_VERTICAL 0x4
#define JOYSTICK_HORIZONTAL 0x5

Position Joystick_Position()
{
	uint8_t joystick_horizontal = ADC_Convert(JOYSTICK_HORIZONTAL);
	uint8_t joystick_vertical = ADC_Convert(JOYSTICK_VERTICAL);
	float X = joystick_horizontal;
	float Y = joystick_vertical;
	
	Position joystick;
	joystick.x = (X / 255.0) * 200.0 - 100.0;
	joystick.y = (Y / 255.0) * 200.0 - 100.0;
	return joystick;
}

char* Joystick_Direction(Position* joystick)
{
	if(joystick->x < -71)
	return "LEFT";
	else if(joystick->x > 71)
	return "RIGHT";
	
	if(joystick->y < -71)
	return "DOWN";
	else if(joystick->y > 71)
	return "UP";
	
	return "NEUTRAL";
}