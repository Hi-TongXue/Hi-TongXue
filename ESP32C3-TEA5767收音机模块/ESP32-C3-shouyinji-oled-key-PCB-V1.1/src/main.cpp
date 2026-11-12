/*============================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563  FM收音机模块 ESP32C3版本===
  ============================================================================
  //库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    FM模块                TEA5767.h
    
  //代码及使用说明:
    1 按钮使用：
        U     单击            切换频道 增
              长按            切换频道 增
              双击            向上搜索频道

        D     短按一次        切换频道 减
              长按            切换频道 减
              双击            向下搜索频道

        MID   短按一次        切换预制的频道
   
        单按钮  单击          切换已经搜索到的频道
    2 自动搜索功能有BUG，不一定满8个频道，切换时有重复
    
*/
///////////////////////////////////////////////////////
///////////////////////////时间设置区

///////////////////////////定义Library
#include <Arduino.h>
#include "ClickButton.h"            //引用消抖库
#include <U8g2lib.h>                //引用OLED库
#include <Wire.h>
#include <TEA5767.h>

///////////////////////////GPIO连接映射
int SDA_U8G2 =  5;                  //OLED屏幕引脚
int SCL_U8G2 =  6;                  //OLED屏幕引脚

const int Button_U_PIN = 2;         //U引脚
const int Button_D_PIN = 7;         //D引脚
const int Button_MID_PIN = 8;       //r引脚

int Button_key1 =  0;               //MID按键引脚

const int Buzzer_IO = 18;           //蜂鸣引脚      I2S
const int I2S_ON_OFF_IO = 4;        //蜂鸣引脚     I2S
const int I2S_BCLK_IO = 19;         //I2S 时钟引脚


///////////////////////////库函数初始化使用
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_U8G2, SDA_U8G2);    //1.3 OLED屏幕


ClickButton button[4] = {//消抖按键映射
  ClickButton (Button_key1, LOW, CLICKBTN_PULLUP),
  ClickButton (Button_MID_PIN, LOW, CLICKBTN_PULLUP),
  ClickButton (Button_U_PIN, LOW, CLICKBTN_PULLUP),
  ClickButton (Button_D_PIN, LOW, CLICKBTN_PULLUP),
};


// 可选：可将此回调函数传递给 TEA5767 构造器。  
// 每当无线电状态更新时，该函数都会被调用。  
// 您可以使用它在外部模块（例如 OLED/LCD）上显示当前信息，或直接将其记录到串口监视器中进行调试，如示例所示。
void radioPrintInfo(const tea5767_info_t *info);

// 可选：可将此回调函数传递给 TEA5767 构造器。  
// 您只需在此处覆盖您关心的设置，其余所有值将保持默认。例如，您可以更改带宽限制、静音设置或去失真时间。  
// 这样可以避免手动初始化每个属性。

//收音机更新配置
void radioUpdateConfig(tea5767_config_t *config);
              // 电台打印信息   //收音机更新配置
TEA5767 radio(&radioPrintInfo, &radioUpdateConfig);

tea5767_info_t info;
 
float foundStations[8];       //已找到的站点
int stationCount = 0;         //站点数量

float stationcount = 87.5;
float foundstation[10] { 89.1, 92.43, 89.99, 93.32, 96.69, 92.00, 95.4, 97.1, 105.1, }; // 预制频道
int count1 = 0;

bool up1 = false, down1 = false;
bool yinpin = 1;

///////////////////////////////////////////////////////


//=========串口打印 调试用===========//可在加载页启用
  void serial_print1(){//调试 串口打印
      Serial.println("======== 串口数据 ========");
      
      Serial.println();
       
      Serial.println("============================");
  }
///////////////////////////////////////////////////////
void OLEDjiazai(){  //加载页
   
  u8g2.clearBuffer();

  u8g2.setCursor(37,30);
  u8g2.print("启动中...");
  u8g2.sendBuffer();
}

