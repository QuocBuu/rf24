#ifndef __TASK_NF24_MASTER_H__
#define __TASK_NF24_MASTER_H__

#include <stdbool.h>
#include <stdint.h>

#include "app.h"
#include "app_if.h"
#include "app_dbg.h"
#include "app_data.h"

#include "task_if.h"
#include "task_list.h"
#include "task_list_if.h"

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_dbg.h"
#include "sys_irq.h"
#include "sys_io.h"

#include "hal_nrf.h"
#include "nrf_data.h"

enum {
    MASTER_NF24 = 0,

    CLIENT_NF24_1,
    CLIENT_NF24_2,
    CLIENT_NF24_3,

    CLIENT_NF24_ALL = 32
};

#endif // __TASK_NF24_MASTER_H__
