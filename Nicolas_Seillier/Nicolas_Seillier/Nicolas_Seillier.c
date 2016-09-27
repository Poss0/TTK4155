/*
 * Nicolas_Seillier.c
 *
 * Created: 30.08.2016 11:43:56
 *  Author: marjolas
 */ 


#include <stdio.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "UART.h"
#include "SRAM.h"
#include "ADC.h"
#include "Joystick.h"
#include "OLED.h"

#define F_CPU 4915200UL
#define FOSC 4915200
#define BAUD 9600
#define UART_BAUD FOSC/16/BAUD-1

#define SLIDE_LEFT 0x6
#define SLIDE_RIGHT 0x7
#define TOUCH_LEFT (PINB & 0x4)>>2
#define TOUCH_RIGHT (PINB & 0x2)>>1
#define JOYSTICK_BUTTON ~PINB & 0x1

int main(void)
{
	/* Initialize UART */
	UART_Init(UART_BAUD);
	printf("Reset\n");
	_delay_ms(1000);
	
	/* Initialize SRAM, ADC and OLED */
	SRAM_Init();
	OLED_Reset();
	OLED_Init();
	
	/* Enable port B: the three buttons are inputs */
	PORTB |= 0x07;
	
	/* Draw something cool */
	OLED_print_arrow(0x00, 0x00);
	OLED_print_home(0x00, 0x00);
	OLED_pos(6, 0);
	OLED_print_string("Welcome to the best ping pong game in the world!");
	OLED_pos(7, 0);
	OLED_print_string("Welcome!");
	OLED_pos(7, 50);
	OLED_print_string("Goodbye!");
	
    while(1)
    {
		Position joystick = Joystick_Position();
		char* direction = Joystick_Direction(&joystick);
		printf("Joystick is %s: x %3.f, y %3.f\n", direction, joystick.x, joystick.y);
		
		uint8_t slide_left = ADC_Convert(SLIDE_LEFT); 
		uint8_t slide_right = ADC_Convert(SLIDE_RIGHT);
		printf("Sliders are: left %d, right %d\n", slide_left, slide_right);
		
		printf("Buttons are: left %u, joystick %u, right %u\n", TOUCH_LEFT, JOYSTICK_BUTTON, TOUCH_RIGHT);
		
		_delay_ms(3000);
		
		OLED_set_brightness(slide_left);
    }
}