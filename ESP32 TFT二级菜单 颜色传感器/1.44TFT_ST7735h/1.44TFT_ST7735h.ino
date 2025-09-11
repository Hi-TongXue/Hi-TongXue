/*此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563 现版本：V2.0
  //库引用记录
    TFT1.44 三按键二级菜单+标题 ESP32E 
    按键库ClickButton
    中文库U8g2_for_Adafruit_GFX
    TFT库ST7735
    传感器库：MH_TCS34725.h
    rgb库：FastLED.h
    核心图形库 Adafruit_GFX.h
  //BUG及代码记录（OLED菜单TFT移植）：
    1 光标不动
    2 菜单UI 无啦！
    3 光标UI未显示但可选择
    4 打开串口打印调试
    5 出现双光标
    6 重新整理算法优化V1.1
    7 启用二级三级菜单
    8 桌面页RGB显示
    9 启用按键长按
    10 引用TCS34732库
    11 设置RGB通道
    12 映射RGB
    13 二级菜单RGB显示重复
    14 二级菜单 起飞啦！
    15 TFT显示异常
    16 重新整理算法优化V1.2
    17 初始化TFT按钮
    18 显示二级按钮1
    19 二级菜单标题 木有了！
    20 二级光标多重显示
    21 二级光标数据和二级按钮数据错误重合
    22 引用RGBWS2812库
    23 TCS传感器映射RGB
    24 白块显示多重，未设置刷新
    25 设置数字按键UI排列
    26 数据UI显示GPIO映射
    27 数字开关拥有了自己的画面
    28 数字开关光标UI使用三维数组
    29 数字按键光标UI两按钮设置上下限
    30 代码整理算法优化V1.8
    31 数字按键启用输出功能
    32 启用第四按钮，设定数字按键状态
    33 数字按键状态使用二维数组
    34 代码整理算法优化V2.0
    35 留存BUG：屏幕刷新极慢，设置UI页面需要刷新两次
*/
///////////////////////////////////引用库
  #include <Adafruit_GFX.h>           // 核心图形库
  #include <Adafruit_ST7735.h>        // ST7735 硬件专用库
  #include <U8g2_for_Adafruit_GFX.h>  //中文库
  #include "ClickButton.h"            //消抖库
  #include "MH_TCS34725.h"            //传感器TCS3472库
  #include "FastLED.h"                //RGB库
///////////////////////////////////定义TFT_RGBWS2812——GPIO
  #define TFT_SCLK 18     // SCL
  #define TFT_MOSI 23     // SDA
  #define TFT_RST 4       // 复位
  #define TFT_DC 26       // 数据/命令选择
  #define TFT_CS 2        // 芯片选择

  #define NUM_LEDS 16             // LED灯珠数量
  #define DATA_PIN 32             // 输出控制信号引脚
  #define LED_TYPE WS2812         // RGB驱动芯片型号
  #define COLOR_ORDER GRB         // RGB灯珠中<R, G, B>  LED的排列顺序
///////////////////////////////////       
///////////////// 按键+数字按键——GPIO            
  int key1 = 16;          //按键1
  int key2 = 17;          //按键2
  int key3 = 12;          //按键4
  int key4 = 19;          //未使用
  int key5 = 15;          //按键3

  int led = 2;            //数字按键灯引脚
  int Relay1_IO = 14;     //数字按键继电1引脚
  int Relay2_IO = 27;     //数字按键继电2引脚
  int Relay3_IO = 13;     //数字按键继电3引脚

  int Relay4_IO = 99;     //数字按键继电4引脚//以下引脚可使用
  int Relay5_IO = 88;     //数字按键继电5引脚
  int Relay6_IO = 77;     //数字按键继电6引脚
  int Relay7_IO = 66;     //数字按键继电7引脚
  int Relay8_IO = 55;     //数字按键继电8引脚
