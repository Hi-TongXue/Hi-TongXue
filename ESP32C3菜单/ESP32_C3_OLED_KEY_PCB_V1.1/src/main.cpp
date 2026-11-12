/*==========================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563    二级菜单 ESP32C3版本===
  ==========================================================================
  //库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    多任务库              SimplifiedTimer.h
    温湿度库              DHT11  DHT20
    RGB库                 Adafruit_NeoPixel.h

  //代码及使用说明:
    1 菜单框架更新，使用millis作为延时，可在时间设置区设置延时值
    2 lastTime  lastTime1  2  3  4  5 已用  
    3 NS4168默认关闭
    4 蜂鸣器默认关闭
    5 对应V1.1的PCB板 板载1个用于RGB的固定接口  GPIO 1
    6 引脚配置：
     --------------------------------------------------------
      板载RGB    IO1
      NS4168 I2S:
              CTRL  IO  4
              LRCLK IO 10
              BCLK  IO 19
              SDATA IO 18
      外接IO IO 3   (可用于PWM)
      I2C : (外接接口共用)
          SDA  IO 5
          SCL  IO 6 
      拨盘按钮：
          IO 2 7 8
      单按钮：
          IO 0
     --------------------------------------------------------
    7 温湿度传感器可选择，DHT11或DHT20
    8 扩展IO口可用于外接RGB
    

*/
///////////////////////////////////////////////////////
///////////////////////////时间设置区
  int gundong_TXT     =   1;      //滚动速度
  int shezhi_TXT      =   900;    //设置光标选项 闪烁速度
  int zhuomian_TXT    =   900;    //桌面光标选项 闪烁速度
  int shuju_TXT       =   900;    //数据光标选项 闪烁速度
  int fengming_TXT    =   0;     //蜂鸣延时 35

///////////////////////////定义Library
  #include <U8g2lib.h>                //引用OLED库
  #include "ClickButton.h"            //引用消抖库
  #include <SimplifiedTimer.h>        //多任务库
  #include <Arduino.h>
  //#include "PWM_TOMO.h"               //PWM 可启用
  #include "RGB_1.h"               //RGB
  #include "DHT_1.h"               //DHT1   DHT11
  #include "DHT_2.h"               //DHT2   DHT20

///////////////////////////GPIO连接映射
  const int Button_U_PIN = 2;         //U引脚
  const int Button_D_PIN = 7;         //D引脚
  const int Button_MID_PIN = 8;       //r引脚

  int Button_key1 =  0;               //MID按键引脚

  int SDA_U8G2 =  5;                  //OLED屏幕引脚
  int SCL_U8G2 =  6;                  //OLED屏幕引脚

  const int Buzzer_IO = 18;           //蜂鸣引脚     I2S
  const int I2S_ON_OFF_IO = 4;        //蜂鸣引脚     I2S
  const int I2S_BCLK_IO = 19;         //I2S 时钟引脚

  int led = 4;              //数字按键灯引脚（未使用）
  int Relay1_IO = 10;       //数字按键继电1引脚（未使用）
  //int Relay2_IO = 12;     //数字按键继电2引脚（未使用）
  //int Relay3_IO = 14;     //数字按键继电3引脚（未使用）

  //int A_D_1 = 34;         //模拟量电位器引脚（未使用）
///////////////////////////数组  光标位置 二级光标位置
  //
  int data1[10] = { 0, 1, 45, 88, 0};               //一级光标位置
  int data2[10][10] = { //二维数组 二级菜单光标XY位置
                    {1, 43, 88}, 
                    {1, 43, 88, 0},//多出的一个元素为数字按键位置限制
                  };               
  int data3[10][10] = { //二维数组 设置页面数字按键 
                        {0, 0, 0},//一行
                        {0, 0, 0},//二行
                      }; 
  int data4[10][10][10] = { //三维数组 设置页数字按键X位置
                            {
                              {10, 52, 97},//一行
                              {10, 52, 97},//二行
                            },
                            { //设置页数字按键Y位置
                              {25, 25, 25},
                              {49, 49, 49},
                            }
                          }; 
