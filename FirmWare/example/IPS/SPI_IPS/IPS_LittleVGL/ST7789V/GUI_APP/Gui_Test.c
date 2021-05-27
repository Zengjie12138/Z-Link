#include "Gui_Test.h"

lv_style_t bg_style;
lv_style_t indic_style;
lv_style_t btn_bg_style;
lv_style_t btn_rel_style;
lv_style_t btn_pr_style;
lv_style_t btn_tgl_rel_style;
lv_style_t btn_tgl_pr_style;
lv_obj_t* tabview1;
bool is_sliding = true;
bool is_btns_hidden = false;

//事件回调函数
void event_handler(lv_obj_t* obj, lv_event_t event)
{
    if(obj == tabview1 && event == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t page_id = lv_tabview_get_tab_act(tabview1);//获取当前可见的页面id
    }
}

void example_test()
{
    lv_obj_t* scr = lv_scr_act();//获取当前活跃的屏幕对象

    //1.创建样式
    //1.1 创建背景样式
    lv_style_copy(&bg_style, &lv_style_plain);
    bg_style.body.main_color = LV_COLOR_MAKE(49, 49, 49); //纯色背景
    bg_style.body.grad_color = bg_style.body.main_color;
    bg_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); //边框颜色
    bg_style.body.border.width = 2;//边框宽度
    bg_style.text.color = LV_COLOR_WHITE;
    //1.2 创建页面指示器的样式
    lv_style_copy(&indic_style, &lv_style_plain_color);
    indic_style.body.main_color = LV_COLOR_MAKE(42, 212, 66); //指示器的颜色,绿色
    indic_style.body.grad_color = indic_style.body.main_color;
    indic_style.body.padding.inner = 3;//设置指示器的高度
    //1.3 创建页面选择按钮栏的背景样式
    //lv_style_transp_tight样式中的inner,left,top,right,bottom等内间距值都为0,这是为了让页面选择按钮能够紧挨在一起
    lv_style_copy(&btn_bg_style, &lv_style_transp_tight);
    //1.4 创建按钮正常态下的松手样式
    lv_style_copy(&btn_rel_style, &lv_style_plain_color);
    btn_rel_style.body.main_color = LV_COLOR_MAKE(98, 98, 98);
    btn_rel_style.body.grad_color = btn_rel_style.body.main_color;
    btn_rel_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); //边框颜色
    btn_rel_style.body.border.width = 1;
    btn_rel_style.text.color = LV_COLOR_WHITE;//字体颜色
    //1.5 创建按钮正常态下的按下样式
    lv_style_copy(&btn_pr_style, &btn_rel_style);
    btn_pr_style.body.main_color = LV_COLOR_GRAY;
    btn_pr_style.body.grad_color = btn_pr_style.body.main_color;
    //1.6 创建按钮切换态下的松手样式
    lv_style_copy(&btn_tgl_rel_style, &btn_rel_style);
    btn_tgl_rel_style.body.main_color = bg_style.body.main_color;//和主背景颜色一致
    btn_tgl_rel_style.body.grad_color = btn_tgl_rel_style.body.main_color;
    //1.7 创建按钮切换态下的按下样式
    //保持和btn_tgl_rel_style一样就行了
    lv_style_copy(&btn_tgl_pr_style, &btn_tgl_rel_style);

    //2.创建选项卡
    tabview1 = lv_tabview_create(scr, NULL);
    lv_obj_set_size(tabview1, lv_obj_get_width(scr), lv_obj_get_height(scr)); //设置选项卡的大小,比屏幕小16像素
    lv_obj_align(tabview1, NULL, LV_ALIGN_CENTER, 0, 0); //与屏幕居中对齐
    lv_obj_set_event_cb(tabview1, event_handler); //设置事件回调函数
    lv_tabview_set_btns_pos(tabview1, LV_TABVIEW_BTNS_POS_TOP); //设置页面选择按钮栏位于顶部
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BG, &bg_style); //设置背景样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_INDIC, &indic_style); //设置页面指示器的样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_BG, &btn_bg_style); //设置页面选择按钮栏的背景样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_REL, &btn_rel_style); //设置按钮正常态下的松手样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_PR, &btn_pr_style); //设置按钮正常态下的按下样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_REL, &btn_tgl_rel_style); //设置按钮切换态下的松手样式
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_PR, &btn_tgl_pr_style); //设置按钮切换态下的按下样式
    //2.1 添加tab1内容页面
    lv_obj_t* tab1_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_WIFI" Tab1");
    //往内容页面1中添加标签子对象
    lv_obj_t* tmp_obj = lv_label_create(tab1_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab1 page\nIf the content\nof a tab\nbecome too long\nthe it \nautomatically\nbecome\nscrollable.");
    //往内容页面1中添加标签子对象
    tmp_obj = lv_btn_create(tab1_page, NULL);
    lv_obj_set_pos(tmp_obj, 50, 300);

    //2.2 添加tab2内容页面
    lv_obj_t* tab2_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_AUDIO" Tab2");
    //往内容页面2中添加标签子对象
    tmp_obj = lv_label_create(tab2_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab2 page\nZheng dian yuan zi");

    //2.3 添加tab3内容页面
    lv_obj_t* tab3_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_BELL" Tab3");
    //往内容页面3中添加标签子对象
    tmp_obj = lv_label_create(tab3_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab3 page\nXiong jia yu");
}
