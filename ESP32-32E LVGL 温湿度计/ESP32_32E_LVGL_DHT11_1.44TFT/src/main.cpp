/*==================================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563       ESP32 lvgl温湿度计 V0.0 ===
  ==================================================================================
  //库
    LVGL
    TFT_eSPI
    DHT

  //使用GUI-Guider设计UI

  //代码及使用说明:
    1 在 User_Setup.h 中配置TFT 如屏幕的接口，传输速度，大小等等等
    2 在 lv_conf.h    中配置LVGL 如开启性能检测，打开或关闭LVGL一些功能，开启关闭动画 设置默认字体 开启其他字体等等等
    3 代码与TFT_eSPI和lvgl需要配合配置，设置不一样可能会出错
    4 ST7735  1.44寸TFT   
    5 time3已使用
    6 在generated文件夹下的文件里 能看到设计UI时GUI生成好的代码，可以以lv_ui guider_ui;   和 guider_ui. 方式引用他
    7 

*/
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////库
#include <Arduino.h>
#include <lvgl.h>           //LVGL库

#if LV_USE_TFT_ESPI         //使用TFT_eSPI库
#include <TFT_eSPI.h>
#endif

#include "generated/gui_guider.h"   //引入GUI Guider 自动生成代码
#include "custom/custom.h"          //用户自定义代码

#include "DHT_1.h"          //

///////////////////////////////////////////////////////////////////////屏幕及LVGL配置
#define TFT_HOR_RES   128       //屏幕宽度
#define TFT_VER_RES   128       //屏幕高度
#define TFT_ROTATION  LV_DISPLAY_ROTATION_180   //屏幕旋转角度
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))   //将LVGL绘制到此缓冲区中，通常使用屏幕大小的1/10即可。尺寸以字节为单位。
static uint8_t draw_buf[DRAW_BUF_SIZE];     //static 静态的变量

lv_ui guider_ui;

///////////////////////////////////////////////////////////////////////变量
int temp_sheding = 30;
int temp_biaozhi = 0;

///////////////////////////////////////////////////////////////////////
bool kaijidonghua_biaozhiwei = true;    //开机动画运行一次标志位
static lv_anim_t cai_kaijidonghua;      //要初始化的lv_anim_t变量的指针
int32_t kaijidonghua;                   //要动画的变量的指针
static void progress_anim_cb(void * var, int32_t value) {   //动画执行期间调用的函数    //这里获取进度条进度值
    lv_bar_set_value((lv_obj_t *)var, value, LV_ANIM_OFF);  //设置条形图的新值         
    kaijidonghua = value;   //全局变量
    //Serial.println(value);
}
void lvgl_kaiji_donghua(){  //开机页面
    if (kaijidonghua_biaozhiwei)
    {   //lv_anim  lvgl自带的动画函数

        //以下是开机的进度条动画配置
        lv_anim_init(&cai_kaijidonghua);                                //初始化动画变量
        lv_anim_set_var(&cai_kaijidonghua, guider_ui.screen_bar_1);     //设置变量以进行动画 //要移动的按钮
        lv_anim_set_exec_cb(&cai_kaijidonghua, progress_anim_cb);       //设置函数以动画 var //动画执行期间调用的函数
        lv_anim_set_values(&cai_kaijidonghua, 0, 100);                  //设置动画的起始和结束值
        lv_anim_set_duration(&cai_kaijidonghua, 5000);                  //设置动画的持续时间 //5000ms
        lv_anim_start(&cai_kaijidonghua);                               //启动动画

        kaijidonghua_biaozhiwei = false;    //标志位 上电一次运行
    }
    
    if (kaijidonghua == 0)
    {
        Serial.println(kaijidonghua);   //进度值  调试用
    }else if (kaijidonghua == 20)
    {
        Serial.println(kaijidonghua);   //进度值  这些进度值中可以加入一些上电一次要打印的数据
    }else if (kaijidonghua == 40)
    {
        Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 60)
    {
        Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 80)
    {
        Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 99)
    {
        Serial.println(kaijidonghua);   //进度值

        //setup_scr_screen_1(&guider_ui);   //
        //lv_scr_load(guider_ui.screen_1);  //无动画切换
        
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_del, setup_scr_screen_1, LV_SCR_LOAD_ANIM_MOVE_TOP,   300 ,    1000,   false ,        true);
        //   带动画切换屏幕                  新屏幕指针           新屏幕删除标志           旧屏幕删除标志          新屏幕创建函数             动画类型            动画时长    延迟   is_clean     自动删除旧屏幕

        //lv_refr_now(NULL);    //强制刷新  调试用
    }
}

///////////////////////////////////////////////////////////////////////
String LVGL_Arduino;
void lvgl_banbenhao(){  //打印lvgl版本号 
    LVGL_Arduino = "Hello LVGL! 版本号:  ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
                                //  LVGL主版本号                次版本号                      补丁版本号
}