///////////////// 多数值
  uint16_t clear, red, green, blue;   //获取颜色传感器R G B 通道
  uint32_t sum;                       //传感器亮度赋值并转换
  float r, g, b;                      //RGB通道赋值
  int data1 = 2;  //初始菜单按键光标数值
  int data2 = 0;  //一级菜单数值
  int data3 = 1;  //二级菜单按键光标X数值
  int data4 = 0;  //二级菜单按键光标Y数值
///////////////// 拓展数字量
  int data20 ;    //未使用
  int data21 ;    //未使用
///////////////// 数组  
  int data10[10] = { 0, 2, 42, 84};     //一级光标UI X位置
  int data17[10][10][10] = {            //二级光标，三维数组           
                      { //二级光标UI X位置
                        {0, 4, 46, 90},//一行
                        {0, 4, 46, 90},//二行
                        {0, 4, 46, 90},//三行
                      },
                      { //二级光标UI Y位置
                        {0, 46, 46, 46},//一行
                        {0, 76, 76, 76},//二行
                        {0, 106, 106, 106},//三行
                      }
                    };
  int data11[10][10] = {                //数字按键设置页面数组
                          {0, 0, 0, 0}, 
                          {0, 0, 0, 0}, 
                          {0, 0, 0, 0},
                      };    
  int data12[10] = { 0, 30, 5, 30, 5};  //白块位置
  //int data13[10] = { 35, 27, 28, 10, 0}; //标题位置(未使用)
  int data14[10] = { 68, 218, 59};      //RGB初始值&变量值
  int data15[10] = { 49, 79, 109, 0, 0};//二级设置UI数字按键Y位置
  int data16[10] = {15, 57, 101,   15, 57, 101,   15, 57, 101};//二级设置UI数字按键X位置
  ////////////////// R    G    B   
///////////////////////////////////中文U8G2_TFTST7735_KEY_TCS34725——GPIO定义
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);//TFT屏幕初始化引脚
  U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;//中文显示

  MH_TCS34725 tcs = MH_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);//颜色传感器（tcs函数名）

  ClickButton button[5] = {//消抖按键映射
    ClickButton (key1, LOW, CLICKBTN_PULLUP),
    ClickButton (key2, LOW, CLICKBTN_PULLUP),
    ClickButton (key3, LOW, CLICKBTN_PULLUP),
    ClickButton (key4, LOW, CLICKBTN_PULLUP),
    ClickButton (key5, LOW, CLICKBTN_PULLUP),
  };

  uint8_t max_bright = 255;       // RGB灯亮度
  CRGB leds[NUM_LEDS];            // 建立RGB灯
//////////////////////////////////////////////////////////////////////运行函数
  void setup() {
    Serial.begin(115200);                  // 初始化串口
    tft.initR(INITR_144GREENTAB);          // 初始化 ST7735 显示屏
    tft.setRotation(1);                    // 设置屏幕旋转 （0：0） （1：90） （2：180） 
    tft.fillScreen(ST7735_BLACK); 
    //常见速度值：1000000 (1MHz), 5000000 (5MHz), 10000000 (10MHz), 20000000 (20MHz), 40000000 (40MHz)
    tft.setSPISpeed(80000000);             // 目前设置为80MHz

    u8g2_for_adafruit_gfx.begin(tft);      //U8g2库TFT中文显示初始化

    //pinMode(mo1 ,  INPUT);//模拟引脚映射（测试用，可删除）
    tft.fillRoundRect(25, data12[1], 85, 24, 8, ST7735_WHITE); //白块
    button[4].longClickTime = 850;        // Key长按判定时间：850ms（按住超过0.85秒即视为长按）
    
    LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化RGB灯带
    FastLED.setBrightness(max_bright);
    if (tcs.begin()) {//传感器初始化及检测是否连接（出于调试方便，检测状态未显示在TFT上）
      Serial.println("发现传感器");         //发现传感器
    } else {//否
      Serial.println("找不到TCS34725…检查连接");//找不到TCS34725…检查连接
      while (1);
    }

    pinMode(led, OUTPUT);       // 设置引脚为输出模式
    pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式
    /*
    pinMode(Relay4_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay5_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay6_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay7_IO, OUTPUT); // 设置引脚为输出模式
    pinMode(Relay8_IO, OUTPUT); // 设置引脚为输出模式
    */
  }
  void loop() {
    xuan ();
  }
