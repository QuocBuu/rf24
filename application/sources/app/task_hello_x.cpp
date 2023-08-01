#include "task_hello_x.h"

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "app.h"
#include "task_list.h"
#include "xprintf.h"
#include "timer.h"

int couter_timer_1, couter_timer_2;

void task_hello_x(ak_msg_t* msg) {
    switch (msg->sig) {
    case AC_TASK_HELLO_X_PRINT: {
        // receive message
        uint8_t* inData = get_data_common_msg(msg);
        int x1 = *(int *)inData;
        xprintf("data message: %d\n", x1);
    }
        break;

    // case AC_TASK_HELLO_X_TIMER_1:
    //     xprintf("\nTask hello x timer 1\n");
    //     couter_timer_1++;
    //     xprintf("Couter Timer 1: %d\n", couter_timer_1);
    //     break;

    // case AC_TASK_HELLO_X_TIMER_2:
    //     xprintf("\nTask hello x timer 2\n");
    //     couter_timer_2++;
    //     xprintf("Couter Timer 2: %d\n", couter_timer_2);
    //     // Delete timer 1
    //     timer_remove_attr(AC_TASK_HELLO_X_ID, AC_TASK_HELLO_X_TIMER_1);
    //     break;    

    default:
        break;
    }
}

