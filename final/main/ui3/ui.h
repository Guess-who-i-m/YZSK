// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
void update_chart_data(float *heart_rate_values, float *spo2_values, size_t length);

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_Label1;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_Image13;
extern lv_obj_t * ui_Image9;
// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_Panel3;
extern lv_obj_t * ui_Label4;
extern lv_obj_t * ui_Image25;
void ui_event_Button2(lv_event_t * e);
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_Image16;
void ui_event_Button4(lv_event_t * e);
extern lv_obj_t * ui_Button4;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui_Image15;
void ui_event_Button10(lv_event_t * e);
extern lv_obj_t * ui_Button10;
extern lv_obj_t * ui_Label25;
extern lv_obj_t * ui_Image10;
// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
extern lv_obj_t * ui_Screen5;
extern lv_obj_t * ui_Panel6;
extern lv_obj_t * ui_Label11;
extern lv_obj_t * ui_TextArea1;
extern lv_obj_t * ui_Label12;
void ui_event_Button3(lv_event_t * e);
extern lv_obj_t * ui_Button3;
extern lv_obj_t * ui_Label2;
void ui_event_Button7(lv_event_t * e);
extern lv_obj_t * ui_Button7;
extern lv_obj_t * ui_Label8;
void ui_event_Button11(lv_event_t * e);
extern lv_obj_t * ui_Button11;
extern lv_obj_t * ui_Label16;
// SCREEN: ui_Screen6
void ui_Screen6_screen_init(void);
extern lv_obj_t * ui_Screen6;
extern lv_obj_t * ui_Panel7;
extern lv_obj_t * ui_Label14;
void ui_event_Button9(lv_event_t * e);
extern lv_obj_t * ui_Button9;
extern lv_obj_t * ui_Label10;
void ui_event_Button12(lv_event_t * e);
extern lv_obj_t * ui_Button12;
extern lv_obj_t * ui_Label18;
void ui_event_Button8(lv_event_t * e);
extern lv_obj_t * ui_Button8;
extern lv_obj_t * ui_Label13;
extern lv_obj_t * ui_Chart1;
extern lv_obj_t * ui_Label9;
extern lv_obj_t * ui_Label19;
// SCREEN: ui_Screen7
void ui_Screen7_screen_init(void);
extern lv_obj_t * ui_Screen7;
extern lv_obj_t * ui_Panel8;
extern lv_obj_t * ui_Label17;
void ui_event_Button5(lv_event_t * e);
extern lv_obj_t * ui_Button5;
extern lv_obj_t * ui_Label20;
extern lv_obj_t * ui_Panel2;
extern lv_obj_t * ui_Label7;
extern lv_obj_t * ui_Panel4;
extern lv_obj_t * ui_Label15;
extern lv_obj_t * ui_Image2;
extern lv_obj_t * ui_TextArea2;
extern lv_obj_t * ui_Panel9;
extern lv_obj_t * ui_Label22;
extern lv_obj_t * ui_Image3;
extern lv_obj_t * ui____initial_actions0;


LV_IMG_DECLARE(ui_img_403540010);    // assets/active (2).png
LV_IMG_DECLARE(ui_img_1582379155);    // assets/ai (3).png
LV_IMG_DECLARE(ui_img_1005215284);    // assets/语音识别训练 (1).png
LV_IMG_DECLARE(ui_img_321964393);    // assets/门户管理.png
LV_IMG_DECLARE(ui_img_people_png);    // assets/people.png
LV_IMG_DECLARE(ui_img_left_png);    // assets/left.png
LV_IMG_DECLARE(ui_img_score_png);    // assets/score.png
LV_IMG_DECLARE(ui_img_ai_png);    // assets/AI.png
LV_IMG_DECLARE(ui_img_1_png);    // assets/1.png
LV_IMG_DECLARE(ui_img_chatbox2_png);    // assets/chatbox2.png
LV_IMG_DECLARE(ui_img_img_202407175155_60x60_png);    // assets/IMG_202407175155_60x60.png
LV_IMG_DECLARE(ui_img_img_202407177275_60x60_png);    // assets/IMG_202407177275_60x60.png
LV_IMG_DECLARE(ui_img_logo0_png);    // assets/logo0.png
LV_IMG_DECLARE(ui_img_logo1_png);    // assets/logo1.png
LV_IMG_DECLARE(ui_img_logo2_png);    // assets/logo2.png
LV_IMG_DECLARE(ui_img_weather_1_png);    // assets/weather_1.png
LV_IMG_DECLARE(ui_img_weather_2_png);    // assets/weather_2.png
LV_IMG_DECLARE(ui_img_weather_3_png);    // assets/weather_3.png



LV_FONT_DECLARE(ui_font_Font1);
LV_FONT_DECLARE(ui_font_FontLast);



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
