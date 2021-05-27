#ifndef _GUI_TEST_H_
#define _GUI_TEST_H_

#include "lvgl.h"
#include "mpu6050.h"
#include "pulse.h"
#include "stdio.h"
#include "fatfs.h"
#include "lv_port_fs.h"

extern FIL file;  

extern lv_obj_t *chart;
extern lv_chart_series_t *series;
extern lv_obj_t * tabview;
extern lv_obj_t *ta;
extern lv_obj_t *ta_w;
extern lv_obj_t *time1,*time2;

extern uint8_t page_id;

void Gui_View(void);

void Navig_Process(uint8_t direct);
void Pulse_Process(uint8_t direct);
void Note_Process(uint8_t direct);
void QQ_Process(uint8_t direct);
void Weather_Process(uint8_t direct);


#endif
