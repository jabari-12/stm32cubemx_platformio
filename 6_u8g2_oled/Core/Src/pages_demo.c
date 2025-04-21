#include <stdlib.h>
#include <stdio.h>
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

// 定义新的全局结构体
typedef struct {
    MenuItem *currentMenu;
    MenuItem *parentMenu;
    int currentIndex;
} MenuContext;

void action1();
void action2();
void actionBack();

MenuItem submenu1[] = {
    {"..", NULL, actionBack},
    {"child menu1-1", NULL, action1},
    {"child menu1-2", NULL, action2},
    {NULL, NULL, NULL},
};

MenuItem submenu2[] = {
    {"..", NULL, actionBack}, 
    {"child menu2-1", NULL, action1},
    {"child menu2-2", NULL, action2},
    {NULL, NULL, NULL},
};

// 定义主菜单
MenuItem mainMenu[] = {
    {"main menu1", submenu1, NULL},
    {"main menu2", submenu2, NULL},
    {NULL, NULL, NULL},
};

// 初始化全局结构体
MenuContext menuContext = {
    .currentMenu = mainMenu,
    .parentMenu = NULL,
    .currentIndex = 0
};

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
// 定义返回上一级的动作函数
void actionBack() {
    if (menuContext.parentMenu != NULL) {
        menuContext.currentMenu = menuContext.parentMenu;
        menuContext.currentIndex = 0;
        // 父菜单返回后，其父菜单应置为 NULL，因为已回到上一级
        menuContext.parentMenu = NULL;
    }
}


// 绘制菜单
// 定义阻尼系数
#define DAMPING_FACTOR 0.5
// 定义基础延迟时间
#define BASE_DELAY 20

// 绘制带动画过渡的菜单
void drawAnimatedMenu(u8g2_t *u8g2) {
    u8g2_FirstPage(u8g2);
    do {
        u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr);
        int y = 10;
        for (int i = 0; menuContext.currentMenu[i].name != NULL; i++) {
            if (i == menuContext.currentIndex) {
                // 添加框选效果，增强为填充背景色
                u8g2_DrawBox(u8g2, 0, y - 8, u8g2_GetStrWidth(u8g2, menuContext.currentMenu[i].name) + 16, 10);
                u8g2_SetDrawColor(u8g2, 0); // 设置文字颜色为反色
                u8g2_DrawStr(u8g2, 10, y, menuContext.currentMenu[i].name);
                u8g2_SetDrawColor(u8g2, 1); // 恢复默认绘制颜色
            } else {
                u8g2_DrawStr(u8g2, 10, y, menuContext.currentMenu[i].name);
            }
            y += 10;
        }
    } while (u8g2_NextPage(u8g2));
}

// 修改处理菜单事件函数
void handleMenuEvent(u8g2_t *u8g2, uint8_t event) {
    int menuCount = 0;
    while (menuContext.currentMenu[menuCount].name != NULL) {
        menuCount++;
    }
    switch (event) {
        case U8X8_MSG_GPIO_MENU_UP:
            menuContext.currentIndex = (menuContext.currentIndex - 1 + menuCount) % menuCount;
            break;
        case U8X8_MSG_GPIO_MENU_DOWN:
            menuContext.currentIndex = (menuContext.currentIndex + 1) % menuCount;
            break;
        case U8X8_MSG_GPIO_MENU_SELECT:
            if (menuContext.currentMenu[menuContext.currentIndex].submenu != NULL) {
                menuContext.parentMenu = menuContext.currentMenu;
                menuContext.currentMenu = menuContext.currentMenu[menuContext.currentIndex].submenu;
                menuContext.currentIndex = 0;
            } else if (menuContext.currentMenu[menuContext.currentIndex].action != NULL) {
                menuContext.currentMenu[menuContext.currentIndex].action();
            }
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            menuContext.currentMenu = mainMenu;
            menuContext.parentMenu = NULL;
            menuContext.currentIndex = 0;
            break;
    }
    drawAnimatedMenu(u8g2);
}

// 修改主函数示例
void demo(u8g2_t *u8g2) {
    drawAnimatedMenu(u8g2);
    while (1) {
        uint8_t event = u8x8_GetMenuEvent(u8g2_GetU8x8(u8g2));
        if (event != 0) {
            handleMenuEvent(u8g2, event);
            printf("Event: %d\r\n", event);
            HAL_Delay(100); // 添加延迟以减少闪烁
        }
    }
}