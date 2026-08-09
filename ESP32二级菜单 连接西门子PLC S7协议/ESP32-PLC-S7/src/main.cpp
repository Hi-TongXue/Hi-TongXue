/*================================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563  ESP32菜单 西门子S7协议 V0.0 ===
  ================================================================================
  //库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    多任务库              SimplifiedTimer.h
    S7库                  Settimino.h
    wifi库                WiFi.h

  //代码及使用说明:
    1 菜单框架更新，使用millis作为延时，可在时间设置区设置延时值
    2 lastTime11已用
    3 连接WIFI  连接PLC  都带有超时连接功能
    4 S7需要可靠连接，所以没有加一键配网，要与固定WIFI连接，后期加入RJ45模块方式ESP32通过LAN连接PLC
    5 蜂鸣器默认关闭
    6 

*/
///////////////////////////////////////////////////////
///////////////////////////时间设置区
  int gundong_TXT     =   1;      //滚动速度
  int shezhi_TXT      =   900;    //设置光标选项 闪烁速度
  int zhuomian_TXT    =   900;    //桌面光标选项 闪烁速度
  int shuju_TXT       =   900;    //数据光标选项 闪烁速度
  int WIFIbiaozhi_TXT =   500;    //WIFI标志 闪烁速度
  int PLC_Q_yanshi_TXT=   0;      //桌面3 读取Q区延时时间
  int fengming_TXT    =   35;     //蜂鸣延时

///////////////////////////定义Library
  #include <U8g2lib.h>                //引用OLED库
  #include "ClickButton.h"            //引用消抖库
  #include <SimplifiedTimer.h>        //多任务库
  #include <Arduino.h>
  #include "PWM_TOMO.h"               //未使用  菜单蜂鸣可需要
  #include "PLC_S7.h"                 //链接PLC
  #include "RGB.h"                    //未使用
  #include "bmp1.h"                   //可用作开机图

///////////////////////////GPIO连接映射
  const int Button_U_PIN = 25;      //U引脚
  const int Button_D_PIN = 33;      //D引脚
  const int Button_R_PIN = 27;      //r引脚
  const int Button_L_PIN = 14;      //l引脚

  int Button_key1 =  26;  //MID按键引脚

  int SDA_U8G2 =  5;        //OLED屏幕引脚
  int SCL_U8G2 = 17;        //OLED屏幕引脚

  ///////////////
  int OLED_GND = 4;        //OLED屏幕引脚
  int OLED_VCC = 16;        //OLED屏幕引脚
  ///////////////

  const int Buzzer_IO = 35;     //蜂鸣器引脚

  int led = 2;            //数字按键灯引脚
  int Relay1_IO = 15;     //数字按键继电1引脚
  //int Relay2_IO = 12;     //数字按键继电2引脚
  //int Relay3_IO = 14;     //数字按键继电3引脚

  //int A_D_1 = 34;         //模拟量电位器引脚
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

  int xuni_key1 = 0;           //已使用 Q1.0 //虚拟按键：代码只循环一次
  int xuni_key2 = 0;           //已使用 Q1.1 //虚拟按键：代码只循环一次
  
  int xuni_key6 = 0;           //已使用 ID查询 //虚拟按键：代码只循环一次

  int begin_dataA;//记录初始值
  int begin_dataB;

  int UP_dataA;//记录当前值
  int UP_dataB;

  int F_data = 0;//蜂鸣数据

  int Servo_DATA;

  int OLED_UI_time1 = 0;//光标移动位置 设置选项卡闪烁
  int OLED_UI_time2 = 0;

  int OLED_UI_time3 = 0;//光标移动位置 桌面选项卡闪烁
  int OLED_UI_time4 = 0;

  int OLED_UI_time5 = 0;//光标移动位置 数据选项卡闪烁
  int OLED_UI_time6 = 0;
  //////////////////////////////////////////////////////////
  int zhuanhuanwifibiaozhi = 0;//wifi检测标志 加闪烁

  int chaoshiPLCCONNECT_biaozhiwei; //PLC连接标志位
  int shezhiPLCIP_biaozhiwei;       //PLCIP设置闪烁显示标志位
  int PlcStartRUN_biaozhiwei;       //plc run模式标志位

  int setip_weizhi = 3;
  int ipweizhi_guangbiao[5] = { 0, 44, 64, 83, 98};

  ////////////////////////芯片ID数据
    char part_XH1[50];      //芯片ID 组合数值
    int ID_time_UI;         //UI滚动
    int ID_time_X_UI = -240;//UI滚动值

