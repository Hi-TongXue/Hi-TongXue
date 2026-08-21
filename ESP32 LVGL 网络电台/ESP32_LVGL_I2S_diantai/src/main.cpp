/*==================================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563       ESP32 lvgl网络电台      ===
  ==================================================================================
  //库
    LVGL
    TFT_eSPI
    ESP32-audioI2S-2.0.6
    ClickButton.h
    WiFi.h

  //使用GUI-Guider设计UI

  //代码及使用说明:
    1 在 User_Setup.h 中配置TFT 如屏幕的接口，传输速度，大小等等等
    2 在 lv_conf.h    中配置LVGL 如开启性能检测，打开或关闭LVGL一些功能，开启关闭动画 设置默认字体 开启其他字体等等等
    3 代码与TFT_eSPI和lvgl需要配合配置，设置不一样可能会出错
    4 ST7735  1.44寸TFT   
    5 在generated文件夹下的文件里 能看到设计UI时GUI生成好的代码，可以以lv_ui guider_ui;   和 guider_ui. 方式引用他
    * 
    * 
    6 time0 time4 time5 time6   已使用
    7 这里使用ESP32-32E模组，因为没有PSRAM 可能随时内存溢出  建议用S3  之后改参数   setBufsize( 16384 , 0 )   (使用内部RAM ,  使用PSRAM)  platformio.ini也要改
    8 基于ESP32-audioI2S库 版本2.0.6  使用S3时建议用最新库
    9 

*/
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////库
#include "Arduino.h"
#include <lvgl.h>                   //LVGL库

#if LV_USE_TFT_ESPI                 //使用TFT_eSPI库
#include <TFT_eSPI.h>
#endif

#include "generated/gui_guider.h"   //引入GUI Guider 自动生成代码
#include "custom/custom.h"          //用户自定义代码


#include "Audio.h"                  //ESP32-audioI2S库
#include <WiFi.h>                   //引用wifi库


#include "ClickButton.h"            //引用消抖库
//////////////////////////////////////////////////////////////////////
///////////////////////////////////// I2S
#define I2S_BCLK  25   //BCLK
#define I2S_LRC   26   //LRC
#define I2S_DOUT  14   //DIN
///////////////////////////////////// 按钮
#define KEY_UP_PIN      32
#define KEY_DOWN_PIN    33
#define KEY_ENTER_PIN   15
#define KEY_RIGHT_PIN   13
#define KEY_LEFT_PIN    12

int led = 2;    //用于wifi连接状态显示
bool led_bool = 0;
////////////////////////////////////////////////////////////// 广播名称
//中国之声  京津冀之声  国际新闻  北京新闻广播  山东经济广播  深圳新闻广播  济南交通广播  潍坊新闻广播
const char *txt_1 [8] { "中国之声" , "京津冀之声" , "国际新闻" , "北京新闻广播" , "山东经济广播" , "深圳新闻广播" , "济南交通广播" , "潍坊新闻广播" };
int txt_xuan = 0;

////////////////////////////////////////////////////////////// 按钮消抖
ClickButton button[5] = {//消抖按键映射
    ClickButton ( KEY_UP_PIN    , LOW, CLICKBTN_PULLUP),
    ClickButton ( KEY_DOWN_PIN  , LOW, CLICKBTN_PULLUP),
    ClickButton ( KEY_ENTER_PIN , LOW, CLICKBTN_PULLUP),
    ClickButton ( KEY_RIGHT_PIN , LOW, CLICKBTN_PULLUP),
    ClickButton ( KEY_LEFT_PIN  , LOW, CLICKBTN_PULLUP),
};

int button_biaozhi = 0;
////////////////////////////////////////////////////////////// 广播网址 （找的网上资源，可能随时会没）
Audio audio;    

const char *user_M3U8_1 [8] PROGMEM { "https://lhttp.qtfm.cn/live/15318317/64k.mp3", "https://lhttp.qtfm.cn/live/5022463/64k.mp3" , "https://lhttp.qtfm.cn/live/20500172/64k.mp3" , "https://lhttp.qingting.fm/live/339/64k.mp3" , "https://lhttp.qtfm.cn/live/20236/64k.mp3" , "http://lhttp.qingting.fm/live/1270/64k.mp3" , "https://lhttp.qingting.fm/live/1669/64k.mp3" , "https://lhttp.qtfm.cn/live/20320/64k.mp3"};  // "https://piccpndali.v.myalicdn.com/audio/cctv13_2.m3u8";
int audio_biaozhiwei = 0;
/////////////////////////////////////////////////////////////////////// 屏幕及LVGL配置
#define TFT_HOR_RES   128       //屏幕宽度
#define TFT_VER_RES   128       //屏幕高度
#define TFT_ROTATION  LV_DISPLAY_ROTATION_180   //屏幕旋转角度
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))   //将LVGL绘制到此缓冲区中，通常使用屏幕大小的1/10即可。尺寸以字节为单位。
static uint8_t draw_buf[DRAW_BUF_SIZE];     //static 静态的变量