///////////////////////////数字量  选择和菜单-数量及参数
  //刷新转换之后的数值
  int data10 = 2;             //一级光标选择
  int data11 = 0;             //菜单级别数量数值
  int data12 = 0;             //二级(设置)菜单光标选择
  int data13 = 48;            //二级（设置）光标Y轴位置
  //刷新转换之前的数值
  int data20 = 2;             //一级光标选择
  int data21 = 0;             //菜单级别数量数值
  int data22 = 0;             //二级(设置)菜单光标选择
  
  int data23 = 0;            //二级（设置）光标X轴位置 一行
  int data24 = 0;            //二级（设置）光标X轴位置 二行

  int WINDOW_UI = 0;         // ui切换
  int DATA_UI = 0;           // ui切换

  int xuni_key1 = 0;           //RGB2关闭 //虚拟按键：代码只循环一次
  int xuni_key2 = 0;           //按钮互锁运行一次 //虚拟按键：代码只循环一次
  
  int xuni_key6 = 0;           //已使用 ID查询 //虚拟按键：代码只循环一次

  int begin_dataA;//记录初始值
  int begin_dataB;

  int UP_dataA;//记录当前值
  int UP_dataB;

  int Servo_DATA;

  int OLED_UI_time1 = 0;//光标移动位置 设置选项卡闪烁
  int OLED_UI_time2 = 0;

  int OLED_UI_time3 = 0;//光标移动位置 桌面选项卡闪烁
  int OLED_UI_time4 = 0;

  int OLED_UI_time5 = 0;//光标移动位置 数据选项卡闪烁
  int OLED_UI_time6 = 0;
  //////////////////////////////////////////////////////////

  ////////////////////////芯片ID数据
    char part_XH1[50];      //芯片ID 组合数值
    int ID_time_UI;         //UI滚动
    int ID_time_X_UI = -240;//UI滚动值
    
  ////////////////////////DHT选择
    int DHT_xuan = 0;

///////////////////////////库函数初始化使用
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_U8G2, SDA_U8G2);    //1.3 OLED屏幕

  ClickButton button[4] = {//消抖按键映射
    ClickButton (Button_key1, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_MID_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_U_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_D_PIN, LOW, CLICKBTN_PULLUP),
  };

  SimplifiedTimer fastTimer;    // 多任务 任务1
  SimplifiedTimer fastTimer1;    // 多任务 任务2

///////////////////////////////////////////////////////


//=========串口打印 调试用===========//可在加载页启用
  void serial_print1(){//调试 串口打印  这里用作HTML数据传输
      Serial.println("======== 温湿度数据 ========");
      
      if (DHT_xuan == 0)  //区分11和20传感器用于HTML本地数据记录
      {
        Serial.println("DHT11温度");
        Serial.println(temp_read);
        Serial.println("DHT11湿度");
        Serial.println(humi_read);
      }
      else if (DHT_xuan == 1)
      {
        Serial.println("DHT20温度");
        Serial.println(temp_read2);
        Serial.println("DHT20湿度");
        Serial.println(humi_read2);
      }
      Serial.println("============================");
  }


void ESP32_ID(){  //芯片型号
  sprintf( part_XH1, "ESP32芯片型号 = %s Rev%d \n", ESP.getChipModel(), ESP.getChipRevision());
  //Serial.println(part_XH1);//调试
  //C语言格式化函数 %s字符串 %d整数
}

void font_oled_1(){//默认字体
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//默认文字大小12
}
void font_oled_2(){//数码管字体
  u8g2.setFont(u8g2_font_7_Seg_33x19_mn);//数码管字体
}

void OLEDjiazai(){  //加载页
   
  u8g2.clearBuffer();

  u8g2.setCursor(37,30);
  u8g2.print("启动中...");
  u8g2.sendBuffer();
}

void OLEDkaiji(){   //开机页    可以打印一些串口数据用作调试
  u8g2.clearBuffer();
  u8g2.setCursor(0,20); //27
  u8g2.print("模块加载中...");
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
  u8g2.setCursor(0,35);
  u8g2.print("PCB-V1.1");
  u8g2.setCursor(0,47);
  u8g2.print("UID 620162563");
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

  //u8g2.clearBuffer();
  //u8g2.drawBitmap(32, 0, 8, 64, gImage_CatHead);  //可以加开机图片
  //u8g2.sendBuffer();
  //delay(1800);
}