///////////////////////////库函数初始化使用
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_U8G2, SDA_U8G2);    //1.3 OLED屏幕

  ClickButton button[5] = {//消抖按键映射
    ClickButton (Button_key1, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_R_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_L_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_U_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_D_PIN, LOW, CLICKBTN_PULLUP),
  };

  SimplifiedTimer fastTimer;    // 多任务 任务1
  SimplifiedTimer fastTimer1;    // 多任务 任务2

///////////////////////////////////////////////////////


//=========串口打印 调试用===========//可在加载页启用
  void serial_print1(){//调试 串口打印
      Serial.println("======== ========");

      Serial.println("=============================================");
  }


void ESP32_ID(){  //芯片型号
  sprintf( part_XH1, "ESP32芯片型号 = %s Rev%d \n", ESP.getChipModel(), ESP.getChipRevision());
  //Serial.println(part_XH1);//调试
  //C语言格式化函数 %s字符串 %d整数
}


void OLEDjiazai(){  //加载页
   
  u8g2.clearBuffer();

  u8g2.setCursor(37,30);
  u8g2.print("启动中...");
  u8g2.sendBuffer();
}

void OLEDkaiji(){   //开机页    可以打印一些串口数据用作调试
  u8g2.clearBuffer();
  u8g2.setCursor(0,27);
  u8g2.print("S7-PLC加载中..");
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
  u8g2.setCursor(0,40);
  u8g2.print("V0.0");
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

  u8g2.clearBuffer();
  u8g2.drawBitmap(32, 0, 8, 64, gImage_CatHead);  //开机图片
  u8g2.sendBuffer();
  delay(1800);
}

