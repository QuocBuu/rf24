#include "scr_nf24.h"

static void view_scr_nf24();

int led_b5;

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_nf24
};

view_screen_t scr_nf24 = {
	&dyn_view_startup,
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
	view_render.print("Add R");
	if(1) {
		static char add_r[32];
		snprintf(add_r, \
				sizeof(add_r), \
				"%d.%d.%d.%d.%d", \
				mac_des_addr[0], \
				mac_des_addr[1], \
				mac_des_addr[2], \
				mac_des_addr[3], \
				mac_des_addr[4]);
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
				mac_src_addr[0], \
				mac_src_addr[1], \
				mac_src_addr[2], \
				mac_src_addr[3], \
				mac_src_addr[4]);
		view_render.setCursor(45, 30);
		view_render.print(add_s);
	}
	else {
		view_render.setCursor(45, 30);
		view_render.print("...");
	}
	view_render.setCursor(3, 42);
	view_render.print("Size");
	if(0) {
		view_render.setCursor(45, 42);
		view_render.print("Addr Recv");
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
	}
		break;

	case AC_DISPLAY_SHOW_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_LOGO\n");
		// SCREEN_TRAN(scr_info_handle, &scr_info);
	}
		break;

	case AC_DISPLAY_SHOW_IDLE: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
		// SCREEN_TRAN(scr_idle_handle, &scr_idle);
	}
		break;

	default:
		break;
	}
}
