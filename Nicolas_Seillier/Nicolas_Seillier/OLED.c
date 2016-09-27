#include <stdint.h>
#include "Font4x6.h"

/* Write to the command register */
void OLED_write_cmd(uint8_t command)
{
	volatile char *oled_cmd = (char *) 0x1000;
	oled_cmd[0] = command;
}

/* Write to the data register */
void OLED_write_data(uint8_t data)
{
	volatile char *oled_data = (char *) 0x1200;
	oled_data[0] = data;
}

/* Set the OLED screen brightness */
void OLED_set_brightness(uint8_t value)
{
	OLED_write_cmd(0x81);
	OLED_write_cmd(value);
}

/* Erase the RAM line by line */
void OLED_Reset()
{
	OLED_write_cmd(0xb0);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb1);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb2);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb3);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb4);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb5);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb6);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
	OLED_write_cmd(0xb7);
	for(int i=0; i<128; i++){
		OLED_write_data(0b00000000);
	}
}

/* Init the OLED display */
void OLED_Init()
{
	OLED_write_cmd(0xae);        //  display  off
	OLED_write_cmd(0xa1);        //segment  remap
	OLED_write_cmd(0xda);        //common  pads  hardware:  alternative
	OLED_write_cmd(0x12);
	OLED_write_cmd(0xc8);        //common output scan direction:com63~com0
	OLED_write_cmd(0xa8);        //multiplex  ration  mode:63
	OLED_write_cmd(0x3f);
	OLED_write_cmd(0xd5);        //display divide ratio/osc. freq. mode
	OLED_write_cmd(0x80);
	OLED_write_cmd(0x81);        //contrast  control
	OLED_write_cmd(0x50);
	OLED_write_cmd(0xd9);        //set  pre-charge  period
	OLED_write_cmd(0x21);
	OLED_write_cmd(0x20);        //Set  Memory  Addressing  Mode
	OLED_write_cmd(0x02);
	OLED_write_cmd(0xdb);        //VCOM  deselect  level  mode
	OLED_write_cmd(0x30);
	OLED_write_cmd(0xad);        //master  configuration
	OLED_write_cmd(0x00);
	OLED_write_cmd(0xa4);        //out  follows  RAM  content
	OLED_write_cmd(0xa6);        //set  normal  display
	OLED_write_cmd(0xaf);        //  display  on
}

/* Set the cursor to the given position: page is from 0 to 7, segment from 0 to 127 */
void OLED_pos(uint8_t page, uint8_t segment)
{
	OLED_write_cmd(176 + page);
	OLED_write_cmd(segment & 0x0f);
	OLED_write_cmd(((segment & 0x70)>>4)|0x10);
}

/* Print a character from the table */
void OLED_print(char* a)
{
	int index = (int)a - 32;
	for(int i=0; i<4; i++)
	{
		OLED_write_data(font[index][i]);
	}
}

/* Print a string. If the string exceeds the screen size, stop printing. */
void OLED_print_string(char* string)
{
	int i=0;
	while(string[i]!='\0' && i < 32)
	{
		OLED_print(string[i]);
		i++;
	}
}

void OLED_print_arrow(uint8_t row, uint8_t col)
{
	OLED_pos(0, 0);
	OLED_write_data(0b00001000);
	OLED_write_data(0b00001000);
	OLED_write_data(0b00001000);
	OLED_write_data(0b00001000);
	OLED_write_data(0b00111110);
	OLED_write_data(0b00011100);
	OLED_write_data(0b00001000);
}

void OLED_print_home(uint8_t row, uint8_t col)
{
	OLED_pos(5, 64);
	OLED_write_data(0b00000000);
	OLED_write_data(0b11111000);
	OLED_write_data(0b11111100);
	OLED_write_data(0b11110110);
	OLED_write_data(0b00111111);
	OLED_write_data(0b11110110);
	OLED_write_data(0b11111111);
	OLED_write_data(0b11111000);
	OLED_write_data(0b00000000);
}

/*void OLED_print_bus(uint8_t row, uint8_t col)
{
	OLED_pos(3, 126);
	OLED_write_data(0b00000000);
	OLED_write_data(0b01111111);
	OLED_write_data(0b11110011);
	OLED_write_data(0b01111111);
	OLED_write_data(0b01110011);
	OLED_write_data(0b01111111);
	OLED_write_data(0b11110011);
	OLED_write_data(0b01111111);
	OLED_write_data(0b11100001);
	OLED_write_data(0b00000000);
}*/