//////////////////////////////////////////////////////////////////////
void moni (){//模拟量（调试用） 未使用
  /*
    data1 = map(analogRead(mo1), 0, 1024, 0, 128) ; 

    tft.drawRoundRect(data1, data4, 40, 30, 8, ST7735_WHITE); //光标
  */
}
///////////////////////////////////
void xuan (){//选择按键
  //////////////////按钮更新
    button[0].Update();   //key1 左
    button[1].Update();   //key2 右
    button[2].Update();   //key3 上
    button[3].Update();   //key4 下
    button[4].Update();   //key5 右上
  if(button[0].clicks == 1 || button[2].clicks == -1 || button[0].clicks == -1 || button[1].clicks == 1 || button[1].clicks == -1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[4].clicks == -1 || button[4].clicks == 2){
    KUAI_WHITE(); //白块1
  }//屏幕白块
  //////////////////按钮初始化
    if(button[0].clicks == 1) {//clicks是次数 //按键1
      if(data2 == 0){//一级
        data1++;
      }
    }
    else if(button[0].clicks == -1){//按键1长按
      if(data2 == 1 && data1 == 1){//二级
        data3++;
      }
    }
    else if(button[1].clicks == 1 ){//按键2
      if(data2 == 0){//一级
        data1--;
      }
    }
    else if(button[1].clicks == -1){//按键2长按
      if(data2 == 1 && data1 == 1){//二级
        data3--;
      }
    }
    else if(button[2].clicks == -1){//按键4长按翻转数字开关
      data11[data4][data3] = !data11[data4][data3];//翻转
    }
    else if(button[4].clicks == 1){//按键3单击进退菜单键
      data2++;
    }
    else if(button[4].clicks == -1){//按键3长按功能
        data2++;
    }
  /////////////////
  if(button[0].clicks == 1  || button[2].clicks == -1 || button[0].clicks == -1 || button[1].clicks == 1 || button[1].clicks == -1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[4].clicks == -1 || button[4].clicks == 2){
    ALLScreen();  //屏幕清零
  }//屏幕清零
  //////////////////一二级菜单
    if (data2 == 0){//初始菜单页
      Cinese_UI_Xuan();
      xuanUI ();
    }
    else if (data2 == 1 && data1 == 1){//设置选择页
      shezhiUI();
      xuanUI ();
      if(button[0].clicks == 1  || button[2].clicks == -1 || button[0].clicks == -1 || button[1].clicks == 1 || button[1].clicks == -1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[4].clicks == -1 || button[4].clicks == 2){
          ALLScreen();  //屏幕清零
      }//屏幕清零

      if (data2 > 1){
        data2 = 0;
      }
    }
    else if (data2 == 1 && data1 == 2){//桌面选择页
      zhuomianUI();
      if (data2 > 1){
        data2 = 0;
      }
    }
    else if (data2 == 1 && data1 == 3){//数据选择页
      canshuUI();
      if (data2 > 1){
        data2 = 0;
      }
    }
    else{//回到初始菜单并且RGBColor=0
      data2 = 0;
      if (data2 == 0){//初始菜单页
        r = 0;
        g = 0;
        b = 0;
        RGBLED_Set();//WS2812
      }
    }
  ////////////////
  if(button[0].clicks == 1  || button[2].clicks == -1 || button[0].clicks == -1  || button[1].clicks == 1 || button[1].clicks == -1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[4].clicks == -1 || button[4].clicks == 2){
    KUAI_WHITE(); //白块1
  }//屏幕白块
}
///////////////////////////////////三大菜单UI
  void shezhiUI(){            //设置UI
    tft.setTextColor(ST7735_BLACK); //设置标题颜色
    tft.setTextSize(2);     //设置标题大小
    tft.setCursor(35, 11);  //设置标题位置X/Y
    tft.print("SetUI");     //显示标题

    UI_Chinese2 ();//中文显示
    /////////////////////////////////////////第一行
      u8g2_for_adafruit_gfx.setCursor(8, 45);
      u8g2_for_adafruit_gfx.print("叮~");

      u8g2_for_adafruit_gfx.setCursor(50, 45);
      u8g2_for_adafruit_gfx.print("数字1");

      u8g2_for_adafruit_gfx.setCursor(94, 45);
      u8g2_for_adafruit_gfx.print("数字2");
    /////////////////////////////////////////第二行
      u8g2_for_adafruit_gfx.setCursor(8, 75);//Y差值30
      u8g2_for_adafruit_gfx.print("数字3");

      u8g2_for_adafruit_gfx.setCursor(50, 75);
      u8g2_for_adafruit_gfx.print("数字4");

      u8g2_for_adafruit_gfx.setCursor(94, 75);
      u8g2_for_adafruit_gfx.print("数字5");
    /////////////////////////////////////////第三行
      u8g2_for_adafruit_gfx.setCursor(8, 105);//Y差值30
      u8g2_for_adafruit_gfx.print("数字6");

      u8g2_for_adafruit_gfx.setCursor(50, 105);
      u8g2_for_adafruit_gfx.print("数字7");

      u8g2_for_adafruit_gfx.setCursor(94, 105);
      u8g2_for_adafruit_gfx.print("数字8");

    ///////// 
    ////////////////////////数字按键UI
      //////////////////////////////////第一行
        tft.drawRoundRect(8, 49, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[0], 55, 5, ST7735_WHITE); // 填充圆形 X:15~30
        digitalWrite(led, data11[0][1]);//输出引脚功能
        
        tft.drawRoundRect(50, 49, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[1], 55, 5, ST7735_WHITE); // 绘制填充圆形X:57~72 X差值15
        digitalWrite(Relay1_IO, data11[0][2]);

        tft.drawRoundRect(94, 49, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[2], 55, 5, ST7735_WHITE);// 填充圆形X:101~116
        digitalWrite(Relay2_IO, data11[0][3]);
      //////////////////////////////////第二行
        tft.drawRoundRect(8, 79, 30, 13, 6, ST7735_WHITE);//圆角矩形//Y差值4
        tft.fillCircle(data16[3], 85, 5, ST7735_WHITE); // 填充圆形//Y差值6 X差值15
        digitalWrite(Relay3_IO, data11[1][1]);
        
        tft.drawRoundRect(50, 79, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[4], 85, 5, ST7735_WHITE); // 绘制填充圆形
        digitalWrite(Relay4_IO, data11[1][2]);

        tft.drawRoundRect(94, 79, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[5], 85, 5, ST7735_WHITE);// 填充圆形
        digitalWrite(Relay5_IO, data11[1][3]);
      //////////////////////////////////第三行
        tft.drawRoundRect(8, 109, 30, 13, 6, ST7735_WHITE);//圆角矩形//Y差值4
        tft.fillCircle(data16[6], 115, 5, ST7735_WHITE); // 填充圆形//Y差值6 X差值15
        digitalWrite(Relay6_IO, data11[2][1]);
        
        tft.drawRoundRect(50, 109, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[7], 115, 5, ST7735_WHITE); // 绘制填充圆形
        digitalWrite(Relay7_IO, data11[2][2]);

        tft.drawRoundRect(94, 109, 30, 13, 6, ST7735_WHITE);//圆角矩形
        tft.fillCircle(data16[8], 115, 5, ST7735_WHITE);// 填充圆形
        digitalWrite(Relay8_IO, data11[2][3]);
    ////////////////////////
    /////////数字按键状态UI
    ////第一行
      if (data11[0][1] == 1){//LED
        data16[0] = 30;
      }
      else {
        data16[0] = 15;
      }
      if(data11[0][2] == 1){//数字1
        data16[1] = 72;
      }
      else {
        data16[1] = 57;
      }
      if(data11[0][3] == 1){//数字2
        data16[2] = 116;
      }
      else {
        data16[2] = 101;
      }
    ////第二行
      if(data11[1][1] == 1){//数字3
        data16[3] = 30;
      }
      else {
        data16[3] = 15;
      }
      if(data11[1][2] == 1){//数字4
        data16[4] = 72;
      }
      else {
        data16[4] = 57;
      }
      if(data11[1][3] == 1){//数字5
        data16[5] = 116;
      }
    ////第三行
      else {
        data16[5] = 101;
      }
      if(data11[2][1] == 1){//数字6
        data16[6] = 30;
      }
      else {
        data16[6] = 15;
      }
      if(data11[2][2] == 1){//数字7
        data16[7] = 72;
      }
      else {
        data16[7] = 57;
      }
      if(data11[2][3] == 1){//数字8
        data16[8] = 116;
      }
      else{
        data16[8] = 101;
      }
    ////////////////////////高低限功能是我特制的，其更改了20多次才成功，别看它代码长，且珍惜
    if(data3 > 3){//光标选择最高限制
      if (data4 != 2){
        data3 = 1;
      }
      else if (data4 == 2){
        data3 = 3;
      }

      if (data4 == 0){
        data4 = 1;
      }
      else if (data4 == 1){
        data4 = 2;
      }
    }
    else if(data3 < 1){//光标选择最低限制
      if (data4 != 0){
        data3 = 3;
      }
      else if (data4 == 0){
        data3 = 1;
      }
      
      if (data4 == 2){
        data4 = 1;
      }
      else if (data4 == 1){
        data4 = 0;
      }
    }
  }
  void zhuomianUI(){          //桌面UI
    TCSGetRGB ();   //RGB通道映射
    KUAI_WHITE ();  //TCS映射白块
    RGBLED_Set();   //WS2812

    tft.setTextColor(ST7735_BLACK); 
    tft.setTextSize(2); 
    tft.setCursor(27, 35); 
    tft.print("DesktopUI"); 

    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(15, 110); 
    tft.print("R:"); 
    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(26, 110); 
    tft.print(data14[0]); 

    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(55, 110);
    tft.print("G:");
    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(66, 110); 
    tft.print(data14[1]); 

    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(95, 110);
    tft.print("B:");
    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); 
    tft.setCursor(106, 110);
    tft.print(data14[2]);

    //用方块显示RGB传感器读取的RGB色彩
    tft.fillRoundRect(25, 60, 85, 45, 8, tft.color565(data14[0], data14[1], data14[2])); 
  }
  void canshuUI(){            //数据UI（主要其显示GPIO接口）
    tft.setTextColor(ST7735_BLACK); 
    tft.setTextSize(2);  
    tft.setCursor(28, 11); 
    tft.print("DataUI"); 

    UI_Chinese2 ();//中文显示
    ////////////////////////////////////////第一行
      u8g2_for_adafruit_gfx.setCursor(8, 45);
      u8g2_for_adafruit_gfx.print("按钮1");
      u8g2_for_adafruit_gfx.setCursor(8, 55);
      u8g2_for_adafruit_gfx.print(key1);

      u8g2_for_adafruit_gfx.setCursor(50, 45);
      u8g2_for_adafruit_gfx.print("按钮2");
      u8g2_for_adafruit_gfx.setCursor(50, 55);
      u8g2_for_adafruit_gfx.print(key2);

      u8g2_for_adafruit_gfx.setCursor(94, 45);
      u8g2_for_adafruit_gfx.print("按钮5");
      u8g2_for_adafruit_gfx.setCursor(94, 55);
      u8g2_for_adafruit_gfx.print(key5);
    ////////////////////////////////////////第二行
      u8g2_for_adafruit_gfx.setCursor(8, 70);
      u8g2_for_adafruit_gfx.print("SCLK");
      u8g2_for_adafruit_gfx.setCursor(8, 80);
      u8g2_for_adafruit_gfx.print(TFT_SCLK);

      u8g2_for_adafruit_gfx.setCursor(50, 70);
      u8g2_for_adafruit_gfx.print("MOSI");
      u8g2_for_adafruit_gfx.setCursor(50, 80);
      u8g2_for_adafruit_gfx.print(TFT_MOSI);

      u8g2_for_adafruit_gfx.setCursor(94, 70);
      u8g2_for_adafruit_gfx.print("RST");
      u8g2_for_adafruit_gfx.setCursor(94, 80);
      u8g2_for_adafruit_gfx.print(TFT_RST);
    ////////////////////////////////////////第三行
      u8g2_for_adafruit_gfx.setCursor(8, 95);
      u8g2_for_adafruit_gfx.print("DC");
      u8g2_for_adafruit_gfx.setCursor(8, 105);
      u8g2_for_adafruit_gfx.print(TFT_DC);

      u8g2_for_adafruit_gfx.setCursor(50, 95);
      u8g2_for_adafruit_gfx.print("CS");
      u8g2_for_adafruit_gfx.setCursor(50, 105);
      u8g2_for_adafruit_gfx.print(TFT_CS);

      u8g2_for_adafruit_gfx.setCursor(94, 95);
      u8g2_for_adafruit_gfx.print("RGB灯");
      u8g2_for_adafruit_gfx.setCursor(94, 105);
      u8g2_for_adafruit_gfx.print(DATA_PIN);
  }
