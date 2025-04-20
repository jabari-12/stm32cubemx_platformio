#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f1xx_hal.h"
// A10是RS选择地址，自动右移一位，则需要如下两个地址的第11位分别是 0 和 1
#define LCD_CMD_ADDR 0x6c000000
#define LCD_DATA_ADDR 0x6c000800


#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE  0x001F
#define BRED  0xF81F
#define GRED  0xFFE0
#define GBLUE 0x07FF
#define RED   0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN  0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //棕色
#define BRRED 0XFC07 //棕红色
#define GRAY  0X8430 //灰色
#define DARKBLUE   0X01CF	//深蓝色

#define LIGHTBLUE  0X7D7C	//浅蓝色
#define GRAYBLUE   0X5458 //灰蓝色
#define LIGHTGREEN 0X841F //浅绿色
#define LGRAY 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 0XA651 //浅灰蓝色(中间层颜色
#define LBBLUE 0X2B12 //浅棕蓝色(选择条目的反色)

typedef struct lcd_params_st {
	uint16_t width;
	uint16_t height;
	uint16_t lcd_id;
	uint8_t dir;
	uint16_t wram_cmd;
	uint16_t set_x_cmd;
	uint16_t set_y_cmd;
} lcd_params_t;

int lcd_read_id(void);

int lcd_init(void);

void lcd_clear(uint16_t color);

void lcd_display_dir(uint8_t dir);

void lcd_set_cursor(uint16_t x_pos, uint16_t y_pos);

void lcd_draw_point(uint16_t x_pos, uint16_t y_pos, uint16_t color);


#endif
