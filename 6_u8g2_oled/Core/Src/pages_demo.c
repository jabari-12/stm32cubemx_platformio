#include "u8g2.h"
#include "oled.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"

// 定义菜单结构体
typedef struct MenuItem {
    const char *name;
    struct MenuItem *submenu;
    void (*action)();
} MenuItem;

// 示例菜单动作函数
void action1() {
    // 实现具体动作
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); // 关闭 LED
}

void action2() {
    // 实现具体动作
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); // 关闭 LED
}

// 定义子菜单
MenuItem submenu1[] = {
    {"child menu1-1", NULL, action1},
    {"child menu1-2", NULL, action2},
    {NULL, NULL, NULL}
};

MenuItem submenu2[] = {
    {"child menu2-1", NULL, action1},
    {"child menu2-2", NULL, action2},
    {NULL, NULL, NULL}
};

// 定义主菜单
MenuItem mainMenu[] = {
    {"main menu1", submenu1, NULL},
    {"main menu2", submenu2, NULL},
    {NULL, NULL, NULL}
};

// 当前选中的菜单项指针
MenuItem *currentMenu = mainMenu;
int currentIndex = 0;

// 绘制菜单
void drawMenu(u8g2_t *u8g2) {
    u8g2_FirstPage(u8g2);
    do {
        u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr);
        int y = 10;
        for (int i = 0; currentMenu[i].name != NULL; i++) {
            if (i == currentIndex) {
                u8g2_DrawStr(u8g2, 0, y, "> ");
                u8g2_DrawStr(u8g2, 10, y, currentMenu[i].name);
            } else {
                u8g2_DrawStr(u8g2, 10, y, currentMenu[i].name);
            }
            y += 10;
        }
    } while (u8g2_NextPage(u8g2));
}

// 处理菜单事件
void handleMenuEvent(u8g2_t *u8g2, uint8_t event) {
    switch (event) {
        case U8X8_MSG_GPIO_MENU_UP:
            if (currentIndex > 0) {
                currentIndex--;
            }
            break;
        case U8X8_MSG_GPIO_MENU_DOWN:
            if (currentMenu[currentIndex + 1].name != NULL) {
                currentIndex++;
            }
            break;
        case U8X8_MSG_GPIO_MENU_SELECT:
            if (currentMenu[currentIndex].submenu != NULL) {
                currentMenu = currentMenu[currentIndex].submenu;
                currentIndex = 0;
            } else if (currentMenu[currentIndex].action != NULL) {
                currentMenu[currentIndex].action();
            }
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            currentMenu = mainMenu;
            currentIndex = 0;
            break;
    }
    drawMenu(u8g2);
}

// 主函数示例
void demo(u8g2_t *u8g2) {
    drawMenu(u8g2);
    while (1) {
        uint8_t event = u8x8_GetMenuEvent(u8g2_GetU8x8(u8g2));
        if (event != 0) {
            handleMenuEvent(u8g2, event);
        }
    }
}