///////////////////////////////////////////PWM
void PWM_LOOP(){//PWM调速运行
  //ledcWrite(PWM_CHANNEL, PWM_DATA1);            // 占空比
}
 

///////////////////////////////////////////桌面UI
  void zhuomianUI1(){ //桌面1 
      u8g2.setCursor(0, 10);
      u8g2.print("①");
      //在这儿加页面

      u8g2.setCursor(10, 30);
      u8g2.print("RGB亮度值: ");
      u8g2.print(r);
    u8g2.print(" ");
      u8g2.print(g);
      u8g2.print(" ");
      u8g2.print(b);

      u8g2.setCursor(10, 45);
      u8g2.print("RGB圈 色度值: ");
      u8g2.print(count1);
      
      u8g2.setCursor(10, 60);
      u8g2.print("RGB圈 亮度值: ");
      u8g2.print(B);
  }
  void zhuomianUI2(){ //桌面2 
    u8g2.setCursor(0, 10);
    u8g2.print("②");

    
    
    static unsigned long lastTime5 = 0;
    if (millis() - lastTime5 > 10)  //每多少MS采集
    {
      if (DHT_xuan == 0)//区分DHT11和DHT20两种传感器
      {
        DHT_READ_DATA();

        //u8g2.setCursor(20, 30);
        //u8g2.print("湿度值: ");
        
        u8g2.setCursor(70, 60);
        u8g2.print(humi_read);
        u8g2.print(" ％");
        
        //u8g2.setCursor(20, 45);
        //u8g2.print("温度值: ");
        
        u8g2.setCursor(5, 15);
        font_oled_2();          //数码管字体
        u8g2.print(temp_read);
        font_oled_1();          //初始化字体
        u8g2.setCursor(100, 46);
        u8g2.print(" ℃");

        serial_print1();
      }
      else if (DHT_xuan == 1)
      {
        DHT_READ_DATA_2();

        //u8g2.setCursor(20, 30);
        //u8g2.print("湿度值: ");

        u8g2.setCursor(70, 60);
        u8g2.print(humi_read2);
        u8g2.print(" ％");


        //u8g2.setCursor(20, 45);
        //u8g2.print("温度值: ");
        
        u8g2.setCursor(5, 15);
        font_oled_2();          //数码管字体
        u8g2.print(temp_read2);
        font_oled_1();          //初始化字体
        u8g2.setCursor(100, 46);
        u8g2.print(" ℃");

        serial_print1();
      }
      
      lastTime5 = millis();
    }
    
  }
  void zhuomianUI3(){ //桌面3 未使用
    u8g2.setCursor(0, 10);
    u8g2.print("③");

    //
  }
  void zhuomianUI4(){ //桌面4 未使用
    u8g2.setCursor(0, 10);
    u8g2.print("④");

    //
  }
  void zhuomianUI5(){ //桌面5 未使用
      u8g2.setCursor(0, 10);
      u8g2.print("⑤");

      //
  }
