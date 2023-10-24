#include "scr_chat.h"

char name_chat[] = "Buu2";
char sub_chat[12][12] = {
    "----1",
    "----2",
    "----3",
    "----4",
};
bool led_test = false;
bool send_chat[12];
int counter;

static void view_scr_chat();
// static void control_led_test();

view_dynamic_t dyn_view_chat = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_chat
};

view_screen_t scr_chat = {
	&dyn_view_chat,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_chat() {
    // Clear display
    view_render.clear();
    // Frame
	view_render.drawRect(0, 0, 128, 64, WHITE);
	view_render.drawFastHLine(0, 14, 128, WHITE);
	view_render.drawFastHLine(88, 15+12, 40, WHITE);
	view_render.drawFastHLine(88, 15+24, 40, WHITE);
	view_render.drawFastHLine(88, 15+36, 40, WHITE);
	view_render.drawFastVLine(88, 0, 64, WHITE);
	view_render.drawFastVLine(110, 15, 50, WHITE);
    // Text 
    view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(6, 5);
	view_render.print("Chat - ");
    view_render.print(name_chat);
    // Setting 
	view_render.setCursor(97, 5);
	view_render.print("NF24");
    view_render.setCursor(91, 18);
	view_render.print("Me");
    view_render.setCursor(113, 18);
	view_render.print(IF_TYPE_RF24_ME);
    view_render.setCursor(91, 30);
	view_render.print("You");
    view_render.setCursor(113, 30);
	view_render.print(IF_TYPE_RF24_YOU);
    view_render.setCursor(91, 42);
	view_render.print("Nwk");
    if(1) {
        view_render.setCursor(113, 42);
	    view_render.print("ON");
    }
    else {
        view_render.setCursor(113, 42);
	    view_render.print("OF");
    }
    view_render.setCursor(91, 54);
	view_render.print("Led");
    if(led_test == true) {
		view_render.fillCircle(118, 57, 4, WHITE);
	}
	else {
		view_render.drawCircle(118, 57, 4, WHITE);
	}
    
    view_render.fillRoundRect(2, 16+12*0, 85, 10, 3, WHITE);
    view_render.fillRoundRect(2, 16+12*1, 85, 10, 3, WHITE);
    view_render.fillRoundRect(2, 16+12*2, 85, 10, 3, WHITE);
    view_render.fillRoundRect(2, 16+12*3, 85, 10, 3, WHITE);

    /*1*/
    if(send_chat[3+counter] == true) {
        view_render.drawCircle(5, 20+12*0, 2, BLACK);
    }
    else {
        view_render.fillCircle(5, 20+12*0, 2, BLACK);
    }
    /*2*/
    if(send_chat[2+counter] == true) {
        view_render.drawCircle(5, 20+12*1, 2, BLACK);
    }
    else {
        view_render.fillCircle(5, 20+12*1, 2, BLACK);
    }
    /*3*/
    if(send_chat[1+counter] == true) {
        view_render.drawCircle(5, 20+12*2, 2, BLACK);
    }
    else {
        view_render.fillCircle(5, 20+12*2, 2, BLACK);
    }
    /*4*/
    if(send_chat[0+counter] == true) {
        view_render.drawCircle(5, 20+12*3, 2, BLACK);
    }
    else {
        view_render.fillCircle(5, 20+12*3, 2, BLACK);
    }

    // // Chat You
    // view_render.fillRoundRect(2, 35, 85, 11, 3, WHITE);
    // view_render.fillCircle(5, 38, 2, BLACK);

    view_render.setTextColor(BLACK);
    view_render.setCursor(12, 18+12*0);
    view_render.print(sub_chat[3+counter]);
    view_render.setCursor(12, 18+12*1);
    view_render.print(sub_chat[2+counter]);
    view_render.setCursor(12, 18+12*2);
    view_render.print(sub_chat[1+counter]);
    view_render.setCursor(12, 18+12*3);
    view_render.print(sub_chat[0+counter]);

    // // Chat Me
    // view_render.fillRoundRect(2, 8*6, 8*10, 11, 3, WHITE);
    // view_render.fillCircle(78, 55, 2, BLACK);
    // view_render.setTextColor(BLACK);
    // view_render.setCursor(20, 52);
    // view_render.print("Hello");
}

void scr_chat_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_on();

		// timer_set(AC_TASK_DISPLAY_ID, SCR_chat_COMMON_SEND, 5000, TIMER_ONE_SHOT);
		// timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_LOGO, AC_DISPLAY_STARTUP_INTERVAL, TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_BUTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
		for(int i = 11; i >= 0; i--) {
            memcpy(&sub_chat[i+1], &sub_chat[i], sizeof(sub_chat[i]));
        }
	}
		break;

	case AC_DISPLAY_BUTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_UP_RELEASED\n");
        if(counter < 7) {counter++;}
	}
		break;

	case AC_DISPLAY_BUTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_RELEASED\n");
        if(counter > 0) {counter--;}
	}
		break;

    case SCR_CHAT_COMMON_SEND: {
		APP_DBG_SIG("SCR_CHAT_COMMON_SEND\n");
        for(int i = 11; i >= 0; i--) {
            memcpy(&sub_chat[i+1], &sub_chat[i], sizeof(sub_chat[i]));
            send_chat[i+1] = send_chat[i]; 
        }

        send_chat[0] = true;
        memcpy(&sub_chat[0], get_data_common_msg(msg), sizeof(sub_chat[0]));
        nf24_common_send(IF_TYPE_RF24_YOU, \
						AC_TASK_DISPLAY_ID, \
						SCR_CHAT_COMMON_RECV, \
						(uint8_t*)&sub_chat[0], \
						sizeof(sub_chat[0]));
	}
		break;

    case SCR_CHAT_COMMON_RECV: {
		APP_DBG_SIG("SCR_CHAT_COMMON_RECV\n");
        for(int i = 11; i >= 0; i--) {
            memcpy(&sub_chat[i+1], &sub_chat[i], sizeof(sub_chat[i]));
            send_chat[i+1] = send_chat[i]; 
        }
        
        send_chat[0] = false;
        memcpy(&sub_chat[0], get_data_common_msg(msg), sizeof(sub_chat[0]));
	}
		break;

    case SCR_CHAT_CONTROL_LED: {
		APP_DBG_SIG("SCR_CHAT_CONTROL_LED\n");
	}
		break;
    }
}
