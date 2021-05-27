#include "Gui_Test.h"

lv_obj_t *tabview;

lv_obj_t *roller;

lv_obj_t *weather;
lv_obj_t *time1,*time2;

lv_obj_t *chart;
lv_chart_series_t *series;
lv_obj_t *BPM;

lv_obj_t *weather_info;

lv_obj_t *ta;
lv_obj_t *ta_w;

LV_IMG_DECLARE(QQBar);
LV_IMG_DECLARE(heart);
LV_IMG_DECLARE(list);
LV_IMG_DECLARE(white_logo);
LV_IMG_DECLARE(qqlogo);
LV_IMG_DECLARE(smileqq);
LV_IMG_DECLARE(wea_new);



void tab1_init(lv_obj_t * parent)
{
	roller = lv_roller_create(parent,NULL);
    lv_roller_set_options(roller,"Heart\nNote\nQQ\nWeather",LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller,1,LV_ANIM_OFF);
    lv_roller_set_visible_row_count(roller,3);
    lv_obj_align(roller,parent,LV_ALIGN_IN_TOP_RIGHT,-5,0);

    time1 = lv_label_create(parent,NULL);
    lv_label_set_recolor(time1,true);
    lv_label_set_text(time1,"#DCA022 Hi,I am here...");
    lv_obj_align(time1,parent,LV_ALIGN_IN_TOP_LEFT,10,2);

    time2 = lv_label_create(parent,NULL);
	lv_label_set_recolor(time2,true);
	lv_label_set_text(time2,"#DCA022 My Name is Z-Link!");
    lv_obj_align(time2,parent,LV_ALIGN_IN_BOTTOM_MID,0,-5);

    weather = lv_label_create(parent,NULL);
    lv_label_set_recolor(weather,true);
    lv_label_set_text(weather,"#DCA022 My Lord...");
    lv_obj_align(weather,time1,LV_ALIGN_OUT_BOTTOM_RIGHT,0,5);
	
	lv_obj_t *img1 = lv_img_create(parent,NULL);
    lv_img_set_src(img1,&white_logo);
    lv_obj_align(img1,parent,LV_ALIGN_CENTER,0,0);
	
}

void tab2_init(lv_obj_t * parent)
{
	chart = lv_chart_create(parent,NULL);
    lv_obj_set_size(chart,170,140);
    lv_chart_set_type(chart,LV_CHART_TYPE_LINE);
    lv_obj_align(chart,parent,LV_ALIGN_IN_TOP_MID,0,40);

    lv_chart_set_series_width(chart,2);
    lv_chart_set_point_count(chart,30);
    lv_chart_set_div_line_count(chart,0,0);
    lv_chart_set_range(chart,450,560);

    series = lv_chart_add_series(chart,LV_COLOR_GREEN);
    lv_chart_init_points(chart,series,505);

    BPM = lv_label_create(parent,NULL);
    lv_label_set_text(BPM,"BPM:68");
    lv_obj_align(BPM,chart,LV_ALIGN_OUT_BOTTOM_MID,0,20);

	
	lv_obj_t *info = lv_label_create(parent,NULL);
	lv_label_set_recolor(info,true);
	lv_label_set_text(info,"#ff0000 PULSE");
	lv_obj_align(info,chart,LV_ALIGN_OUT_TOP_MID,15,-20);
	
	lv_obj_t *img1 = lv_img_create(parent,NULL);
    lv_img_set_src(img1,&heart);
    lv_obj_align(img1,info,LV_ALIGN_OUT_LEFT_MID,-5,0);
}

void tab3_init(lv_obj_t * parent)
{
	
    ta = lv_ta_create(parent,NULL);
    lv_obj_set_size(ta,180,150);
    lv_obj_align(ta,parent,LV_ALIGN_IN_TOP_MID,0,40);
    lv_ta_set_cursor_type(ta,LV_CURSOR_NONE);
    

    lv_obj_t *info = lv_label_create(parent,NULL);
	lv_label_set_recolor(info,true);
    lv_label_set_text(info,"#DCA022 NOTE");
    lv_obj_align(info,ta,LV_ALIGN_OUT_TOP_MID,0,-20);
	
	lv_obj_t *img1 = lv_img_create(parent,NULL);
    lv_img_set_src(img1,&list);
    lv_obj_align(img1,info,LV_ALIGN_OUT_LEFT_MID,-5,0);
}

void tab4_init(lv_obj_t * parent)
{
	lv_obj_t *img1 = lv_img_create(parent,NULL);
    lv_img_set_src(img1,&QQBar);
    lv_obj_align(img1,parent,LV_ALIGN_CENTER,0,-20);
	
	
	lv_obj_t *info = lv_label_create(parent,NULL);
	lv_label_set_recolor(info,true);
	lv_label_set_text(info,"#DCA022 Make Friends?");
	lv_obj_align(info,img1,LV_ALIGN_OUT_BOTTOM_MID,-10,30);
	
	lv_obj_t *img3 = lv_img_create(parent,NULL);
	lv_img_set_src(img3,&smileqq);
	lv_obj_align(img3,info,LV_ALIGN_OUT_RIGHT_MID,5,0);
	
}

