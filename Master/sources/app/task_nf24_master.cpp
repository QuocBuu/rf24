#include "task_nf24_master.h"

uint8_t client_feedback = CLIENT_NF24_1;

void task_nf24_master(ak_msg_t* msg) {
    switch (msg->sig)
    {
    case AC_NF24_MASTER_INIT_NETWORK: {
        APP_DBG_SIG("AC_NF24_MASTER_INIT_NETWORK\n");
        nrf_set_static_nwk_addr(0);
        FSM(&nrf_nwk_fsm, nrf_nwk_fsm_init);
        task_post_pure_msg(AC_RF24_NWK_ID, AC_RF24_NWK_INIT);
    }
        break;

    case AC_NF24_MASTER_PURE_SEND_ONE: {
        APP_DBG_SIG("AC_NF24_MASTER_PURE_SEND_ONE\n");
        /* init msg */
        ak_msg_t* m_msg = get_pure_msg();
        /* setup msg */
        set_if_des_id(m_msg, msg->if_des_id);    /* set client rev */
        set_if_src_task_id(m_msg, AC_TASK_IF_ID);
        set_if_des_task_id(m_msg, AC_TASK_NF24_CLIENT_ID);
        set_if_sig(m_msg, AC_NF24_CLIENT_PURE_RECV);
        /* send msg */
        set_msg_sig(m_msg, AC_IF_PURE_MSG_OUT);
        task_post(AC_TASK_IF_ID, m_msg);
    }
        break;

    case AC_NF24_MASTER_PURE_SEND_ALL: {
        APP_DBG_SIG("AC_NF24_MASTER_PURE_SEND_ALL\n");
        /* init msg */
        ak_msg_t* m_msg = get_pure_msg();
        /* setup msg */
        set_if_des_id(m_msg, CLIENT_NF24_ALL);    /* set client rev */
        set_if_src_task_id(m_msg, AC_TASK_IF_ID);
        set_if_des_task_id(m_msg, AC_TASK_NF24_CLIENT_ID);
        set_if_sig(m_msg, AC_NF24_CLIENT_PURE_RECV);
        /* send msg */
        set_msg_sig(m_msg, AC_IF_PURE_MSG_OUT);
        task_post(AC_TASK_IF_ID, m_msg);
    }
        break;

    case AC_NF24_MASTER_COMMON_SEND_ONE: {
        APP_DBG_SIG("AC_NF24_MASTER_COMMON_SEND_ONE\n");

    }
        break;

    case AC_NF24_MASTER_COMMON_SEND_ALL: {
        APP_DBG_SIG("AC_NF24_MASTER_COMMON_SEND_ALL\n");

    }
        break;

    case AC_NF24_MASTER_REQUEST_FEEDBACK: {
        APP_DBG_SIG("AC_NF24_MASTER_REQUEST_FEEDBACK\n");
        /* init msg */
        ak_msg_t* m_msg = get_pure_msg();
        /* setup msg */
        set_if_des_id(m_msg, client_feedback);    /* set client rev */
        set_if_src_task_id(m_msg, AC_TASK_IF_ID);
        set_if_des_task_id(m_msg, AC_TASK_NF24_CLIENT_ID);
        set_if_sig(m_msg, AC_NF24_CLIENT_PURE_RECV);
        /* send msg */
        set_msg_sig(m_msg, AC_IF_PURE_MSG_OUT);
        task_post(AC_TASK_IF_ID, m_msg);

        /* timer feedback */
        timer_set(AC_TASK_NF24_MASTER_ID, AC_NF24_MASTER_TIMER_FEEDBACK, SCR_NF24_MASTER_FEEDBACK_INTERVAL, TIMER_ONE_SHOT);
    }
        break;

    case AC_NF24_MASTER_TIMER_FEEDBACK: {
        APP_DBG_SIG("AC_NF24_MASTER_TIMER_FEEDBACK\n");
        client_feedback = 0;
    }
        break;

    case AC_NF24_MASTER_PURE_RECV: {
        APP_DBG_SIG("AC_NF24_MASTER_PURE_RECV\n");

    }
        break;

    case AC_NF24_MASTER_COMMON_RECV: {
        APP_DBG_SIG("AC_IF_PURE_MSG_IN\n");

    }
        break;

    case AC_NF24_MASTER_INTERRUPT_SEND: {
        APP_DBG_SIG("AC_IF_PURE_MSG_IN\n");

    }
        break;

    case AC_NF24_MASTER_CONTROL_LED_SEND: {
        APP_DBG_SIG("AC_NF24_MASTER_CONTROL_LED_SEND\n");

    }
        break;

    case AC_NF24_MASTER_BUTTON_STATUS_RECV: {
        APP_DBG_SIG("AC_NF24_MASTER_BUTTON_STATUS_RECV\n");
        uint8_t button_recv[3];
        memcpy(&button_recv, get_data_common_msg(msg), sizeof(button_recv));
        xprintf("\nStatus Button Client %d:\n", msg->if_des_id);
        xprintf("\n\tButton Mode: %d", button_recv[0]);
        xprintf("\n\tButton Up  : %d", button_recv[1]);
        xprintf("\n\tButton Down: %d", button_recv[2]);
        xprintf("\n");
        timer_remove_attr(AC_TASK_NF24_MASTER_ID, AC_NF24_MASTER_TIMER_FEEDBACK);
    }
        break;

    default:
        break;
    }
}