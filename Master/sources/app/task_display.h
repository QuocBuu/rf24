#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

#include "screen_manager.h"
#include "scr_nf24.h"

#define LCD_WIDTH			(124)
#define LCD_HEIGHT			(60)

#define NUMFLAKES			(10)
#define XPOS				(0)
#define YPOS				(1)
#define DELTAY				(2)
#define LOGO16_GLCD_HEIGHT	(16)
#define LOGO16_GLCD_WIDTH	(16)

extern scr_mng_t scr_mng_app;

extern void scr_nf24_handle(ak_msg_t* msg);

#endif // __TASK_DISPLAY_H__