///////////////////////////////////////////参数UI
void canshuUI1(){
    u8g2.setCursor(0, 10);
    u8g2.print("①");
    
    u8g2.setCursor( 3,25);
    u8g2.print("RGB1");
    u8g2.setCursor( 3,34);
    u8g2.print(PIN);

    u8g2.setCursor(45,25);
    u8g2.print("RGB2");
    u8g2.setCursor(45,34);
    u8g2.print(PIN2);

    u8g2.setCursor(90,25);
    u8g2.print("CTR");      //I2S
    u8g2.setCursor(90,34);
    u8g2.print("4");
    ////
    u8g2.setCursor( 3,49);
    u8g2.print("LRC");      //I2S
    u8g2.setCursor( 3,59);
    u8g2.print("10");

    u8g2.setCursor(45,49);
    u8g2.print("BCL");      //I2S
    u8g2.setCursor(45,59);
    u8g2.print("19");

    u8g2.setCursor(90,49);
    u8g2.print("SDA");
    u8g2.setCursor(90,59);
    u8g2.print("18");

}
void canshuUI2(){
    u8g2.setCursor(0, 10);
    u8g2.print("②");
    
    u8g2.setCursor( 3,25);
    u8g2.print("上UP");     //  拨盘按钮
    u8g2.setCursor( 3,34);
    u8g2.print(Button_U_PIN);

    u8g2.setCursor(45,25);
    u8g2.print("下DWN");     //  拨盘按钮
    u8g2.setCursor(45,34);
    u8g2.print(Button_D_PIN);

    u8g2.setCursor(90,25);
    u8g2.print("中间");     //  拨盘按钮
    u8g2.setCursor(90,34);
    u8g2.print(Button_MID_PIN);

    u8g2.setCursor( 3,49);
    u8g2.print("XXX");
    u8g2.setCursor( 3,59);
    //u8g2.print();

    u8g2.setCursor(45,49);
    u8g2.print("按钮");         // 板载单按钮
    u8g2.setCursor(45,59);
    u8g2.print(Button_key1);

    u8g2.setCursor(90,49);
    u8g2.print("自定义");
    u8g2.setCursor(90,59);
    //u8g2.print();

}
///////////////////////////////////////////
void OLEDcaidan() {       //菜单界面
  u8g2.setCursor(42,10);
  u8g2.print("菜单儿~");

  if(xuni_key6 == 0){//ID查询位 只运行一次
    ESP32_ID();   //型号数据
    xuni_key6 = 1;//ID查询 停止位
  }

  //滚动文字///////芯片型号/////////滚动文字
    static unsigned long lastTime = 0;
    if (millis() - lastTime > gundong_TXT) {
        lastTime = millis();

        ID_time_UI++;//滚动X速度增加
    }

    if(ID_time_UI > 0){         //滚动速度启动位+滚动时间 数值越大越慢
      ID_time_X_UI++;           //滚动增加
      if(ID_time_X_UI >= 240){  //滚动X限制位
        ID_time_X_UI = -240;    //滚动X重置位
      }
      
      ID_time_UI = 0;//滚动停止位
    }

    int IDdata1 = map( ID_time_X_UI, -240, 0, 0, 240);
    u8g2.setCursor( IDdata1, 28); //35
    u8g2.print(part_XH1);//显示ID

    int IDdata2 = map( ID_time_X_UI, 0, 240, -240, 0);
    u8g2.setCursor( IDdata2, 28); //35
    u8g2.print(part_XH1);//显示ID

    u8g2.setCursor( ID_time_X_UI, 28);  //35
    u8g2.print(part_XH1);//显示ID
  
  ///////////////////////////////闪烁数值
    ///////////////////////////////设置
      u8g2.setCursor(4,60);
      if(data11 == 0 && data21 == 0 && data10 == 1){
        static unsigned long lastTime1 = 0;
        if (millis() - lastTime1 > shezhi_TXT) {
          lastTime1 = millis();
          OLED_UI_time1 = !OLED_UI_time1;
        }

        if(OLED_UI_time1 == 0){
          u8g2.print("<设置>");
        }
        else{
          u8g2.print(" 设置 ");
        }
      }
      else{
        u8g2.print("<设置>");
      }
    
    ///////////////////////////////桌面
      u8g2.setCursor(48,60);
      if(data11 == 0 && data21 == 0 && data10 == 2){
        static unsigned long lastTime2 = 0;
        if (millis() - lastTime2 > zhuomian_TXT) {
          lastTime2 = millis();
          OLED_UI_time3 = !OLED_UI_time3;
        }

        if(OLED_UI_time3 == 0){
          u8g2.print("<桌面>");
        }
        else{
          u8g2.print(" 桌面 ");
        }
      }
      else{
        u8g2.print("<桌面>");
      }
    ///////////////////////////////数据
      u8g2.setCursor(90,60);
      if(data11 == 0 && data21 == 0 && data10 == 3){
        static unsigned long lastTime3 = 0;
        if (millis() - lastTime3 > shuju_TXT) {
          lastTime3 = millis();
          OLED_UI_time5 = !OLED_UI_time5;
        }

        if(OLED_UI_time5 == 0){
          u8g2.print("<数据>");
        }
        else{
          u8g2.print(" 数据 ");
        }
      }
      else{
        u8g2.print("<数据>");
      }

  if (button[1].clicks == 1 || button[2].clicks == 1)
  {
    if(data10 > 3 || data20 > 3){//光标选择高限
      data10 = 3;
      data20 = 3;
    }
    else if(data10 < 1 || data20 < 1){//光标选择低限
      data10 = 1;
      data20 = 1;
    }
  }
  else{
    if(data10 > 3 || data20 > 3){//光标选择高限
      data10 = 1;
      data20 = 1;
    }
    else if(data10 < 1 || data20 < 1){//光标选择低限
      data10 = 3;
      data20 = 3;
    }
  }
}

