#include "scr_nf24.h"

static void view_scr_nf24();

int led_b5;
int recv_data;
char textToSend[32] = "Hello, Phan Quoc Buu!";
char textToRecv[32];

// uint8_t add_send;

void nf24_pure_send(uint8_t add, task_id_t des_task_id, uint8_t sig);
void nf24_common_send(uint8_t add, task_id_t des_task_id, uint8_t sig, uint8_t *data, uint8_t len);

view_dynamic_t dyn_view_nf24 = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_nf24
};

view_screen_t scr_nf24 = {
	&dyn_view_nf24,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_nf24() {
	/* NF24 Receive */
	view_render.clear();
	view_render.drawRect(0, 0, 128, 64, WHITE);
	view_render.drawFastHLine(0, 15, 128, WHITE);
	view_render.drawFastHLine(0, 15+12*1, 128, WHITE);
	view_render.drawFastHLine(0, 15+12*2, 128, WHITE);
	view_render.drawFastHLine(0, 15+12*3, 128, WHITE);
	view_render.drawFastHLine(0, 15+12*4, 128, WHITE);
	view_render.drawFastVLine(40, 15, 50, WHITE);
	view_render.drawFastVLine(95, 52, 12, WHITE);

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(20, 5);
	view_render.print("NF24 - Receive");

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	// Address Receive
	view_render.setCursor(3, 18);
	view_render.print("Add Me");
	if(1) {
		static char add_r[32];
		snprintf(add_r, \
				sizeof(add_r), \
				"%d.%d.%d.%d.%d", \
				mac_src_addr[0], \
				mac_src_addr[1], \
				mac_src_addr[2], \
				mac_src_addr[3], \
				mac_src_addr[4]);
		view_render.setCursor(45, 18);
		view_render.print(add_r);
		
	}
	else {
		view_render.setCursor(45, 18);
		view_render.print("...");
	}
	// Address Send
	view_render.setCursor(3, 30);
	view_render.print("Add S");
	if(1) {
		static char add_s[32];
		snprintf(add_s, \
				sizeof(add_s), \
				"%d.%d.%d.%d.%d", \
				mac_des_addr[0], \
				mac_des_addr[1], \
				mac_des_addr[2], \
				mac_des_addr[3], \
				mac_des_addr[4]);
		view_render.setCursor(45, 30);
		view_render.print(add_s);
	}
	else {
		view_render.setCursor(45, 30);
		view_render.print("...");
	}
	view_render.setCursor(3, 42);
	view_render.print("Data");
	if(1) {
		view_render.setCursor(45, 42);
		view_render.print(recv_data);
		view_render.print(textToRecv);
	}
	else {
		view_render.setCursor(45, 42);
		view_render.print("...");
	}

	view_render.setCursor(3, 54);
	view_render.print("Led B5");
	view_render.setCursor(105, 54);
	if(led_b5 == 1) {
		view_render.fillCircle(65, 57, 4, WHITE);
		view_render.print("ON");
	}
	else {
		view_render.drawCircle(65, 57, 4, WHITE);
		view_render.print("OFF");
	} 
}

void scr_nf24_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_on();

		SCREEN_TRAN(scr_chat_handle, &scr_chat);
		// timer_set(AC_TASK_DISPLAY_ID, SCR_NF24_COMMON_SEND, 5000, TIMER_ONE_SHOT);
		// timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_LOGO, AC_DISPLAY_STARTUP_INTERVAL, TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_BUTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
		if(led_b5 != 0) {
			led_B5_off();
			led_b5 = 0;
		}
		else {
			led_B5_on();
			led_b5 = 1;
		}
		// if (counter == 3) {
			// task_post_common_msg(AC_TASK_RF24_IF_ID, AC_RF24_IF_COMMON_MSG_OUT, (uint8_t*)&buu, sizeof(buu));
		// }
	}
		break;

	case AC_DISPLAY_BUTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_UP_RELEASED\n");
	}
		break;

	case AC_DISPLAY_BUTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_RELEASED\n");
	}
		break;

	case SCR_NF24_PURE_SEND: {
		APP_DBG_SIG("SCR_NF24_PURE_SEND\n");
	}
		break;

	case SCR_NF24_COMMON_SEND: {
		APP_DBG_SIG("SCR_NF24_COMMON_SEND\n");
		recv_data++;
		nf24_common_send(IF_TYPE_RF24_YOU, \
						AC_TASK_DISPLAY_ID, \
						SCR_NF24_COMMON_RECV, \
						(uint8_t*)&textToSend, \
						sizeof(textToSend));
	}
		break;

	case SCR_NF24_PURE_RECV: {
		APP_DBG_SIG("SCR_NF24_PURE_RECV\n");
	}
		break;

	case SCR_NF24_COMMON_RECV: {
		APP_DBG_SIG("SCR_NF24_COMMON_RECV\n");

		if(led_b5 != 0) {
			led_B5_off();
			led_b5 = 0;
		}
		else {
			led_B5_on();
			led_b5 = 1;
		}

		// uint8_t *inData = get_data_common_msg(msg);
		// recv_data = *(int *)inData;

		memcpy(&textToRecv, get_data_common_msg(msg), sizeof(textToRecv));
		
		// timer_set(AC_TASK_DISPLAY_ID, SCR_NF24_COMMON_SEND, 1000, TIMER_ONE_SHOT);
	}
		break;

	default:
		break;
	}
}

void nf24_pure_send(uint8_t add, task_id_t des_task_id, uint8_t sig) {
	// init message
	ak_msg_t* p_msg = get_pure_msg();
	// setup message
	set_if_src_task_id(p_msg, AC_TASK_RF24_IF_ID);
	set_if_des_task_id(p_msg, des_task_id);
	set_if_des_type(p_msg, add);
	set_if_sig(p_msg, sig);
	// send message
	set_msg_sig(p_msg, AC_IF_PURE_MSG_OUT);
	task_post(AC_TASK_IF_ID, p_msg);
}

void nf24_common_send(uint8_t add, task_id_t des_task_id, uint8_t sig, uint8_t *data, uint8_t len) {
	ak_msg_t* c_msg = get_common_msg();

	set_if_src_task_id(c_msg, AC_TASK_RF24_IF_ID);
	set_if_des_task_id(c_msg, des_task_id);
	set_if_des_type(c_msg, add);
	set_if_sig(c_msg, sig);
	set_if_data_common_msg(c_msg, data, len);

	set_msg_sig(c_msg, AC_IF_COMMON_MSG_OUT);
	task_post(AC_TASK_IF_ID, c_msg);
}