lv_ui guider_ui;

/////////////////////////////////////////////////////////////////////// 
lv_group_t * group;             //控件组 这里用于滚轮

/////////////////////////////////////////////////////////////////////// 变量
bool wificonnect_biaozhi = true;
bool connecttohost_biaozhiwei = true;
int yinliang = 15;              //音量默认 15   0 ~ 21

int16_t roller_weizhi;          //记录滚轮位置
bool audio_lianjiebiaozhiwei;

/////////////////////////////////////////////////////////////////////// 开机动画
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
        //Serial.println(kaijidonghua);   //进度值  调试用
    }else if (kaijidonghua == 20)
    {
        //Serial.println(kaijidonghua);   //进度值  这些进度值中可以加入一些上电一次要打印的数据
    }else if (kaijidonghua == 40)
    {
        //Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 60)
    {
        //Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 80)
    {
        //Serial.println(kaijidonghua);   //进度值
    }else if (kaijidonghua == 99)
    {
        Serial.println(kaijidonghua);   //进度值

        //setup_scr_screen_1(&guider_ui);   //
        //lv_scr_load(guider_ui.screen_1);  //无动画切换

        ui_load_scr_animation(&guider_ui, 
                      &guider_ui.screen_1, 
                      &guider_ui.screen_1_del,   // 注意 & 
                      &guider_ui.screen_del, 
                      setup_scr_screen_1, 
                      LV_SCR_LOAD_ANIM_MOVE_TOP, 
                      700, 
                      0, 
                      false, 
                      true);

        //ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_del, setup_scr_screen_1, LV_SCR_LOAD_ANIM_MOVE_TOP,   300 ,    1000,   false ,        true);
        //   带动画切换屏幕                  新屏幕指针           新屏幕删除标志           旧屏幕删除标志          新屏幕创建函数             动画类型            动画时长    延迟   is_clean     自动删除旧屏幕

        //lv_refr_now(NULL);    //强制刷新  调试用

        
    }
}

/////////////////////////////////////////////////////////////////////// 按钮选择
void button_chuli(){    //按钮处理1  这里用作切换频道的哈
    button[0].Update();           //按钮状态更新
    button[1].Update();           //

    if (button[0].clicks == 1){
        lv_group_focus_obj(guider_ui.screen_1_roller_1); lv_group_send_data(  group, LV_KEY_DOWN);  //下方向键    //将焦点移至某个小部件      向组内具有焦点的控件发送控制字符
    }
    else if (button[1].clicks == 1){
        lv_group_focus_obj(guider_ui.screen_1_roller_1); lv_group_send_data(  group, LV_KEY_UP);    //上方向键    //将焦点移至某个小部件      向组内具有焦点的控件发送控制字符。
    }
    else if(button[0].clicks == -1 ){  //lv_group_send_data(  group, LV_KEY_DOWN); 
        button_biaozhi = 1;
        Serial.println("打印1");
    }  //
    else if(button[1].clicks == -1 ){  //lv_group_send_data(  group, LV_KEY_UP); 
        button_biaozhi = 2;
        Serial.println("打印2");
    }
    
    if (button[1].depressed == 0 && button[0].depressed == 0){
        button_biaozhi = 0;
    }
        

    if (button_biaozhi == 1)
    {
        static uint32_t time4 = 0;
        if (millis() - time4 > 300) { //默认5ms
            time4 = millis();  lv_group_focus_obj(guider_ui.screen_1_roller_1); lv_group_send_data(  group, LV_KEY_DOWN); //下方向键    //将焦点移至某个小部件      向组内具有焦点的控件发送控制字符
        }
        Serial.println(button_biaozhi);
    }
    else if (button_biaozhi == 2)
    {
        static uint32_t time5 = 0;
        if (millis() - time5 > 300) { //默认5ms
            time5 = millis();  lv_group_focus_obj(guider_ui.screen_1_roller_1); lv_group_send_data(  group, LV_KEY_UP);   //上方向键    //将焦点移至某个小部件      向组内具有焦点的控件发送控制字符。
        }
        Serial.println(button_biaozhi);
    }

    //此处用于同步滚轮与网址还有广播名称的位置对应
    if (button[0].clicks == 1 || button[1].clicks == 1 || button[0].clicks == -1 || button[1].clicks == -1)
    {   //
        roller_weizhi = lv_roller_get_selected(guider_ui.screen_1_roller_1);    //获取所选选项的索引 也就是滚轮的索引位置
        Serial.printf("当前滚到位置: %d\n", roller_weizhi); //测试

        static uint32_t time6 = 0;
        if (millis() - time6 > 0) { //缓冲作用 默认0
            time6 = millis();  

            audio.setVolume(0); //设定音量 0
            audio.stopSong();   //彻底停止播放 停止获取网络流
            delay(200);         //缓冲作用 默认200
            
            audio_lianjiebiaozhiwei = audio.connecttohost(user_M3U8_1 [roller_weizhi]); // 连接播放流  
            if (audio_lianjiebiaozhiwei)
            {
                audio.setVolume(yinliang); // 0...21
            }
            
        }
        
    } 
    
}
void button_chuli_2(){  //按钮处理2  这里用作音量大小
    button[2].Update();           //未使用
    button[3].Update();           //
    button[4].Update();           //


    /* 未使用 使用带有PSRAM的模组之后可用
    if (button[2].clicks == 1)
    {//播放、暂停
        bool ret = audio.pauseResume();
        Serial.print("pauseResume returned: ");
        Serial.println(ret);
        Serial.print("isRunning: ");
        Serial.println(audio.isRunning());
    }*/
    

    if (button[3].clicks == 1)
    {
        yinliang++;
    }
    else if (button[4].clicks == 1)
    {
        yinliang--;
    }
    else if (button[4].clicks == -1)
    {
        yinliang = 0;
    }

    if (button[3].clicks == 1 || button[4].clicks == 1 || button[4].clicks == -1)
    {
        audio.setVolume(yinliang); // 0...21
    }
    
    
    //限制
    if (yinliang > 21)
    {
        yinliang = 21;
    }
    else if (yinliang < 0)
    {
        yinliang = 0;
    }
    
    
}

/////////////////////////////////////////////////////////////////////// 打印LVGL版本  
String LVGL_Arduino;
void lvgl_banbenhao(){  //打印lvgl版本号 
    LVGL_Arduino = "Hello LVGL! 版本号:  ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
                                //  LVGL主版本号                次版本号                      补丁版本号
}

static uint32_t my_tick_cb(void) {  //使用Arduino的millis()作为计时源       //配置LVGL动画时钟
    return millis();
}

/////////////////////////////////////////////////////////////////////// 多任务 这里用作处理音频，用在核心Core0上
void audioTask(void *pvParameters) {    //多任务任务入口函数
    while (1) {    //创建任务参数
        audio.loop(); //库运行需要 必加
    }
}


void setup() {
   Serial.begin(115200);
   
    //////////////////////////////////////////////////////// 功能引脚设置  可定义
    pinMode( led, OUTPUT);
    

    //////////////////////////////////////////////////////// 版本号
    lvgl_banbenhao();               //获取lvgl版本号
    Serial.println( LVGL_Arduino ); //串口打印版本号

    //////////////////////////////////////////////////////// LVGL初始化和配置
    lv_init();                    //LVGL初始化
    lv_tick_set_cb(my_tick_cb);   //设置时钟源

    lv_display_t *disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf)); //启用TFT_eSPI库 //一键初始化函数 //TFT_ESPI创建
    lv_display_set_rotation(disp, TFT_ROTATION);   //LVGL屏幕旋转  我的配置里是旋转180°  也就是LV_DISPLAY_ROTATION_180
    
    setup_ui(&guider_ui);               //LVGL页面GUI初始化

    Serial.println("LVGL初始化完成!");  //调试用

    lv_scr_load(guider_ui.screen_2); 
    lv_timer_handler();                 //LVGL运行  必要    这里用作更新页面
    //////////////////////////////////////////////////////// 显示配网页面  EspTouch App一键配网
    WiFi.mode(WIFI_STA);                    //Station模式
    WiFi.beginSmartConfig();                //初始化配网
    Serial.println("\nSmartConfig");
    Serial.println("手机 EspTouch App 配网");
    while (!WiFi.smartConfigDone()) {       //判断配网是否完成  循环
        delay(500);
        Serial.print(".");
        led_bool = !led_bool;
        digitalWrite( led, led_bool);
        //return;
    }

    Serial.println("\nSmartConfig成功, 连接WiFi");
    while (WiFi.status() != WL_CONNECTED) { //判断WIFI是否连接  循环
        delay(500);
        Serial.print(".");
        led_bool = !led_bool;
        digitalWrite( led, led_bool);
        //return;
    }
    
    Serial.println("\nWiFi已连接");
    digitalWrite( led, LOW);    //连接完成之后的状态
    Serial.print("IP地址:");
    Serial.println(WiFi.localIP());

    lv_scr_load(guider_ui.screen);  //加载屏幕页面
    lv_timer_handler();             //这里用作刷新页面
    //////////////////////////////////////////////////////// 
    while (kaijidonghua != 100)
    {
        lvgl_kaiji_donghua();   //开机动画 可打印数据
        lv_timer_handler();     //LVGL运行  必要
        //return;
    }

    /////////////////////////////////////////////////////// 初始化网络流
    audio.setBufsize( 16384 , 0 );                          //网络流预加载限制  RAM 缓冲区 15KB，禁用 PSRAM    (使用内部RAM ,  使用PSRAM)
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);           //设置I2S引脚输出
    audio.setVolume(yinliang);                              //音量 范围0~21
    audio.connecttohost(user_M3U8_1 [audio_biaozhiwei]);    //连接播放流   mp3 、 M3U8
    audio.setConnectionTimeout(10000, 6000);                //设置连接超时时间 HTTP HTTPS/SSL  毫秒

    /////////////////////////////////////////////////////// 控件 滚轮
    group = lv_group_create();                                //创建新的控件组。
    lv_group_add_obj(group, guider_ui.screen_1_roller_1);     //向组中添加小部件。
    lv_group_focus_obj(guider_ui.screen_1_roller_1);          //将焦点移至某个小部件（取消当前小部件的焦点）。

    ////////////////////////////////////////////////////// 多任务使用，双核心    audio.loop();在这里使用，不干扰LVGL
    xTaskCreatePinnedToCore(//创建具有指定关联的新任务。
        audioTask,          //任务入口函数
        "AudioTask",        //任务的描述性名称
        4096,               //指定的任务栈大小（以字节为单位）
        NULL,               //用于传递创建任务参数的指针
        1,                  //任务运行的优先级
        NULL,               //用于返回创建任务的句柄，以便后续引用该任务
        0                   //运行核心 Core0
    );
    ///////////////////////////////////////////////////////
}