void GuangBiao(){   //光标
  if(data11 == 0){
    u8g2.drawRFrame(data1[data10] , 48, 40, 15, 5);
  }
  else if (data11 == 1 && data10 == 1){//光标位置选择限制
    if (data12 >= 3){//光标Y限制
      data13 = 48;
      data23 = 1;
      data24 = map( data12, 3, 5, 0, 2);
    }
    else{
      data13 = 24;
      data23 = 0;
      data24 = data12;
    }
    u8g2.drawRFrame(data2[data23][data24] , data13, 34, 16, 5);//光标
  }
}


///////////////////////////////////////////光标 限制 刷新
void U_a_button(){
        //Serial.println("顺时针");//调试用
        
        if (data21 == 0){//一级菜单光标
          data20++ ;
          data10 = data20;// - 1;//做减一 是防止编码两次跳动外加数值转换
        }
        else if(data21 == 1 && data20 == 1){//二级菜单光标
          data22++;
          data12 = data22;// - 1;
        }

  /////////////
  else if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件
    } 
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
    
    
  }
}
void D_b_button(){
  //Serial.println("逆时针旋转");//调试用
  
  if (data21 == 0){//一级菜单光标
      data20-- ;
      data10 = data20;// + 1;//加一 防止编码两次跳动外加数值转换
  }
  else if(data21 == 1 && data20 == 1){//二级菜单光标
      data22--;
      data12 = data22;// + 1;
  }

  /////////////
  else if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件
    }
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
  }
}
void KEY_button(){  //单按钮
        if (data21 == 0){//一级菜单光标
          data20++ ;
          data10 = data20;
        }
        else if(data21 == 1 && data20 == 1){//二级菜单光标
          data22++;
          data12 = data22;
        }
  /////////////
  else if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件
    } 
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
  }
}
void KEY_button_Direction(){  //单按钮 光标方向
        if (data21 == 0){//一级菜单光标
          data20--;
          data10 = data20;
        }
        if(data21 == 1 && data20 == 1){//二级菜单光标
          data22--;
          data12 = data22;// - 1;
        }
  /////////////
  else if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件
    } 
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
    
    
  }
}

///////////////// 长按
void U_L_button(){
  if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件
      
      B++;    /// //设置RGB亮度
      if (B > 255)
      {
          B = 255;
      }
      else if (B < 0)
      {
          B = 0; 
      }
      pixels2.setBrightness(B);/// //设置RGB亮度
      
    } 
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
  }
}
void D_L_button(){
  if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
    if (WINDOW_UI == 0){//桌面1
      //在这儿加条件

      B--;/// //设置RGB亮度
      if (B > 255)
      {
          B = 255;
      }
      else if (B < 0)
      {
          B = 0; 
      }
      pixels2.setBrightness(B);/// //设置RGB亮度
    }
    else if (WINDOW_UI == 1){//
    }
    else if (WINDOW_UI == 2){//
    }
    else if (WINDOW_UI == 3){//
    }
    else if (WINDOW_UI == 4){//
    }
  }
}

