#ifndef OLED_H_
#define OLED_H_


void OLED_write_cmd(uint8_t command);

void OLED_write_data(uint8_t data);

void OLED_set_brightness(uint8_t value);

void OLED_reset();

void OLED_init();

void OLED_pos(uint8_t page, uint8_t segment);

void OLED_print(char*);

void OLED_print_string(char*);

void OLED_print_arrow();

void OLED_erase_arrow();

void OLED_print_home();

void OLED_print_woman(uint8_t page, uint8_t segment);


#endif /* OLED_H_ */