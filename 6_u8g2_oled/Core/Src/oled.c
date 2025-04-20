#include "u8g2.h"
#include "oled.h"
#include "main.h"
#include "spi.h"
#include "stm32f1xx_hal.h"

u8g2_t u8g2; // OLED display context

uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            HAL_Delay(1); 
            break;
        case U8X8_MSG_GPIO_RESET:
            HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case U8X8_MSG_GPIO_MENU_UP:
            u8x8_SetGPIOResult(u8x8, HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin));
            break;
        case U8X8_MSG_GPIO_MENU_DOWN:
            u8x8_SetGPIOResult(u8x8, HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin));
            break;
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin));
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, !HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin));
            break;
        
    }
    return 1;
}

uint8_t u8x8_stm32_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 10000);
            break; 
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_SET_DC:
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
            break;
        default:
            return 0;
    }
    return 1;
}


void oled_init(void) {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, U8G2_R0, u8x8_stm32_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    //u8g2_InitInterface(&u8g2);
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
}

void oled_draw_test(void) {
    u8g2_FirstPage(&u8g2);
    do {
        u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr); // Set the font
        u8g2_DrawStr(&u8g2, 10, 10, "Hello, World!"); // Draw the string
        u8g2_DrawBox(&u8g2, 10, 20, 100, 50); // Draw a box
    } while (u8g2_NextPage(&u8g2));
}

