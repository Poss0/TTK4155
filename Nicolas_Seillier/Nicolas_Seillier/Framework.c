#include <stdint.h>
#include "OLED.h"
#include "Framework.h"

void draw_home()
{
	OLED_reset();
	OLED_pos(1, 4);
	OLED_print_string("Welcome to the best ping pong");
	OLED_pos(3, 28);
	OLED_print_string("game in the world!");
	OLED_pos(7, 20);
	OLED_print_string("Touch right to start!");
}

void draw_main_menu()
{
	OLED_reset();
	OLED_pos(0, 46);
	OLED_print_string("Main menu");	
	OLED_pos(2, 8);
	OLED_print_arrow();
	OLED_pos(2, 20);
	OLED_print_string("One-player mode");
	OLED_pos(3, 20);
	OLED_print_string("Two-players mode");
	OLED_pos(4, 20);
	OLED_print_string("Highscores");
	OLED_pos(5, 20);
	OLED_print_string("Settings");
	OLED_pos(7, 48);
	OLED_print_string("Touch right to enter");
	OLED_pos(7,0);
	OLED_print_home();
}

void draw_one_player()
{
	OLED_reset();
	OLED_pos(0, 32);
	OLED_print_string("One-player mode");
	OLED_print_woman(3,58);
	OLED_pos(7,0);
	OLED_print_home();
}

void draw_two_players()
{
	OLED_reset();
	OLED_pos(0, 32);
	OLED_print_string("Two-players mode");
	OLED_print_woman(3,29);
	OLED_print_woman(3,87);
	OLED_pos(7,0);
	OLED_print_home();
}

void draw_highscores()
{
	OLED_reset();
	OLED_pos(0, 44);
	OLED_print_string("Highscores");
	OLED_pos(2, 20);
	OLED_print_string("1. 650 points");
	OLED_pos(3, 20);
	OLED_print_string("2. 422 points");
	OLED_pos(4, 20);
	OLED_print_string("3. 200 points");
	OLED_pos(7,0);
	OLED_print_home();
}

void draw_settings()
{
	OLED_reset();
	OLED_pos(0, 48);
	OLED_print_string("Settings");
	OLED_pos(2, 8);
	OLED_print_arrow();
	OLED_pos(2, 20);
	OLED_print_string("Brightness");
	OLED_pos(3, 20);
	OLED_print_string("Sound");
	OLED_pos(7,0);
	OLED_print_home();
}

void draw_brightness()
{
	OLED_reset();
	OLED_pos(0, 44);
	OLED_print_string("Brightness");
	OLED_pos(5, 0);
	OLED_print_string("Use left slide to set brightness");
	OLED_pos(7,0);
	OLED_print_home();
}