///////////////////////////////////////////二级菜单页面
bool play = false;
  void shezhiUI(){    //设置页UI
    u8g2.clearBuffer(); //清屏

    u8g2.setCursor(46,10);
    u8g2.print("设置UI");
    /////////////////////////灯
      u8g2.setCursor( 3, 22);
      u8g2.print("RGB板");
      u8g2.drawRFrame(3, 25, 30, 13, 6); 
      u8g2.drawDisc( data4[0][0][0], 31, 5);                //X:10~25 //半圆
      //digitalWrite(led, data3[0][0]);           //引脚输出

    //
    /////////////////////////NumberKey 只有X改变位置
      u8g2.setCursor(40, 22);
      u8g2.print("RGB圈");
      u8g2.drawRFrame( 45, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][1], 31, 5);                //X:97~
      //digitalWrite(Relay1_IO, data3[0][1]);

      u8g2.setCursor(83, 22);
      u8g2.print("RGB圈白");
      u8g2.drawRFrame( 90, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][2], 31, 5);                //X:52~
      //digitalWrite(Relay2_IO, data3[0][2]);
      if (data3[0][2] == 1)
      {//当数字按钮2被按下时，执行以下代码
        if (xuni_key2 == 0)
        {
          data3[0][1] = 0;
          xuni_key2 = 1;
        }
        xuni_key1 = 0;
      }
      else{
        if (xuni_key1 == 0 && data3[0][1] == 0)
        {
          pixels2.fill(pixels2.Color(0, 0, 0), 0);
          pixels2.show();
          
          xuni_key1 = 1;
        }
        xuni_key2 = 0;
      }
      

      u8g2.setCursor(3,48);
      u8g2.print("DHT选");
      u8g2.drawRFrame( 3, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][0], 55, 5);                //X:10~25
      //digitalWrite(Relay3_IO, data3[1][0]);

      u8g2.setCursor(40,48);
      u8g2.print("自定义");
      u8g2.drawRFrame(45, 49, 30, 13, 6); 
      u8g2.drawDisc( data4[0][1][1], 55, 5);                //X:10~25 //半圆
      if(data3[1][1] == 1){
        digitalWrite(I2S_ON_OFF_IO , HIGH);//蜂鸣 I2S
          
      }
      else {
        
      }

      u8g2.setCursor(88,48);
      u8g2.print("蜂鸣静");
      u8g2.drawRFrame( 90, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][2], 55, 5);                //X:52~

    //可在一下判断条件中，添加你想要的相应的代码功能
    /////////////////////////数字按键输出 设置
      if (data3[0][0] == 1){//数字按钮1   
        data4[0][0][0] = 25;
      }
      else{
        data4[0][0][0] = 10;

        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
      }

      if(data3[0][1] == 1){//数字按钮2   
        data4[0][0][1] = 67;

        data3[0][2] = 0;
        xuni_key1 = 0;
      }
      else{
        data4[0][0][1] = 52;

        if (xuni_key1 == 0 && data3[0][2] == 0)
        {
          pixels2.fill(pixels2.Color(0, 0, 0), 0);
          pixels2.show();
          
          xuni_key1 = 1;
        }
        
        
      }

      if(data3[0][2] == 1){//数字按钮3   
        data4[0][0][2] = 112;
      }
      else{
        data4[0][0][2] = 97;
      }

      if(data3[1][0] == 1){//数字按钮4  
        data4[0][1][0] = 25;

        DHT_xuan = 1;
      }
      else {
        data4[0][1][0] = 10; 
        
        DHT_xuan = 0;
      }

      if(data3[1][1] == 1){//数字按钮5    
          data4[0][1][1] = 67;
      }
      else{
        data4[0][1][1] = 52;
      }

      if(data3[1][2] == 1){//数字按钮6  
        data4[0][1][2] = 112;
      }
      else{
        data4[0][1][2] = 97;
      }

    //限制
      if (button[1].clicks == 1 || button[2].clicks == 1)
      {
        if (data12 > 5 && data22 > 5){//设定二级光标上限（二级数字按键数量）
        data12 = 5;
        data22 = 5;
        }
        else if (data12 < 0 && data22 < 0){//设定二级光标下限
          data12 = 0;
          data22 = 0;
        }
      }
      else{
        if (data12 > 5 && data22 > 5){//设定二级光标上限（二级数字按键数量）
          data12 = 0;
          data22 = 0;
        }
        else if (data12 < 0 && data22 < 0){//设定二级光标下限
          data12 = 5;
          data22 = 5;
        }
      }

      if(data12 >= 3){//数字按键翻转的位置按钮选择限制
        data2[1][3] = 1;//第一行的数字按键
      }
      else{
        data2[1][3] = 0;//第二行的数字按键
      }
  }
  void zhuomianUI(){  //桌面页UI
    u8g2.setCursor(46,10);
    u8g2.print("桌面UI");
    
    //serial_print1();  //串口打印

    if (WINDOW_UI == 0){//桌面1
      zhuomianUI1();
    } 
    else if (WINDOW_UI == 1){//
      zhuomianUI2();
    }
    else if (WINDOW_UI == 2){//未使用
      zhuomianUI3();
    }
    else if (WINDOW_UI == 3){//
      zhuomianUI4();
    }
    else if (WINDOW_UI == 4){//
      zhuomianUI5();
    }
    
    if (WINDOW_UI > 1){//ui限制
      WINDOW_UI = 0;
    }
  }
  void canshuUI(){    //参数页UI 主要用作显示GPIO接口引脚
    u8g2.clearBuffer();

    u8g2.setCursor(46,10);
    u8g2.print("数据UI");
    //可添加多个画面，设置多个参数显示
    if(DATA_UI == 0){//参数1
      canshuUI1();
    }
    else if(DATA_UI == 1){//参数2
      canshuUI2();
    }
    
    if(DATA_UI > 1){//ui限制
      DATA_UI = 0;
    }
  }


