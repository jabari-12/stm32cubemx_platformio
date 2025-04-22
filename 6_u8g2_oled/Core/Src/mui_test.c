#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "u8g2.h"
#include "mui.h"
#include "mui_u8g2.h"

#if 0
/*=================================================*/
/* global variables */

extern u8g2_t u8g2;
mui_t ui;

uint8_t var1 = '0';
uint8_t var2 = '0';
uint8_t var3 = '0';
uint8_t var4 = '0';

muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_5x8_tr),
  
  MUIF_VARIABLE("B1",&var1,mui_u8g2_u8_char_wm_mud_pi),  
  MUIF_VARIABLE("B2",&var2,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B3",&var3,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B4",&var4,mui_u8g2_u8_char_wm_mud_pi), 

  MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi)
  
};


fds_t fds[] MUI_PROGMEM  =
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5,10, "Password")
MUI_XY("B1", 5, 20)
MUI_XY("B2", 15, 20)
MUI_XY("B3", 25, 20)
MUI_XY("B4", 35, 20)
MUI_XYAT("G0", 64, 40, 10, "Goto Static Pages")

MUI_FORM(10)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 1")

MUI_FORM(11)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 2")

MUI_FORM(12)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 3")

;



int mui_test(void)
{
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 10, 0);
  
  for(;;)
  {
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      mui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );

    
    // printf("mui_GetCurrentCursorFocusPosition=%d\n", mui_GetCurrentCursorFocusPosition(&ui));
    
  }
  return 0;
}
#endif