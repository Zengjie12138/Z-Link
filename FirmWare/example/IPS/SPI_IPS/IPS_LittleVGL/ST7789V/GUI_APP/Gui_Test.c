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

//�¼��ص�����
void event_handler(lv_obj_t* obj, lv_event_t event)
{
    if(obj == tabview1 && event == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t page_id = lv_tabview_get_tab_act(tabview1);//��ȡ��ǰ�ɼ���ҳ��id
    }
}

void example_test()
{
    lv_obj_t* scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����

    //1.������ʽ
    //1.1 ����������ʽ
    lv_style_copy(&bg_style, &lv_style_plain);
    bg_style.body.main_color = LV_COLOR_MAKE(49, 49, 49); //��ɫ����
    bg_style.body.grad_color = bg_style.body.main_color;
    bg_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); //�߿���ɫ
    bg_style.body.border.width = 2;//�߿���
    bg_style.text.color = LV_COLOR_WHITE;
    //1.2 ����ҳ��ָʾ������ʽ
    lv_style_copy(&indic_style, &lv_style_plain_color);
    indic_style.body.main_color = LV_COLOR_MAKE(42, 212, 66); //ָʾ������ɫ,��ɫ
    indic_style.body.grad_color = indic_style.body.main_color;
    indic_style.body.padding.inner = 3;//����ָʾ���ĸ߶�
    //1.3 ����ҳ��ѡ��ť���ı�����ʽ
    //lv_style_transp_tight��ʽ�е�inner,left,top,right,bottom���ڼ��ֵ��Ϊ0,����Ϊ����ҳ��ѡ��ť�ܹ�������һ��
    lv_style_copy(&btn_bg_style, &lv_style_transp_tight);
    //1.4 ������ť����̬�µ�������ʽ
    lv_style_copy(&btn_rel_style, &lv_style_plain_color);
    btn_rel_style.body.main_color = LV_COLOR_MAKE(98, 98, 98);
    btn_rel_style.body.grad_color = btn_rel_style.body.main_color;
    btn_rel_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); //�߿���ɫ
    btn_rel_style.body.border.width = 1;
    btn_rel_style.text.color = LV_COLOR_WHITE;//������ɫ
    //1.5 ������ť����̬�µİ�����ʽ
    lv_style_copy(&btn_pr_style, &btn_rel_style);
    btn_pr_style.body.main_color = LV_COLOR_GRAY;
    btn_pr_style.body.grad_color = btn_pr_style.body.main_color;
    //1.6 ������ť�л�̬�µ�������ʽ
    lv_style_copy(&btn_tgl_rel_style, &btn_rel_style);
    btn_tgl_rel_style.body.main_color = bg_style.body.main_color;//����������ɫһ��
    btn_tgl_rel_style.body.grad_color = btn_tgl_rel_style.body.main_color;
    //1.7 ������ť�л�̬�µİ�����ʽ
    //���ֺ�btn_tgl_rel_styleһ��������
    lv_style_copy(&btn_tgl_pr_style, &btn_tgl_rel_style);

    //2.����ѡ�
    tabview1 = lv_tabview_create(scr, NULL);
    lv_obj_set_size(tabview1, lv_obj_get_width(scr), lv_obj_get_height(scr)); //����ѡ��Ĵ�С,����ĻС16����
    lv_obj_align(tabview1, NULL, LV_ALIGN_CENTER, 0, 0); //����Ļ���ж���
    lv_obj_set_event_cb(tabview1, event_handler); //�����¼��ص�����
    lv_tabview_set_btns_pos(tabview1, LV_TABVIEW_BTNS_POS_TOP); //����ҳ��ѡ��ť��λ�ڶ���
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BG, &bg_style); //���ñ�����ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_INDIC, &indic_style); //����ҳ��ָʾ������ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_BG, &btn_bg_style); //����ҳ��ѡ��ť���ı�����ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_REL, &btn_rel_style); //���ð�ť����̬�µ�������ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_PR, &btn_pr_style); //���ð�ť����̬�µİ�����ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_REL, &btn_tgl_rel_style); //���ð�ť�л�̬�µ�������ʽ
    lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_PR, &btn_tgl_pr_style); //���ð�ť�л�̬�µİ�����ʽ
    //2.1 ���tab1����ҳ��
    lv_obj_t* tab1_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_WIFI" Tab1");
    //������ҳ��1����ӱ�ǩ�Ӷ���
    lv_obj_t* tmp_obj = lv_label_create(tab1_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab1 page\nIf the content\nof a tab\nbecome too long\nthe it \nautomatically\nbecome\nscrollable.");
    //������ҳ��1����ӱ�ǩ�Ӷ���
    tmp_obj = lv_btn_create(tab1_page, NULL);
    lv_obj_set_pos(tmp_obj, 50, 300);

    //2.2 ���tab2����ҳ��
    lv_obj_t* tab2_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_AUDIO" Tab2");
    //������ҳ��2����ӱ�ǩ�Ӷ���
    tmp_obj = lv_label_create(tab2_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab2 page\nZheng dian yuan zi");

    //2.3 ���tab3����ҳ��
    lv_obj_t* tab3_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_BELL" Tab3");
    //������ҳ��3����ӱ�ǩ�Ӷ���
    tmp_obj = lv_label_create(tab3_page, NULL);
    lv_label_set_text(tmp_obj, "\nThis is the tab3 page\nXiong jia yu");
}