void caidanshuzhi(){//菜单数值
  /////////////////////////////////////////菜单数值
    if (data11 == 0 && data21 == 0){//菜单页
      OLEDcaidan();
    }
    else if(data11 == 1 && data21 == 1 && data10 == 1){//设置
      shezhiUI();
      if(data11 > 1 && data21 > 1){
        data11 = 0;
        data21 = 0;
      }
    }
    else if(data11 == 1 && data21 == 1 && data10 == 2){//桌面
      zhuomianUI();
      if(data11 > 1 && data21 > 1){
        data11 = 0;
        data21 = 0;
      }
    }
    else if(data11 == 1 && data21 == 1 && data10 == 3){//参数
      canshuUI();
      if(data11 > 1 && data21 > 1){
        data11 = 0;
        data21 = 0;
      }
    }
    else{
      data11 = 0;
      data21 = 0;
    }
}


///////////////////////////////////////////
void shuaxin(){//刷新
      u8g2.clearBuffer(); //清屏
      u8g2.firstPage();
      caidanshuzhi();     //菜单数值 限制值
      GuangBiao ();       //光标值
      //可在这儿加显示页面或者条件
      u8g2.nextPage();
      //u8g2.sendBuffer();  //屏幕刷新作用

}

///////////////////////////////////////////////////////
void xuan (){//选择按键
  //////////////////////////////////按钮及拨码刷新
    button[0].Update();           //按钮
    button[1].Update();           //MID
    button[2].Update();           //U
    button[3].Update();           //D
  //////////////////////////////////按键数值映射
    if (button[0].clicks == 1 || button[1].clicks == 1){  //单击 菜单进退 //clicks次数 
      data11++ ;//data11
      data21++ ;//data21
    }
    else if (button[0].clicks == 3 || button[1].clicks == 3) {//双按钮双击 单按钮三击 翻转及切换UI
      if (data11 == 1 && data10 == 1){//数字按键翻转
        data3[data23][data24] = !data3[data23][data24];//翻转
      }
      
      if (data11 == 1 && data10 == 2){//桌面页面 _UI切换
        WINDOW_UI++;
      }
      else if (data11 == 1 && data10 == 3){//参数页面 DATA_UI切换
        DATA_UI++;
      }
    }
    else if (button[0].clicks == -1){ //长按  进退菜单
      data11++ ;//data11菜单
      data21++ ;//data21
    }
    else if (button[0].clicks == 2){ //双击 桌面页面 _UI切换
      if (data11 == 1 && data10 == 2){//桌面页面 _UI切换
        WINDOW_UI++;
      }
    }
    
    if (button[2].clicks == 1)   //短按一次 U
    {
      U_a_button(); //光标值刷新
    }
    if (button[3].clicks == 1)   //短按一次 D
    {
      D_b_button(); //光标值刷新
    }
    ///////////////
    if (button[2].clicks == -1)       //长按 U
    {
      U_zhuanhuan = true;
    }
    else if (button[3].clicks == -1)  //长按 D
    {
      D_zhuanhuan = true;
    }

    if (button[2].depressed == 0 && button[3].depressed == 0)
    {
      U_zhuanhuan = false;
      D_zhuanhuan = false;
    }
    
    if (U_zhuanhuan == true)
    {
      U_L_button();
      //U_zhuanhuan = false;
    }
    else if (D_zhuanhuan == true)
    {
      D_L_button();
      //D_zhuanhuan = false;
    }
    
    
    //////////////////////////////////////////////////
    if (button[0].clicks == 2) {//// 单按钮
      KEY_button(); //光标值刷新
    }
    else if (button[0].clicks == -2) {//// 单按钮 光标方向
      KEY_button_Direction(); //光标值刷新
    }
    //////////////////////////////////////////////////
    if(data3[1][2] == 0){//静音
      //digitalWrite(Buzzer_IO , HIGH);//蜂鸣器

      //digitalWrite(Buzzer_IO , LOW);//蜂鸣 I2S
      digitalWrite(I2S_ON_OFF_IO , LOW);//蜂鸣 I2S
    }

    static unsigned long lastTime4 = 0;
    if(data3[1][2] == 1 && digitalRead(Buzzer_IO) == HIGH){
      if (millis() - lastTime4 > fengming_TXT) { // 
          //digitalWrite(Buzzer_IO, LOW);
          digitalWrite(I2S_ON_OFF_IO, LOW);
          noTone(Buzzer_IO);
      }
    }

    //按键音 可静音
    if((button[0].clicks == 1 || button[0].clicks == 2 || button[0].clicks == -1 || button[1].clicks == 1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[1].clicks == 2) && data3[1][2] == 1 ){//按键音
      //digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
      //digitalWrite(Buzzer_IO , HIGH);//蜂鸣器
      digitalWrite(I2S_ON_OFF_IO , HIGH);//蜂鸣 I2S
      tone(Buzzer_IO, 440, 40);
      
      tone(Buzzer_IO, 1046, 40);
      lastTime4 = millis();
    }
}

