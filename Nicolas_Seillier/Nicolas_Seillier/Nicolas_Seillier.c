/*
 * Nicolas_Seillier.c
 *
 * Created: 30.08.2016 11:43:56
 *  Author: marjolas
 */ 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "SRAM.h"
#include "ADC.h"
#include "Joystick.h"
#include "OLED.h"
#include "Framework.h"
#include "SPI.h"
#include "MCP2515.h"
#include "MCP251DEFS.h"
#include "CAN.h"

#define F_CPU 4915200UL
#define FOSC 4915200
#define BAUD 9600
#define UART_BAUD FOSC/16/BAUD-1

#define SLIDE_LEFT 0x6
#define SLIDE_RIGHT 0x7
#define TOUCH_LEFT INT1
#define TOUCH_RIGHT INT0
#define JOYSTICK_BUTTON ~PINB & 0x1

int mode = HOME;
int arrow = 2 ;
int i = 0;
uint8_t slide_left;

ISR(INT0_vect)
{
	if(i < 3)
	{
		i++;
	}
	else
	{
		if(mode == HOME)
		{
			mode = MAIN_MENU;
			arrow = 2;
			draw_main_menu();
		}
		else if(mode == MAIN_MENU && arrow == 2)
		{
			mode = ONE_PLAYER;
			draw_one_player();
		}
		else if(mode == MAIN_MENU && arrow == 3)
		{
			mode = TWO_PLAYERS;
			draw_two_players();
		}
		else if(mode == MAIN_MENU && arrow == 4)
		{
			mode = HIGHSCORES;
			draw_highscores();
		}
		else if(mode == MAIN_MENU && arrow == 5)
		{
			mode = SETTINGS;
			arrow = 2;
			draw_settings();
		}
		else if((mode == ONE_PLAYER) || (mode == TWO_PLAYERS))
		{
			MSG reset_message;
			reset_message.ID = 3;
			reset_message.length = 0;
			reset_message.data[0] = 0;
			CAN_send(&reset_message);
			OLED_pos(5, 20);
			OLED_print_string("          ");
		}
		else if(mode == SETTINGS && arrow == 2)
		{
			mode = BRIGHTNESS;
			draw_brightness(slide_left);
		}
	}
}

ISR(INT1_vect)
{
	if(mode == MAIN_MENU)
	{
		mode = HOME;
		draw_home();
	}
	else if(mode == ONE_PLAYER || mode == TWO_PLAYERS || mode == HIGHSCORES || mode == SETTINGS)
	{
		mode = MAIN_MENU;
		arrow = 2;
		draw_main_menu();
	}
	else if(mode == BRIGHTNESS)
	{
		mode = SETTINGS;
		arrow = 2;
		draw_settings();
	}
}

void ISR_slide_left()
{
	if(mode == BRIGHTNESS)
	{
		OLED_set_brightness(slide_left);
		OLED_pos(3, 32);
		for(int i=0; i < 64; i++)
		{
			OLED_write_data(0b00000000);
		}
		OLED_pos(3, 32);
		for(int i=0; i < slide_left/4; i++)
		{
			OLED_write_data(0b11111111);
		}
	}
}

void ISR_joystick(Position joystick)
{
	char* direction = Joystick_Direction(&joystick);
	if(mode == MAIN_MENU && direction == "UP"){
		if(arrow > 2)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow--;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
		else if(arrow == 2)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow = 5;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
	}
	else if(mode == MAIN_MENU && direction == "DOWN"){
		if(arrow < 5)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow++;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
		else if(arrow == 5)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow = 2;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
	}
	else if(mode == SETTINGS && direction == "UP"){
		if(arrow == 3)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow--;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
		else if(arrow == 2)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow = 3;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
	}
	else if(mode == SETTINGS && direction == "DOWN"){
		if(arrow == 2)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow++;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
		else if(arrow == 3)
		{
			OLED_pos(arrow, 8);
			OLED_erase_arrow();
			arrow = 2;
			OLED_pos(arrow, 8);
			OLED_print_arrow();
		}
	}
	else if((mode == ONE_PLAYER) || (mode == TWO_PLAYERS)){
		MSG joystick_message;
		MSG score_message;
		joystick_message.ID = 1;
		joystick_message.length = 2;
		joystick_message.data[0] = joystick.x;
		joystick_message.data[1] = joystick.y;
		CAN_send(&joystick_message);
		score_message = CAN_receive();
		OLED_pos(5, 20);
		char score_string[20];
		sprintf(score_string, "Score: %d", score_message.data[0]);
		OLED_print_string(score_string);
	}
}

int main(void)
{
	/* Initialize UART */
 	UART_init(UART_BAUD);
	printf("Reset\n");
	_delay_ms(1000);
	
	/* Initialize SRAM, ADC, OLED and CAN */
	SRAM_init();
	OLED_reset();
	OLED_init();
	CAN_init();
	
	/* Draw something cool */
	draw_home();
	
	/* Enable interrupts */
	GICR |= (1 << INT0) | (1 << INT1);
	SREG |= (1 << 7);
	MCUCR |= (1<< ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	EMCUCR &= (0 << ISC2);
	sei();
	
	/* Initialize variables */
	Position joystick = Joystick_Position();
	Position joystick_previous = joystick;
	slide_left = ADC_Convert(SLIDE_LEFT);
	uint8_t previous_slide_left = slide_left;
	uint8_t slide_right = ADC_Convert(SLIDE_RIGHT);
	uint8_t previous_slide_right = slide_right;
	
    while(1)
    {
		joystick = Joystick_Position();
		if((joystick.x != joystick_previous.x)||(joystick.y != joystick_previous.y))
		{
			ISR_joystick(joystick);
		}
		joystick_previous = joystick;
		
		slide_left = ADC_Convert(SLIDE_LEFT);
		if(slide_left != previous_slide_left)
		{
			ISR_slide_left();
		}
		previous_slide_left = slide_left;
		 
		_delay_ms(800);
    }
}