void loop()
{
    static uint32_t time0 = 0;
    if (millis() - time0 > 2) { //默认5ms  这里用2ms
        time0 = millis();
        lv_timer_handler();     //LVGL运行  必要
    }

    button_chuli();     //按钮处理  这里用作切换频道的哈
    button_chuli_2();   //按钮处理  这里用作音量大小

    
    if (kaijidonghua == 100)
    {
        /////////////////////////////////////////////////////////////////////////////////
        String ipStr = WiFi.localIP().toString();                       //IP地址      转换为String
        lv_label_set_text(guider_ui.screen_1_label_14, ipStr.c_str());  //显示IP地址  转换String

        char rssi_buf[8];                                               //缓冲组
        if (WiFi.status() == WL_CONNECTED) {                            //判断WIFI是否连接
            snprintf(rssi_buf, sizeof(rssi_buf), "%d", WiFi.RSSI());    //使用snprintf 对不同的数据类型组合   
        } else {
            strcpy(rssi_buf, "error");
        }
        lv_label_set_text(guider_ui.screen_1_label_11, rssi_buf);   //最终在屏幕上显示RSSI值，用于判断WIFI信号连接


        lv_label_set_text(guider_ui.screen_1_label_2, txt_1[roller_weizhi]);    //显示广播名称
        
        char buf3[12];                                          //缓冲组
        snprintf(buf3, sizeof(buf3), "%d", yinliang);           //转换数据类型 
        lv_label_set_text(guider_ui.screen_1_label_7, buf3);    //显示音量
        /////////////////////////////////////////////////////////////////////////////////

        if (connecttohost_biaozhiwei == true){  //未使用
            //audio.connecttohost(user_M3U8_1); 
            connecttohost_biaozhiwei = false;
        }
        
    }
    
}

void audio_info(const char *info){  //反馈使用 必须添加
    Serial.print("音频信息: "); Serial.println(info);
}