///////////////////////////////////////////////////////
/*
void MONI(){//模拟量 测试屏幕位置或转换数据作用，可在loop启用
  Servo_DATA = map(analogRead(A_D_1), 0, 4096, 0, 180); // A/D转换
}
*/

void setup() {
  Serial.begin(115200);

  u8g2.begin();                             //初始化OLED
  u8g2.enableUTF8Print();                   //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);   //文字大小15
  u8g2.setFlipMode(1);                      //翻转显示

  //setupPWM();     //PWM初始化
  RGB_setup_1();    //RGB初始化
  DHT_setup();      //DHT初始化

  pinMode(     Buzzer_IO, OUTPUT);           //蜂鸣 I2S
  pinMode( I2S_ON_OFF_IO, OUTPUT);           //I2S
  pinMode(   I2S_BCLK_IO, OUTPUT);           //I2S 时钟
  pinMode(Button_MID_PIN, INPUT_PULLUP);     //MID
  pinMode(  Button_U_PIN, INPUT_PULLUP);     //U
  pinMode(  Button_D_PIN, INPUT_PULLUP);     //D
  pinMode(   Button_key1, INPUT_PULLUP);     //按钮

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//默认文字大小12

  //pinMode(led, OUTPUT);       // 设置引脚为输出模式
  //pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
  //pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
  //pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式

  //pinMode( A_D_1, INPUT);    //模拟量输入

  fastTimer.setInterval(100);             //多任务 //10MS
  //fastTimer1.setInterval(300); // 未使用  //多任务 //10MS

  data3[1][2] = 0;  //蜂鸣器默认关闭

  shuaxin();//执行一次OLED画面刷新

  digitalWrite(I2S_BCLK_IO , LOW);//I2S 时钟  默认关闭NS4168使能

}

void loop() {
  //MONI();     //模拟量，测试及数据转换
  xuan ();      //选择功能

  if (fastTimer.isReady()) {//多任务就绪时执行
      shuaxin();          //刷新
      fastTimer.reset();  //重置多任务定时器
  }

  //在这儿加页面或条件
  if (data3[0][0] == 1){//数字按钮1  
    RGB_loop_1();
  }

  if (data3[0][1] == 1){//数字按钮2  
    RGB_loop_2();
  }

  if (data3[0][2] == 1){//数字按钮3  
    RGB_loop_2_1();
  }
  //if (fastTimer1.isReady()) {//多任务就绪时执行
      //可使用
      //fastTimer1.reset();  //重置多任务定时器
  //}
}