void OLEDkaiji(){   //开机页    可以打印一些串口数据用作调试
  u8g2.clearBuffer();
  u8g2.setCursor(0,27);
  u8g2.print("收音机加载中..");
  Serial.println("=================== 芯片型号 ===================");
    Serial.printf("ESP32 芯片型号 = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  delay(500);
  u8g2.sendBuffer();
    //
    
    u8g2.setCursor(0,60);
    u8g2.print(">");
      //
    delay(300);

    u8g2.sendBuffer();
    u8g2.setCursor(20,60);
    u8g2.print(">");
  //u8g2.setCursor(0,40);
  //u8g2.print("V0.0");
      //
    delay(300);

    u8g2.sendBuffer();
    u8g2.setCursor(40,60);
    u8g2.print(">");
      //
    delay(300);
  
    //
  
  u8g2.sendBuffer();
  u8g2.setCursor(60,60);
  u8g2.print(">");
  //
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(80,60);
  u8g2.print(">");
  //
  Serial.println("=============================================");
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(100,60);
  u8g2.print(">");
    //
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(120,60);
  u8g2.print(">");
    //
  delay(500);

  u8g2.sendBuffer();
  delay(700);
}


///////////////////////////////////////////
void kuangjia_shouye(){ //UI线段框架
  u8g2.setCursor( 0, 17);
  u8g2.println("---------------------");
  u8g2.setCursor(36, 27);
  u8g2.println("|");
  //u8g2.setCursor(80, 10);
  //u8g2.println("|");
  u8g2.setCursor( 0, 54);
  u8g2.println("---------------------");
}

void shouye_1(){//刷新
  u8g2.setCursor(47,10);
  u8g2.print("收音机"); 
  kuangjia_shouye();

  radio.getInfo(&info);

  u8g2.setCursor(50,27);
  u8g2.print(info.mhz);
  u8g2.setCursor(90,27);
  u8g2.print("MHz");

  u8g2.setCursor(0,61);
  u8g2.println(info.stereo ? "立体声" : "单声道");
  
  u8g2.setCursor(38,61);
  u8g2.print("调谐");
  u8g2.println(info.tune);
  
  u8g2.setCursor(81,61);
  u8g2.print("信号");
  u8g2.print(info.signal_level);
  u8g2.println("%");

  u8g2.setCursor( 0,10);
  u8g2.print(info.muted ? "静" : "音");
  
  //u8g2.setCursor( 100, 10);
  //u8g2.print( ? "" : "");

  u8g2.setCursor( 0, 27);
  u8g2.print(stationcount);
}

///////////////////////////////////////////
void shuaxin(){//刷新
      u8g2.clearBuffer(); //清屏
      u8g2.firstPage();

      shouye_1();

      
      //可在这儿加显示页面或者条件
      u8g2.nextPage();
      //u8g2.sendBuffer();  //屏幕刷新作用

}

///////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  
  u8g2.begin();                             //初始化OLED
  u8g2.enableUTF8Print();                   //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);   //文字大小15
  u8g2.setFlipMode(1);                      //翻转显示

  Wire.begin( 5, 6);  //收音模块I2C接口

  pinMode(     Buzzer_IO, OUTPUT);           //蜂鸣 I2S
  pinMode( I2S_ON_OFF_IO, OUTPUT);           //I2S
  pinMode(   I2S_BCLK_IO, OUTPUT);           //I2S 时钟
  pinMode(Button_MID_PIN, INPUT_PULLUP);     //MID
  pinMode(  Button_U_PIN, INPUT_PULLUP);     //U
  pinMode(  Button_D_PIN, INPUT_PULLUP);     //D
  pinMode(   Button_key1, INPUT_PULLUP);     //按钮

  digitalWrite(I2S_BCLK_IO , LOW);//I2S 时钟

  radio.awake();    //唤醒
  radio.setFrequency(stationcount);//设置频率
  digitalWrite(I2S_ON_OFF_IO , LOW);//蜂鸣 I2S

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//默认文字大小12

  shuaxin();
  
}

