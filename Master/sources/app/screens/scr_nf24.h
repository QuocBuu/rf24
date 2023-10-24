#ifndef __scr_nf24_H__
#define __scr_nf24_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "buzzer.h"

#include "nrf_data.h"

extern int recv_data;

extern void nf24_common_send(uint8_t add, task_id_t des_task_id, uint8_t sig, uint8_t *data, uint8_t len);

extern view_dynamic_t dyn_view_startup;

extern view_screen_t scr_nf24;
extern void scr_nf24_handle(ak_msg_t* msg);

#endif //__scr_nf24_H__