///////////////////////////////////////////浮点数线性转换
/*
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/
///////////////////////////////////////////WIFI
void WIFI_START1(){ //WIFI检测并显示状态
  if(WiFi.status() == WL_CONNECTED){
    u8g2.drawStr(108, 10, "^_^");
  }
  else{
    
    if (data3[1][1] == 1)
    {
      static unsigned long lastTime5 = 0;
      if (millis() - lastTime5 > WIFIbiaozhi_TXT) {
          lastTime5 = millis();

          zhuanhuanwifibiaozhi = !zhuanhuanwifibiaozhi; //闪烁作用
      }

      if (zhuanhuanwifibiaozhi == 0)
      {
        u8g2.drawStr(108, 10, "X_X"); //1,36
      }
    } 
  }
}
///////////////////////////////////////////RGB
void RGB_R1(){  //未使用
  //RGB_bright = LD_RGB;
  RGB_control_1();
}
///////////////////////////////////////////PWM
void PWM_LOOP(){//PWM调速运行 //未使用
  //ledcWrite(PWM_CHANNEL, PWM_DATA1);            // 占空比
}
 
///////////////////////////////////////////菜单页链接状态显示
void ESP32_WIFI_PLC_zhuangtai(){  //菜单首页显示 ESP32与PLC连接状态
  //u8g2.drawStr(10, 42, "ESP32--WIFI--PLC"); //1,36
  u8g2.drawStr(10, 42, "ESP32");  //
  if (WiFi.status() == WL_CONNECTED) {
    u8g2.drawStr(43, 42, "--");     //
  }
  u8g2.drawStr(58, 42, "WiFi");   //
  if (client.Connected) {
    u8g2.drawStr(81, 42, "--");     //
  }
  
  u8g2.drawStr(98, 42, "PLC");    //
}
///////////////////////////////////////////桌面UI
  void zhuomianUI1(){ //桌面1 
      u8g2.setCursor(0, 10);
      u8g2.print("①");

      
      u8g2.setCursor(0,25);
      if(WiFi.status() == WL_CONNECTED){  //WIFI连接后0LED显示IP
        u8g2.print("WiFi已接");
        u8g2.print(WiFi.localIP());
      }
      else{
        if(zhuanhuanwifibiaozhi == 1 && data3[1][1] == 1){
          u8g2.print("等待连接WIFI...");
        }
        else{
          u8g2.print("<< WiFi开关未打开 >>");
        }
      }
      
      u8g2.setCursor(0,40); //0,49
      if(client.Connected != false && data3[0][1] == 1){
        u8g2.print("PLC已接");
        u8g2.print(plcIP);
      }
      else{
        if(data3[0][1] == 1){
          u8g2.print("等待PLC...");
        }
        else{
          u8g2.print("<< PLC开关未打开 >>");
        }
      }
      
      u8g2.setCursor(0,62); //
      if (client.Connected == false && data3[0][1] == 0)  //要链接PLC的IP地址  可设置
      {
        u8g2.print("设置");//u8g2.print("    ");
        u8g2.print("I");
        
        static unsigned long lastTime7 = 0;
        if (millis() - lastTime7 > 1000) { //计时闪烁
          lastTime7 = millis();
          shezhiPLCIP_biaozhiwei = !shezhiPLCIP_biaozhiwei;
        }
        if (shezhiPLCIP_biaozhiwei == 1)
        {
          u8g2.print("P ");
        }
        else{
          u8g2.print("  ");
        }
        
        //IP 四段 每段最大255  常见形式192.168.0.100  
        u8g2.print(plcIP[0]);
        u8g2.print(".");
        u8g2.print(plcIP[1]);
        u8g2.print(".");
        u8g2.print(plcIP[2]);
        u8g2.print(".");
        u8g2.print(plcIP[3]);

        
        u8g2.setCursor( ipweizhi_guangbiao[setip_weizhi], 52); //设置IP的光标
        if (shezhiPLCIP_biaozhiwei == 1)
        {
          u8g2.print("↓");
          //u8g2.drawStr(10, 42, "↓"); //
        }
        else{
          u8g2.print(" ");
        }

        if (setip_weizhi < 1) //限制设置IP的光标
        {
          setip_weizhi = 1;
        }
        else if (setip_weizhi > 4)
        {
          setip_weizhi = 4;
        }
      }
  }
  void zhuomianUI2(){ //桌面2 
    u8g2.setCursor(0, 10);
    u8g2.print("②");

    if (data3[0][1] == 1)
    {
      s7_plc_vw100(); //vw100读/写
      s7_plc_vw200(); //VW200读/写
    }
    
      u8g2.setCursor( 2, 38);
      u8g2.print("VW100: ");
      u8g2.setCursor(10, 52);
      u8g2.print(data_VW100);   //PLC: V区 VW100

      u8g2.setCursor(62, 38);
      u8g2.print("VW200: ");
      u8g2.setCursor(72, 52);
      u8g2.print(data_VW200);   //PLC: V区 VW200

  }
  void zhuomianUI3(){ //桌面3 
    u8g2.setCursor(0, 10);
    u8g2.print("③");

    if (data3[0][1] == 1)
    {
      static unsigned long lastTime4 = 0;
      if (millis() - lastTime4 > PLC_Q_yanshi_TXT) { //读取Q区延时时间 初始化为0  可在时间设置区设置
          lastTime4 = millis();
          s7_plc_Q10_R() , s7_plc_Q11_R() , s7_plc_Q12_R() , s7_plc_Q13_R();  //PLC: Q区
      }
    }

      u8g2.setCursor( 2, 38);
      u8g2.print("Q1.0");
      u8g2.setCursor( 8, 52);
      if (bufferQ10_R == 1) //判断Q区 线圈
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }

      u8g2.setCursor( 33, 38);
      u8g2.print("Q1.1");
      u8g2.setCursor(39, 52);
      if (bufferQ11_R == 1)
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }

      u8g2.setCursor(63, 38);
      u8g2.print("Q1.2");
      u8g2.setCursor(69, 52);
      if (bufferQ12_R == 1)
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }
  
      u8g2.setCursor(94, 38);
      u8g2.print("Q1.3");
      u8g2.setCursor(100, 52);
      if (bufferQ13_R == 1)
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }
  }
  void zhuomianUI4(){ //桌面4 
    u8g2.setCursor(0, 10);
    u8g2.print("④");
    s7_plc_I10_R() , s7_plc_I11_R();  //PLC: I区读取  （只读）

    u8g2.setCursor( 2, 52);
    u8g2.print("I1.0: ");
    if (bufferI10_R == 1)       //I1.0
    {
      u8g2.print("已按");     //
    }
    else{
      u8g2.print("已松");     //
    }
    
    u8g2.setCursor(62, 52);
    u8g2.print("I1.1: ");
    if (bufferI11_R == 1)       //I1.1
    {
      u8g2.print("已按");     //
    }
    else{
      u8g2.print("已松");     //
    }
  }
  void zhuomianUI5(){ //桌面5 
      u8g2.setCursor(0, 10);
      u8g2.print("⑤");
      if (data3[0][1] == 1)
      {
        s7_plc_Q02_R() , s7_plc_Q03_R();  //读Q区  可用作反向控制ESP32 IO引脚电平控制
      }
      
      u8g2.setCursor( 3, 38);
      u8g2.print("IO");
      u8g2.print(led);
      u8g2.setCursor( 8, 52);
      if (bufferQ02_R == 1) //判断Q区 线圈   //反向控制ESP32 IO引脚电平控制
      {
        u8g2.print("●");
        digitalWrite(led, HIGH);
      }
      else{
        u8g2.print("○");
        digitalWrite(led, LOW);
      }

      u8g2.setCursor( 33, 38);
      u8g2.print("IO");
      u8g2.print(Relay1_IO);
      u8g2.setCursor(39, 52);
      if (bufferQ03_R == 1)
      {
        u8g2.print("●");
        digitalWrite(Relay1_IO, HIGH);
      }
      else{
        u8g2.print("○");
        digitalWrite(Relay1_IO, LOW);
      }

      u8g2.setCursor(63, 38);
      u8g2.print("可定义");
      u8g2.setCursor(69, 52);
      if (0 == 1)             //自定义功能
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }
  
      u8g2.setCursor(95, 38);
      u8g2.print("可定义");
      u8g2.setCursor(100, 52);
      if (0 == 1)
      {
        u8g2.print("●");
      }
      else{
        u8g2.print("○");
      }
  }
///////////////////////////////////////////参数UI
void canshuUI1(){
    u8g2.setCursor(0, 10);
    u8g2.print("①");
    
    u8g2.setCursor( 3,25);
    u8g2.print("led");
    u8g2.setCursor( 3,34);
    u8g2.print(led);

    u8g2.setCursor(45,25);
    u8g2.print("SCL");
    u8g2.setCursor(45,34);
    u8g2.print(SCL_U8G2);

    u8g2.setCursor(90,25);
    u8g2.print("SDA");
    u8g2.setCursor(90,34);
    u8g2.print(SDA_U8G2);
    ////
    u8g2.setCursor( 3,49);
    u8g2.print("D数字1");
    u8g2.setCursor( 3,59);
    u8g2.print(Relay1_IO);

    u8g2.setCursor(45,49);
    u8g2.print("自定义");
    u8g2.setCursor(45,59);
    //u8g2.print();

    u8g2.setCursor(90,49);
    u8g2.print("PWM");
    u8g2.setCursor(90,59);
    u8g2.print(PWM_PIN);

}
void canshuUI2(){
    u8g2.setCursor(0, 10);
    u8g2.print("②");
    
    u8g2.setCursor( 3,25);
    u8g2.print("上UP");
    u8g2.setCursor( 3,34);
    u8g2.print(Button_U_PIN);

    u8g2.setCursor(45,25);
    u8g2.print("下DWN");
    u8g2.setCursor(45,34);
    u8g2.print(Button_D_PIN);

    u8g2.setCursor(90,25);
    u8g2.print("左L");
    u8g2.setCursor(90,34);
    u8g2.print(Button_L_PIN);

    u8g2.setCursor( 3,49);
    u8g2.print("右");
    u8g2.setCursor( 3,59);
    u8g2.print(Button_R_PIN);

    u8g2.setCursor(45,49);
    u8g2.print("中间");
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
    if (millis() - lastTime > gundong_TXT) {  //延时 可在时间设置区设置
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
        if (millis() - lastTime1 > 900) {
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

  if(data10 > 3 || data20 > 3){//光标选择高限
    data10 = 3;
    data20 = 3;
  }
  else if(data10 < 1 || data20 < 1){//光标选择低限
    data10 = 1;
    data20 = 1;
  }

  ESP32_WIFI_PLC_zhuangtai(); //菜单首页 状态显示
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
      if (setip_weizhi == 1)  //IP设置  开始设置默认第三段  （常用）
      {
        plcIP[0]++;
      }
      else if (setip_weizhi == 2)
      {
        plcIP[1]++;
      }
      else if (setip_weizhi == 3)
      {
        plcIP[2]++;
      }
      else if (setip_weizhi == 4)
      {
        plcIP[3]++;
      }
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
      if (setip_weizhi == 1)    //IP设置  开始设置默认第三段  （常用）
      {
        plcIP[0]--;
      }
      else if (setip_weizhi == 2)
      {
        plcIP[1]--;
      }
      else if (setip_weizhi == 3)
      {
        plcIP[2]--;
      }
      else if (setip_weizhi == 4)
      {
        plcIP[3]--;
      }
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
  void shezhiUI(){    //设置页UI
    u8g2.clearBuffer(); //清屏

    u8g2.setCursor(46,10);
    u8g2.print("设置UI");
    /////////////////////////灯
      u8g2.setCursor( 3, 22);
      u8g2.print("Q0.0");
      u8g2.drawRFrame(3, 25, 30, 13, 6); 
      u8g2.drawDisc( data4[0][0][0], 31, 5);                //X:10~25 //半圆
      //digitalWrite(led, data3[0][0]);           //引脚输出

    //
    /////////////////////////NumberKey 只有X改变位置
      u8g2.setCursor(40, 22);
      u8g2.print("PLC连接");
      u8g2.drawRFrame( 45, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][1], 31, 5);                //X:97~
      //digitalWrite(Relay1_IO, data3[0][1]);

      u8g2.setCursor(86, 22);
      u8g2.print("PLC启动");
      u8g2.drawRFrame( 90, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][2], 31, 5);                //X:52~
      //digitalWrite(Relay2_IO, data3[0][2]);
      if (data3[0][2] == 1)
      {//当数字按钮2被按下时，执行以下代码
        
      }
      

      u8g2.setCursor(3,48);
      u8g2.print("Q0.1");
      u8g2.drawRFrame( 3, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][0], 55, 5);                //X:10~25
      //digitalWrite(Relay3_IO, data3[1][0]);

      u8g2.setCursor(40,48);
      u8g2.print("WiFi连接");
      u8g2.drawRFrame(45, 49, 30, 13, 6); 
      u8g2.drawDisc( data4[0][1][1], 55, 5);                //X:10~25 //半圆
      if(data3[1][1] == 1){
        wifi_CONNECT();   //打开WIFI连接
        if (WiFi.status() != WL_CONNECTED) {//循环检测WIFI是否连接
          static unsigned long lastTime9 = 0;
          if (millis() - lastTime9 > 40000) { //约30s 超时关闭连接WIFI开关
              lastTime9 = millis();
              data3[1][1] = 0;
          }
        }
      }
      else {
        WiFi.disconnect();  //关闭WIFI连接
        wificonnect_biaozhi = 0;
      }

      u8g2.setCursor(88,48);
      u8g2.print("蜂鸣静");
      u8g2.drawRFrame( 90, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][2], 55, 5);                //X:52~

    //可在一下判断条件中，添加你想要的相应的代码功能
    /////////////////////////数字按键输出 设置
      if (data3[0][0] == 1){//数字按钮1   Q1.0
        data4[0][0][0] = 25;

        if (data3[0][1] == 1 && client.Connected == true)
        {
          bufferQ00_W = 1;
          s7_plc_Q00();
        }
        else{
          data3[0][0] = 0;
        }
        xuni_key1 = 0;
      }
      else{
        data4[0][0][0] = 10;

        if (data3[0][1] == 1 && client.Connected == true && xuni_key1 == 0)
        {
          bufferQ00_W = 0;
          s7_plc_Q00();
          xuni_key1 = 1;
        }
      }

      if(data3[0][1] == 1){//数字按钮2    PLC连接
        data4[0][0][1] = 67;
      }
      else{
        data4[0][0][1] = 52;
      }

      if(data3[0][2] == 1){//数字按钮3    PLC启动
        data4[0][0][2] = 112;

        if(data3[0][1] == 1){//数字按钮1    
          
          client.PlcStart();  //PLC RUN模式   单次启动，自复位式
          static unsigned long lastTime8 = 0;
          if (millis() - lastTime8 > 1000) { //
              lastTime8 = millis();
              PlcStartRUN_biaozhiwei = !PlcStartRUN_biaozhiwei;
          }
          
          if (PlcStartRUN_biaozhiwei == 1)
          { 
            data3[0][2] = 0;
            PlcStartRUN_biaozhiwei = 0;
          }
        }
        else{
          data3[0][2] = 0;
        }
      }
      else{
        data4[0][0][2] = 97;
      }

      if(data3[1][0] == 1){//数字按钮4    Q1.1
        data4[0][1][0] = 25;

        if (data3[0][1] == 1 && client.Connected == true)
        {
          bufferQ01_W = 1;
          s7_plc_Q01(); //PLC连接检测
        }
        else{
          data3[0][0] = 0;
        }
        xuni_key2 = 0;
      }
      else {
        data4[0][1][0] = 10; 

        if (data3[0][1] == 1 && client.Connected == true && xuni_key2 == 0)
        {
          bufferQ01_W = 0;
          s7_plc_Q01(); //PLC连接检测
          xuni_key2 = 1;
        }
      }

      if(data3[1][1] == 1){//数字按钮5    WIFI连接
          data4[0][1][1] = 67;
      }
      else{
        data4[0][1][1] = 52;
      }

      if(data3[1][2] == 1){//数字按钮6    未使用，原蜂鸣器开关
        data4[0][1][2] = 112;
      }
      else{
        data4[0][1][2] = 97;
      }

    //限制
      if (data12 > 5 && data22 > 5){//设定二级光标上限（二级数字按键数量）
        data12 = 5;
        data22 = 5;
      }
      else if (data12 < 0 && data22 < 0){//设定二级光标下限
        data12 = 0;
        data22 = 0;
      }

      if(data12 >= 3){//数字按键翻转的位置按钮选择限制
        data2[1][3] = 1;//第一行的数字按键
      }
      else{
        data2[1][3] = 0;//第二行的数字按键
      }

      u8g2.setCursor(0, 10);
      if (chaoshiPLCCONNECT == 0 && data3[0][1] == 1 && !client.Connected)
      {
        u8g2.print("XPLCX");  //PLC准备链接时未连接的状态显示 带 秒超时
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
    else if (WINDOW_UI == 2){//
      zhuomianUI3();
    }
    else if (WINDOW_UI == 3){//
      zhuomianUI4();
    }
    else if (WINDOW_UI == 4){//
      zhuomianUI5();
    }
    
    if (WINDOW_UI > 4){//ui限制  可设置只显示多少页
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
      WIFI_START1();      //显示WIFI状态
      u8g2.nextPage();
      //u8g2.sendBuffer();  //屏幕刷新作用

}

///////////////////////////////////////////////////////
void xuan (){//选择按键
  //////////////////////////////////按钮及拨码刷新
    button[0].Update();           //MID按钮
    button[1].Update();           //KEY1
    button[2].Update();           //KEY2
    button[3].Update();           //U
    button[4].Update();           //D
  //////////////////////////////////按键数值映射
    if (button[0].clicks == 1){  //单击 菜单进退 //clicks次数 
      data11++ ;//data11
      data21++ ;//data21
    }
    else if (button[0].clicks == 2) {//双击 翻转及切换UI
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
    //按键音 可静音
    if((button[0].clicks == 1 || button[0].clicks == 2 || button[0].clicks == -1) && data3[1][2] == 0 ){//按键音
      //digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
    }

    
    if (button[1].clicks == 1) {//
      U_a_button(); //光标值刷新
    }
    
    if (button[2].clicks == 1){//
      D_b_button(); //光标值刷新
    }
    
    
    if (button[3].clicks == 1){//
      setip_weizhi++; //设置IP的光标位置
    }
    
    if (button[4].clicks == 1){//
      setip_weizhi--; //设置IP的光标位置
    }
    

    if(data3[1][2] == 0){//静音
      //digitalWrite(Buzzer_IO , HIGH);//蜂鸣器

      digitalWrite(Buzzer_IO , LOW);//蜂鸣器
    }

    static unsigned long lastTime11 = 0;
    if(data3[1][2] == 1 && digitalRead(Buzzer_IO) == HIGH){
      if (millis() - lastTime11 > fengming_TXT) { // 
          digitalWrite(Buzzer_IO, LOW);
      }
    }

    //按键音 可静音
    if((button[0].clicks == 1 || button[0].clicks == 2 || button[0].clicks == -1 || button[1].clicks == 1 || button[2].clicks == 1 || button[3].clicks == 1 || button[4].clicks == 1 || button[1].clicks == 2) && data3[1][2] == 1 ){//按键音
      //digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
      digitalWrite(Buzzer_IO , HIGH);//蜂鸣器
      lastTime11 = millis();
    }
}

///////////////////////////////////////////////////////PLC运行控制所需要
void PLC_RUN_xuyao(){ //PLC连接的必要运行代码
  if(data3[0][1] == 1){
    if (data3[1][1] == 1)
    {
      PLC_CONNECT();  //PLC连接以及检测
    }
    else{
      data3[0][1] = 0;
    }

    if (chaoshiPLCCONNECT == 1)
    {
      data3[0][1] = 0;
      chaoshiPLCCONNECT = 0;
    }

    if (data3[1][1] == 1)
    {
      static unsigned long lastTime10 = 0;
      if (millis() - lastTime10 > 1000) {
          lastTime10 = millis();
          S7_loop();  //PLC运行 读可写操作，保持PLC连接
      }
    }
    else{
      data3[0][1] = 0;
    }
  }
  else{
    client.Disconnect();//关闭PLC连接
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

  //////////////////////////////////OLED引脚供电，可删除
  pinMode(OLED_GND, OUTPUT);  //
  pinMode(OLED_VCC, OUTPUT);  //
  digitalWrite(OLED_GND, LOW);
  digitalWrite(OLED_VCC, HIGH);
  //////////////////////////////////

  u8g2.begin();                             //初始化OLED
  u8g2.enableUTF8Print();                   //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);   //文字大小15
  u8g2.setFlipMode(1);                      //翻转显示

  setupPWM();     //PWM初始化  未使用

  pinMode(Buzzer_IO, OUTPUT);           //蜂鸣器
  pinMode(Button_R_PIN, INPUT_PULLUP);  //KEY1
  pinMode(Button_L_PIN, INPUT_PULLUP);  //KEY2
  pinMode(Button_U_PIN, INPUT_PULLUP);     //U
  pinMode(Button_D_PIN, INPUT_PULLUP);     //D
  pinMode( Button_key1, INPUT_PULLUP);     //MID按钮

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//文字大小12

  RGB_RUN1(); //RGB初始化

  pinMode(led, OUTPUT);       // 设置引脚为输出模式
  pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
  //pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
  //pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式

  //pinMode( A_D_1, INPUT);    //模拟量输入  未使用

  fastTimer.setInterval(100);             //多任务 //10MS
  //fastTimer1.setInterval(300); // 未使用  //多任务 //10MS  未使用，可启用

  data3[1][1] = 0;  //WIFI连接    初始值
  data3[0][1] = 0;  //PLC连接关闭 初始值
  data3[1][2] = 0;  //蜂鸣器默认关闭
  
  shuaxin();//执行一次OLED画面刷新
}
void loop() {
  //MONI();     //模拟量，测试及数据转换
  xuan ();      //选择功能
  //RGB_R1();     //RGB运行  未使用

  if (fastTimer.isReady()) {//多任务就绪时执行
      shuaxin();          //刷新
      fastTimer.reset();  //重置多任务定时器
  }

  PLC_RUN_xuyao();   //PLC连接的必要运行代码
  
  //if (fastTimer1.isReady()) {//多任务就绪时执行
      //
      //fastTimer1.reset();  //重置多任务定时器
  //}
}