///////////////////////////////////一级菜单UI
  void Cinese_UI_Xuan(){      //中文选项卡
    tft.setTextColor(ST7735_WHITE); 
    tft.setTextSize(1); // 设置字体大小
    tft.setCursor(10, 10); // 设置光标位置
    tft.print("V2.0"); // 显示文本

    tft.setTextColor(ST7735_BLACK); 
    tft.setTextSize(2); 
    tft.setCursor(35, 35); 
    tft.print("Title~"); 

    UI_Chinese ();
    u8g2_for_adafruit_gfx.setCursor(8, 120);
    u8g2_for_adafruit_gfx.print("设置");
    u8g2_for_adafruit_gfx.setCursor(48, 120);
    u8g2_for_adafruit_gfx.print("桌面");
    u8g2_for_adafruit_gfx.setCursor(90, 120);
    u8g2_for_adafruit_gfx.print("数据");

    if(data1 > 3){//光标选择最高限制
      data1 = 3;
    }
    else if(data1 < 1){//光标选择最低限制
      data1 = 1;
    }
  }
///////////////////////////////////传感器 TCS&RGBWS
  void TCSGetRGB (){          //TCS34725传感器
    //获取颜色传感器R G B 通道
    tcs.getRGBC(&red, &green, &blue, &clear);
    tcs.lock();                         // 关闭LED提示
    //RGB灯 RGB通道赋值
    sum = clear;            
    r = red; r /= sum;                  // “/=”符：除且赋值
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;       //最大值

    /////////////////////
    data14[0] = r;
    data14[1] = g;
    data14[2] = b;
    ALLScreen ();//TFT屏幕实时刷新
  }
  void RGBLED_Set(){          //WS2812
    CRGB myRGBcolor(r,g,b);
    fill_solid(leds, NUM_LEDS, myRGBcolor);
    FastLED.show();
  }