static uint32_t my_tick_cb(void) {  //使用Arduino的millis()作为计时源       //配置LVGL动画时钟
    return millis();
}
///////////////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(115200);
    ////////////////////////////////////////////////////////功能引脚设置  可定义
    pinMode(13, INPUT_PULLUP);    //按钮 这里用作设定温度的按钮
    pinMode(12, INPUT_PULLUP);    //按钮 

    ////////////////////////////////////////////////////////版本号
    lvgl_banbenhao();               //获取lvgl版本号
    Serial.println( LVGL_Arduino ); //串口打印版本号

    ///////////////////////////////////////////////////////////////////LVGL初始化和配置
    lv_init();                    //LVGL初始化
    lv_tick_set_cb(my_tick_cb);   //设置时钟源

    lv_display_t *disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf)); //启用TFT_eSPI库 //一键初始化函数 //TFT_ESPI创建
    lv_display_set_rotation(disp, TFT_ROTATION);   //LVGL屏幕旋转  我的配置里是旋转180°  也就是LV_DISPLAY_ROTATION_180
    
    setup_ui(&guider_ui);   //LVGL页面GUI初始化

    Serial.println("LVGL初始化完成!");  //调试用
    ///////////////////////////////////////////////////////////////////

    lvgl_kaiji_donghua();   //开机动画 可打印数据
    
}

void loop()
{
    static uint32_t time0 = 0;
    if (millis() - time0 > 5) { //默认5ms
        time0 = millis();
        lv_timer_handler();     //LVGL运行  必要
    }
    
    lvgl_kaiji_donghua();       //开机动画 可打印数据  一次运行
    
    if (kaijidonghua == 100)    //
    {
        static uint32_t time3 = 0;
        if (millis() - time3 > 300) { //这里的延时用作按钮的消抖 300ms  当然你也可以引用专用的按钮消抖库
            time3 = millis();
            if (digitalRead(13) == 1)
            {
                temp_sheding = temp_sheding + 1;
                if (temp_sheding > 59)
                {
                    temp_sheding = 59;
                }
            }

            if (digitalRead(12) == 1)
            {
                temp_sheding = temp_sheding - 1;
                if (temp_sheding < -20)
                {
                    temp_sheding = -20;
                }
            }
        }

        static uint32_t time1 = 0;
        if (millis() - time1 > 5) { //默认5ms
            time1 = millis();

            DHT_READ_DATA();    //读取传感器数据

            lv_label_set_text_fmt(guider_ui.screen_1_label_2, "%d", (int)temp_read);    //为标签设置新的格式化文本  fmt – 类似 printf 的格式字符串 //这里转换使用 浮点转整
            lv_arc_set_value(guider_ui.screen_1_arc_1, temp_read);                      //温度 表  为弧线设置新值

            float val = temp_read;
            int digit = (int)((val - (int)val) * 10);  
            lv_label_set_text_fmt(guider_ui.screen_1_label_14, "%d", digit);          //温度 小数   显示温度

            lv_label_set_text_fmt(guider_ui.screen_1_label_4, "%d", (int)humi_read);  //湿度 数字
            lv_arc_set_value(guider_ui.screen_1_arc_2, humi_read);                    //湿度 表

            lv_label_set_text_fmt(guider_ui.screen_1_label_15, "%d", temp_sheding);   //显示温度设定值
            
        }

        //以下是完成温湿度数字外的一些功能   比如显示闹铃 数据限制等
        if (temp_read < 20)
        {   //设定线段的颜色一下同理   小于20°时
            lv_obj_set_style_line_color(guider_ui.screen_1_line_1, lv_color_hex(0xa0cc01), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else{
            lv_obj_set_style_line_color(guider_ui.screen_1_line_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        }

        if (temp_read >= 20 && temp_read < 30)
        {
            lv_obj_set_style_line_color(guider_ui.screen_1_line_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else{
            lv_obj_set_style_line_color(guider_ui.screen_1_line_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        
        if (temp_read >= 30)
        {
            lv_obj_set_style_line_color(guider_ui.screen_1_line_3, lv_color_hex(0xff1a3d), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else{
            lv_obj_set_style_line_color(guider_ui.screen_1_line_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
        }

        if (temp_sheding == (int)temp_read || (temp_sheding + 1) == (int)temp_read || (temp_sheding - 1) == (int)temp_read) //设定 ±1℃  在设定的温度值内加上限+1下限-1  也就是三个整数温度值内显示警铃
        {
            static uint32_t time2 = 0;
            if (millis() - time2 > 300) { //这里闪烁作用 警铃
                time2 = millis();
                temp_biaozhi = !temp_biaozhi;
            }

            if (temp_biaozhi == 0)
            {   //显示警铃  
                lv_label_set_text(guider_ui.screen_1_label_5, "" LV_SYMBOL_VOLUME_MAX " ");     //为标签设置新的文本
            }
            else{
                lv_label_set_text(guider_ui.screen_1_label_5, " ");
            }
        }
        else{
            lv_label_set_text(guider_ui.screen_1_label_5, " ");
        }
        
        if (temp_read < 0)
        {
            lv_label_set_text(guider_ui.screen_1_label_17, "负温度");
        }
        else{
            lv_label_set_text(guider_ui.screen_1_label_17, "");
        }
    }
    //
}