void tab5_init(lv_obj_t * parent)
{

	ta_w = lv_ta_create(parent,NULL);
    lv_obj_set_size(ta_w,180,150);
    lv_obj_align(ta_w,parent,LV_ALIGN_IN_TOP_MID,0,60);
    lv_ta_set_cursor_type(ta_w,LV_CURSOR_NONE);
	lv_ta_set_text(ta_w,"BLE NOT LINK!");
	
	lv_obj_t *info = lv_label_create(parent,NULL);
	lv_label_set_recolor(info,true);
	lv_label_set_text(info,"#DCA022 Today Weather");
	lv_obj_align(info,parent,LV_ALIGN_IN_TOP_MID,20,5);
	
	lv_obj_t *img3 = lv_img_create(parent,NULL);
	lv_img_set_src(img3,&wea_new);
	lv_obj_align(img3,info,LV_ALIGN_OUT_LEFT_MID,-5,0);
	
}



void Gui_View(void)
{
    lv_obj_t * scr = lv_scr_act();//获取当前活跃的屏幕对象

    lv_theme_t * theme = lv_theme_night_init(90,NULL);//创建主题
    lv_theme_set_current(theme);//使用主题
	

    tabview = lv_tabview_create(scr,NULL);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview,"Tab 1");
    tab1_init(tab1);

    lv_obj_t * tab2 = lv_tabview_add_tab(tabview,"Tab 2");
    tab2_init(tab2);

    lv_obj_t * tab3 = lv_tabview_add_tab(tabview,"Tab 3");
    tab3_init(tab3);
	
	lv_obj_t * tab4 = lv_tabview_add_tab(tabview,"Tab 4");
    tab4_init(tab4);
	
	lv_obj_t * tab5 = lv_tabview_add_tab(tabview,"Tab 5");
    tab5_init(tab5);
	

    lv_tabview_set_btns_hidden(tabview,true);
	
}


void Navig_Process(uint8_t direct)
{
	uint8_t id = 0;
	
	id = lv_roller_get_selected(roller);
	switch(direct)
	{
		case UP:
			if(id == 4)
				id = 4;
			else
				id++;
			lv_roller_set_selected(roller,id,LV_ANIM_OFF);
		break;
		case DOWN:
			if(id == 0)
				id = 0;
			else
				id--;
			lv_roller_set_selected(roller,id,LV_ANIM_OFF);
		break;
		case LEFT:                                        //BACK
		break;
		case RIGHT:	                                      //ENTER
			lv_tabview_set_tab_act(tabview,id+1,LV_ANIM_OFF);
			page_id = id+1;
		break;
		
		default:
		break;
	}
}

void Pulse_Process(uint8_t direct)
{
	int value = 0;
	static uint8_t BPM_V = 0;
	uint8_t temp[10] = {0};
	
	value = pulse_process(&BPM_V);
	lv_chart_set_next(chart,series,value);
	sprintf((char*)temp,"BPM:%d",BPM_V);
	lv_label_set_text(BPM,(char*)temp);

	switch(direct)
	{
		case UP:

		break;
		case DOWN:
			
		break;
		case LEFT:
			page_id = 0;
			lv_tabview_set_tab_act(tabview,0,LV_ANIM_OFF);
		break;
		case RIGHT:	
			
		break;
		
		default:
		break;
	}
}

void Note_Process(uint8_t direct)
{
	
	switch(direct)
	{
		case UP:
			
		break;
		case DOWN:
			
		break;
		case LEFT:
			lv_tabview_set_tab_act(tabview,0,LV_ANIM_OFF);
		    page_id = 0;
		break;
		case RIGHT:	
			
		break;
		
		default:
		break;
	}
}

void QQ_Process(uint8_t direct)
{
	switch(direct)
	{
		case UP:
			
		break;
		case DOWN:
			
		break;
		case LEFT:
			lv_tabview_set_tab_act(tabview,0,LV_ANIM_OFF);
		    page_id = 0;
		break;
		case RIGHT:	
		break;
		
		default:
		break;
	}
}

void Weather_Process(uint8_t direct)
{
	switch(direct)
	{
		case UP:
			
		break;
		case DOWN:
			
		break;
		case LEFT:
			lv_tabview_set_tab_act(tabview,0,LV_ANIM_OFF);
		    page_id = 0;
		break;
		case RIGHT:	
		break;
		
		default:
		break;
	}
}