///////////////////////////////////白块&光标
  void KUAI_WHITE (){ //白块
    if (data2 == 0){//菜单
      tft.fillRoundRect(25, data12[1], 85, 24, 8, ST7735_WHITE); //白块
    }
    else if (data2 == 1 && data1 == 1){//设置
      tft.fillRoundRect(25, data12[2], 85, 24, 8, ST7735_WHITE); //白块
    }
    else if (data2 == 1 && data1 == 2){//桌面
      tft.fillRoundRect(25, data12[3], 85, 24, 8, ST7735_WHITE); //白块
    }
    else if (data2 == 1 && data1 == 3){//数据
      tft.fillRoundRect(25, data12[4], 85, 24, 8, ST7735_WHITE); //白块
    }
  }
  void xuanUI (){             //光标UI
    if(data2 == 0){//一级菜单光标UI
      tft.drawRoundRect( data10[data1], 103, 40, 24, 8, tft.color565(255, 255, 255)); //光标3 98Y103
    }
    else if(data2 == 1 && data1 == 1){//二级设置光标UI
      tft.drawRoundRect( data17[0][0][data3], data17[1][data4][1], 37, 19, 8, tft.color565(255, 255, 255)); //光标3 98Y103
    }
  }
///////////////////////////////////
void chuan_Serial1(){       //串口打印1（调试用）
  Serial.println("print 1:");
  Serial.println(button[0].depressed);
  Serial.println(button[1].depressed);
  Serial.println(data2);
}

