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

/*ISR(INT2_vect)
{
	printf("SPI\n");
}*/

void ISR_joystick_click()
{
	//printf("Interrupted by joystick button!\n");
}

void ISR_slide_right()
{
	//printf("Interrupted by right slide!\n");
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
}

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
	CAN_init();
	
	/* Draw something cool */
	draw_home();
	
	/* Send and receive a CAN message */
	MSG test_message;
	test_message.ID = 163;
	test_message.length = 1; 
	test_message.data[0] = 153;
	CAN_send(&test_message);
	_delay_ms(1000);
	MSG received_test = CAN_receive();
	printf("ID: %d, length: %d, data: %d\n", received_test.ID, received_test.length, received_test.data[0]);
	
	/* Enable interrupts */
	GICR |= (1 << INT0) | (1 << INT1); //| (1 << INT2);
	SREG |= (1 << 7);
	MCUCR |= (1<< ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	EMCUCR &= (0 << ISC2);
	sei();
	
	/* Variables initialization */
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
		 
		slide_right = ADC_Convert(SLIDE_RIGHT);
		if(slide_right != previous_slide_right)
		{
			ISR_slide_right();
		}
		previous_slide_right = slide_right;
		
		if(JOYSTICK_BUTTON == 1)
		{
			ISR_joystick_click();
		}
		
		_delay_ms(800);
    }
}