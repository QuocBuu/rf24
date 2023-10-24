#include "task_nf24_client.h"
#include "task_nf24_master.h"

uint8_t control_led;
void task_nf24_client(ak_msg_t* msg) {
    switch (msg->sig)
    {
    case AC_NF24_CLIENT_INIT_NETWORK: {
        APP_DBG_SIG("AC_NF24_CLIENT_INIT_NETWORK\n");
        nrf_set_static_nwk_addr(if_TYPE_NF24_CLIENT);
        FSM(&nrf_nwk_fsm, nrf_nwk_fsm_init);
        task_post_pure_msg(AC_RF24_NWK_ID, AC_RF24_NWK_INIT);
    }
        break;

    case AC_NF24_CLIENT_PURE_RECV: {
        APP_DBG_SIG("AC_NF24_CLIENT_PURE_RECV\n");
        if(msg->if_des_id == CLIENT_NF24_2 || msg->if_des_id == CLIENT_NF24_ALL) {
            xprintf("\n Master gui Dung id\n");
            xprintf("\n%d\n",msg->if_des_id);
        }
        else {
            xprintf("\n Master gui Sai id\n");
            xprintf("\n%d\n",msg->if_des_id);
        }
    }
        break;

    case AC_NF24_CLIENT_CONTROL_LED_RECV: {
        APP_DBG_SIG("AC_NF24_CLIENT_CONTROL_LED_RECV\n");
        if(control_led == 0) {
            led_B5_on();
            control_led = 1;
        }
        else {
            led_B5_off();
            control_led = 0;
        }
    }
        break;

    case AC_NF24_CLIENT_BUTTON_STATUS_SEND: {
        // data
        uint8_t button_send[3] = {
            io_button_mode_read(),
            io_button_up_read(),
            io_button_down_read()
        };
        ak_msg_t* g_msg = get_common_msg();

        set_if_des_id(g_msg, CLIENT_NF24_2);
        set_if_src_task_id(g_msg, AC_TASK_RF24_IF_ID);
        set_if_des_task_id(g_msg, AC_TASK_NF24_MASTER_ID);
        set_if_sig(g_msg, AC_NF24_MASTER_BUTTON_STATUS_RECV);
        set_if_data_common_msg(g_msg, (uint8_t*)&button_send, sizeof(button_send));

        set_msg_sig(g_msg, AC_IF_COMMON_MSG_OUT);
        task_post(AC_TASK_IF_ID, g_msg);
    }
        break;

    default:
        break;
    }
}