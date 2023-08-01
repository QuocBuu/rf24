#include "task_hello.h"

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "app.h"
#include "task_list.h"
#include "xprintf.h"
#include "timer.h"

static int x = 0;

void task_hello(ak_msg_t* msg) {
    switch (msg->sig) {
    case AC_TASK_HELLO_PRINT: {
        x++;
        xprintf("\n\nHello World\n");
        xprintf("Counter button [up]: %d\n", x);
        // send message
        task_post_common_msg(AC_TASK_HELLO_X_ID, AC_TASK_HELLO_X_PRINT, (uint8_t *)&x, sizeof(x));
        // start timer 2
        timer_set(AC_TASK_HELLO_X_ID, AC_TASK_HELLO_X_TIMER_2, TASK_HELLO_TIMER_2_INTERVAL, TIMER_PERIODIC);
    }
        break;
    
    default:
        break;
    }
}
      