void chuan_Serial2(){       //串口打印2（调试用）
  //Serial.println("串口打印2");
  //Serial.println();
}

void UI_Chinese (){         //U8g2中文库  TXT大小：14
  u8g2_for_adafruit_gfx.setFontMode(1);                     //显示模式                
  u8g2_for_adafruit_gfx.setFontDirection(0);                //显示方向
  u8g2_for_adafruit_gfx.setForegroundColor(ST7735_WHITE);   //中文颜色   
  u8g2_for_adafruit_gfx.setFont(u8g2_font_wqy14_t_gb2312);  //中文大小
}

void UI_Chinese2 (){        //U8g2中文库2 TXT大小：12
  u8g2_for_adafruit_gfx.setFontMode(1);    
  u8g2_for_adafruit_gfx.setFontDirection(0);            
  u8g2_for_adafruit_gfx.setForegroundColor(ST7735_WHITE);      
  u8g2_for_adafruit_gfx.setFont(u8g2_font_wqy12_t_gb2312);
}

void ALLScreen (){          //屏幕清零
  tft.fillScreen(ST7735_BLACK);//整块屏幕清除为黑色
  //tft.fillScreen(tft.color565(128, 0, 128));//使用RGB
}

//以下为调试和学习使用，可删除
////////剩余数据/函数/打印/定义///////////////////////////
  /*
  tft.fillScreen(ILI9341_BLACK); // 清除屏幕并设置为黑色背景
  // 绘制线条
  tft.drawLine(0, 0, 100, 100, ST7735_RED); // 从 (0,0) 到 (100,100) 绘制红色线条
  // 绘制矩形
  tft.drawRect(10, 10, 50, 30, ST7735_GREEN); // 绘制绿色矩形
  // 绘制圆形
  tft.drawCircle(60, 60, 20, ST7735_BLUE); // 绘制蓝色圆形
  // 填充矩形
  tft.fillRect(20, 20, 40, 20, ST7735_YELLOW); // 填充黄色矩形
  //圆角矩形       radius：  圆角半径（控制圆角弧度大小）
  tft.drawRoundRect(x, y, width, height, radius, color);

  // 在坐标(120, 160)处绘制半径为50的红色填充圆
  tft.fillCircle(120, 160, 50, TFT_RED);

  // 可以在填充圆上再绘制一个轮廓，形成边框效果
  tft.drawCircle(120, 160, 50, TFT_BLACK); // 黑色边框

  tft.setTextSize(1); // 设置字体大小
  tft.setCursor(100, 60); // 设置光标位置
  tft.setTextColor(ST7735_RED); // 设置字体颜色为白色
  tft.println("abcd"); // 显示文本
  //tft.fillScreen(ST7735_WHITE);
  */
