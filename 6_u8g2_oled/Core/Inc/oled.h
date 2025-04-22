#ifndef OLED_H
#define OLED_H
#include "u8g2.h"
void oled_init(void);
void oled_draw_test(void);
void demo(u8g2_t *u8g2);
int mui_test(void);
int mui_all(void);
#endif