void loop() {
  button[0].Update();           //按钮
  button[1].Update();           //MID
  button[2].Update();           //U
  button[3].Update();           //D

  ///////////////////////////////////////////////////////////
  if (button[2].clicks == 1){               //U 单击            切换频道 增
    //radio.standby();
    stationcount = stationcount + 0.01;
  }
  else if (button[3].clicks == 1) {         //D 短按一次        切换频道 减
    //radio.standby();
    stationcount = stationcount - 0.01;
  }
  else if (button[2].clicks == -1)          //U 长按            切换频道 增
  {
    //Serial.println("证");
    //stationcount = stationcount + 0.01;
    up1 = true;
  }
  else if (button[3].clicks == -1)          //D  长按           切换频道 减
  {
    //Serial.println("饭");
    //stationcount = stationcount - 0.01;
    down1 = true;
  }
  else if (button[1].clicks == 1)           //MID 短按一次      切换预制的频道
  {
    count1 = count1 + 1;
    if (count1 > 8)
    {
      count1 = 0;
    }
  }////////////////////////////////////////////
  else if (button[2].clicks == 2)          //U   双击          向上搜索频道
  {
    stationCount = 0;
    radio.awake();
    for (int i = 0; i < 8; i++) {
      radio.mute();                   //关闭声音   
      radio.searchUp();               //向上搜索
      //tea5767_info_t info;
      radio.getInfo(&info);           //获取信息
      if (info.band_limit_reached) {//波段到达限制
        radio.unmute();               //解除静音
        Serial.println("上-搜频结束！");
        break;
      }
      radio.unmute();
      foundStations[stationCount] = info.mhz;
      stationCount++;
      delay(5000);
    }
  }////////////////////////////////////////////
  else if (button[3].clicks == 2)          //D   双击          向下搜索频道
  {
    stationCount = 0;
    radio.awake();
    for (int i = 0; i < 8; i++) {
      radio.mute();                   //关闭声音             
      radio.searchDown();             //向下搜索
      //tea5767_info_t info;
      radio.getInfo(&info);           //获取信息
      if (info.band_limit_reached) {
        radio.unmute();               //解除静音
        Serial.println("下-搜频结束！");
        break;
      }
      radio.unmute();
      foundStations[stationCount] = info.mhz;
      stationCount++;
      delay(5000);
    }
  }////////////////////////////////////////////
  else if (button[0].clicks == 1)           //按钮  单击          切换已经搜索到的频道
  {
    stationCount++;
  }
  else if (button[0].clicks == 2)
  {
    radio.mute(); //关闭声音
    
    
  }
  

  if (stationCount > 8)
  {
    stationCount = 0;
  }

////////////////////////////////////////////
  
  if (up1 == true)
  {//Serial.println("证11");
    static uint32_t time_1 = 0;
    if (millis() - time_1 > 200)//当按钮按下时 每多长时间增加  用于快速调至频道
    {
      time_1 = millis();
      stationcount = stationcount + 0.01;

      Serial.println(stationcount);
      //
    }
  }
  else if (down1 == true)
  {//Serial.println("饭22");
    static uint32_t time_2 = 0;
    if (millis() - time_2 > 200)
    {
      time_2 = millis();
      stationcount = stationcount - 0.01;

      Serial.println(stationcount);
      //
    }
  }
  
  if (up1 == true || down1 == true)
  {
    if (button[2].depressed == 0 && button[3].depressed == 0)
    {
      radio.standby();    //待机
      radio.awake();      //唤醒
      radio.setFrequency(stationcount);//设置频率
      
      up1 = false;
      down1 = false;
    }
    
    if (button[2].depressed == 1 || button[3].depressed == 1)
    {
      shuaxin();
    }
  }

  
////////////////////////////////////////////


  if (stationcount > 108.00)
  {
    stationcount = 108.00;
  }
  else if (stationcount < 87.50)
  {
    stationcount = 87.50;
  }

///////////////////////////////////////////////



  if (button[2].clicks == 1 || button[3].clicks == 1)
  {
    if (info.muted) {
      radio.unmute();
    }
    radio.standby();
    radio.awake();
    radio.setFrequency(stationcount);
  }
  else if (button[1].clicks == 1)
  {
    if (info.muted) {
      radio.unmute();
    }
    radio.standby();
    radio.awake();
    radio.setFrequency(foundstation[count1]);
  }
  else if (button[0].clicks == 1)
  {
    if (info.muted) {
      radio.unmute();
    }
    radio.standby();
    radio.awake();
    radio.setFrequency(foundStations[stationCount]);
  }
  
  
  ////////////////////////////////////////////////

  if (button[0].depressed == 1 || button[1].depressed == 1 || button[2].depressed == 1 || button[3].depressed == 1  || up1 == true || down1 == true)
  {
    //shuaxin();
  }

}

// 电台打印信息
void radioPrintInfo(const tea5767_info_t *info) {
  Serial.println("**********");
  if (info->muted) {
    Serial.println("关闭声音");
  } else if (info->standby) {
    Serial.println("等待");
  } else {
    if (info->searching) {
      Serial.println("搜索...");
    } else {
      Serial.print(info->mhz);
      Serial.println("MHz");
    }
  }
  Serial.print("立体声: ");
  Serial.println(info->stereo ? "立体声" : "单声道");
  Serial.print("调谐: ");
  Serial.println(info->tune);
  Serial.print("信号强度: ");
  Serial.print(info->signal_level);
  Serial.println("%");
  Serial.println("**********");

  shuaxin();
}

// 收音机更新配置
void radioUpdateConfig(tea5767_config_t *config) {
  config->standby = false;
}


