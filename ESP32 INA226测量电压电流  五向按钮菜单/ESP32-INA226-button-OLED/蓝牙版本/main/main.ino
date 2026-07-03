/*==========================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563   INA226--五向按钮    ===
  ==========================================================================
  //库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    多任务库              SimplifiedTimer.h

  //代码及使用说明:
    1 使用INA226模块检测电压电流
    2 PWM调速用于查看实际电流变化，硬件通过MOS管调节
    3 添加了蓝牙功能，点灯科技APP可查看电压电流功率
    4 

*/
///////////////////////////////////////////////////////

///////////////////////////定义Library
  #include <U8g2lib.h>                //引用OLED库
  #include "ClickButton.h"            //引用消抖库
  #include <SimplifiedTimer.h>        //多任务库
  #include <Arduino.h>
  #include "INA226AV.h"
  #include "PWM_TOMO.h"
  #include "ble_.h"
///////////////////////////GPIO连接映射
  const int Button_L_PIN = 27;      //L引ESP脚
  const int Button_R_PIN = 32;      //R引脚
  const int Button_U_PIN = 25;      //U引脚
  const int Button_D_PIN = 33;      //D引脚

  int Button_key1 =  26;  //MID按键引脚

  int SDA_U8G2 =  5;        //OLED屏幕引脚
  int SCL_U8G2 = 17;        //OLED屏幕引脚

  ///////////////
  int OLED_GND = 4;        //OLED屏幕引脚
  int OLED_VCC = 16;        //OLED屏幕引脚
  ///////////////

  const int Buzzer_IO = 35;     //蜂鸣器引脚

  int led = 2;            //数字按键灯引脚
  int Relay1_IO = 13;     //数字按键继电1引脚
  int Relay2_IO = 12;     //数字按键继电2引脚
  //int Relay3_IO = 14;     //数字按键继电3引脚

  int A_D_1 = 34;         //模拟量电位器引脚
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

  int xuni_key1 = 0;           //虚拟按键1：代码只循环一次
  int xuni_key2 = 0;           //虚拟按键2：代码只循环一次
  int xuni_key3 = 0;           // ：代码只循环一次

  int xuni_key4 = 0;           //：代码只循环一次
  int xuni_key5 = 0;           //：代码只循环一次
  int xuni_key6 = 0;           //：代码只循环一次
  int xuni_key7 = 0;           //：代码只循环一次
  int xuni_key8 = 0;           //：代码只循环一次

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

  ////////////////////////芯片ID数据
    char part_XH1[50];      //芯片ID 组合数值
    int ID_time_UI;         //UI滚动
    int ID_time_X_UI = -240;//UI滚动值
  
  ////////////////////////PWM调速
    //int PWM_DATA1 = 0;;//PWM数据

///////////////////////////库函数初始化使用
  //U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_U8G2, SDA_U8G2);
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_U8G2, SDA_U8G2);    //1.3 OLED屏幕

  ClickButton button[5] = {//消抖按键映射
    ClickButton (Button_key1, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_L_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_R_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_U_PIN, LOW, CLICKBTN_PULLUP),
    ClickButton (Button_D_PIN, LOW, CLICKBTN_PULLUP),
  };

  SimplifiedTimer fastTimer;    // 多任务 任务1
  SimplifiedTimer fastTimer1;    // 多任务 任务2

///////////////////////////////////////////////////////


//=========串口打印 调试用===========//可在加载页启用
  void serial_print1(){//调试 串口打印
      Serial.println("======== 电压 & 电流 & 功率 & 分流分压========");

      Serial.print("总线电压："); Serial.print(INA226_data.voltage, 2); Serial.println(" V");
      Serial.print("负载电流："); Serial.print(INA226_data.current, 4); Serial.println(" A");
      Serial.print("负载功率："); Serial.print(INA226_data.power, 3); Serial.println(" W");
      Serial.print("分流电压："); Serial.print(INA226_data.shuntV * 1000, 3); Serial.println(" mV");

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
  u8g2.print("载OLED&INA226..");
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

///////////////////////////////////////////蓝牙
void ble_run(){//PWM调速运行
  TextV.print( INA226_data.voltage);
  TextA.print( INA226_data.current);
  TextW.print( INA226_data.power);
}
///////////////////////////////////////////
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
///////////////////////////////////////////PWM
void PWM_LOOP(){//PWM调速运行
  ledcWrite(PWM_CHANNEL, PWM_DATA1);            // 占空比
}
///////////////////////////////////////////桌面UI
  void zhuomianUI1(){ //桌面1 
      u8g2.setCursor(0, 10);
      u8g2.print("①");

      u8g2.setCursor(0,23);
      u8g2.print("总线电压: ");
      u8g2.print(INA226_data.voltage);// 总线电压(V)
      u8g2.print(" V");

      u8g2.setCursor(0,33);
      u8g2.print("负载电流: ");
      u8g2.print(INA226_data.current);// 负载电流(A)
      u8g2.print(" A");
      
      u8g2.setCursor(0,44);
      u8g2.print("负载功率: ");
      u8g2.print(INA226_data.power);  // 负载功率(W)
      u8g2.print(" W");
      
      u8g2.setCursor(0,55);
      u8g2.print("分流电压: ");
      u8g2.print(INA226_data.shuntV);  // 分流电压(mV)
      u8g2.print(" mV");

  }
  void zhuomianUI2(){ //桌面2 
    u8g2.setCursor(0, 10);
    u8g2.print("②");
    
    PWM_LOOP();//PWM调速运行

    u8g2.setCursor(0,22); //60  
    u8g2.print("PWM:");
    u8g2.print(PWM_DATA1);  // PWM

    u8g2.setCursor(50, 55);
    u8g2.print(INA226_data.current);
    u8g2.print( "A");
    
    u8g2.setCursor(0, 33);
    u8g2.print(INA226_data.voltage);
    u8g2.print( "V");
    
    u8g2.setCursor(95, 33);
    u8g2.print(INA226_data.power);
    u8g2.print( "W");
    
    //u8g2.setFont(u8g2_font_6x10_mf);
    //u8g2.setFont(u8g2_font_4x6_mf);
    u8g2.setFont(u8g2_font_5x8_mf);
    u8g2.setCursor(27, 64);
    u8g2.print( "0");
    u8g2.setCursor(32, 45);
    u8g2.print( "0.25");
    u8g2.setCursor(56, 33);
    u8g2.print( "0.5");
    u8g2.setCursor(76, 45);
    u8g2.print( "0.75");
    u8g2.setCursor(99, 64);
    u8g2.print( "1");
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);//文字大小12

    float _dataR1 = mapFloat( INA226_data.current, 0.0, 1.00, 90.0, 0.0);   //转换表针位置半圆
    float _dataR2 = mapFloat( INA226_data.current, 0.0, 1.00, -90.0, -0.0); //转换表针位置半圆
    //////////////////////////////////////////
    float L_draw_R1 = cos( _dataR1 * 2 * 3.14 / 180.0) * 40.0 + 64.0;//转换表针在屏幕显示
    /////////////////////////////////////区域大小///函数域中心轴
    float L_draw_R2 = sin( _dataR2 * 2 * 3.14 / 180.0) * 40.0 + 64.0;//转换表针在屏幕显示
    ///////////////////////////////////////////////////////

    u8g2.drawCircle(64, 63, 40, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);//表盘
    u8g2.drawCircle(62, 64, 6, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT); //表盘

    u8g2.drawLine(62, 62, L_draw_R1, L_draw_R2);  //表针
 
  }
  void zhuomianUI3(){ //桌面3 未使用
    u8g2.setCursor(0, 10);
    u8g2.print("③");

  }
  void zhuomianUI4(){ //桌面4 未使用
    u8g2.setCursor(0, 10);
    u8g2.print("④");

  }
  void zhuomianUI5(){ //桌面5 未使用
      u8g2.setCursor(0, 10);
      u8g2.print("⑤");

  }
///////////////////////////////////////////参数UI
void canshuUI1(){
    u8g2.setCursor(0, 10);
    u8g2.print("①");
    
    u8g2.setCursor( 3,25);
    u8g2.print("D数字4");
    u8g2.setCursor( 3,34);
    u8g2.print(data3[1][0]);

    u8g2.setCursor(45,25);
    u8g2.print("D数字5");
    u8g2.setCursor(45,34);
    u8g2.print(data3[1][1]);

    u8g2.setCursor(90,25);
    u8g2.print("D数字6");
    u8g2.setCursor(90,34);
    u8g2.print(data3[1][2]);
    ////
    u8g2.setCursor( 3,49);
    u8g2.print("Relay1");
    u8g2.setCursor( 3,59);
    u8g2.print(Relay1_IO);

    u8g2.setCursor(45,49);
    u8g2.print("Relay2");
    u8g2.setCursor(45,59);
    u8g2.print(Relay2_IO);

    u8g2.setCursor(90,49);
    u8g2.print("PWM");
    u8g2.setCursor(90,59);
    //u8g2.print(Relay3_IO);
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
    u8g2.print("蜂鸣");
    u8g2.setCursor( 3,59);
    u8g2.print(Buzzer_IO);

    u8g2.setCursor(45,49);
    u8g2.print("模拟");
    u8g2.setCursor(45,59);
    u8g2.print(A_D_1);

    u8g2.setCursor(90,49);
    u8g2.print("右R");
    u8g2.setCursor(90,59);
    u8g2.print(Button_R_PIN);

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
    ID_time_UI++;               //滚动X速度增加
    if(ID_time_UI > 0){         //滚动速度启动位+滚动时间 数值越大越慢
      ID_time_X_UI++;           //滚动增加
      if(ID_time_X_UI >= 240){  //滚动X限制位
        ID_time_X_UI = -240;    //滚动X重置位
      }
      
      ID_time_UI = 0;//滚动停止位
    }

    int IDdata1 = map( ID_time_X_UI, -240, 0, 0, 240);
    u8g2.setCursor( IDdata1, 35);
    u8g2.print(part_XH1);//显示ID

    int IDdata2 = map( ID_time_X_UI, 0, 240, -240, 0);
    u8g2.setCursor( IDdata2, 35);
    u8g2.print(part_XH1);//显示ID

    u8g2.setCursor( ID_time_X_UI, 35);
    u8g2.print(part_XH1);//显示ID
  
  ///////////////////////////////闪烁数值
    ///////////////////////////////设置
      char str1[10] = "";  // 用于拼接的目标字符串（每次循环重置）
      char part1[10] = "<";  // 可切换的部分1（< 或 空格）
      char part2[10] = "设置";  // 固定中文部分
      char part3[10] = ">";  // 可切换的部分3（> 或 空格）

      if(data11 == 0 && data21 == 0 && data10 == 1){
        if(OLED_UI_time1 == 0){
          strcpy(part1, "<");
          strcpy(part3, ">");
          OLED_UI_time2++;
          if(OLED_UI_time2 > 15){
            OLED_UI_time1 = 1;
            OLED_UI_time2 = 0;
          }
        }else{
          strcpy(part1, " ");
          strcpy(part3, " ");
          OLED_UI_time2++;
          if(OLED_UI_time2 > 15){
            OLED_UI_time1 = 0;
            OLED_UI_time2 = 0;
          }
        }
        strcpy(str1, part1);    // 
        strcat(str1, part2);    //
        strcat(str1, part3);    //

        u8g2.setCursor(4,60);
        u8g2.print(str1);
      }
      else{
        u8g2.setCursor(4,60);
        u8g2.print("<设置>");
      }
    ///////////////////////////////桌面
      char str_Z1[10] = "";  // 用于拼接的目标字符串（每次循环重置）
      char part_Z1[10] = "<";  // 可切换的部分1（< 或 空格）
      char part_Z2[10] = "桌面";  // 固定中文部分
      char part_Z3[10] = ">";  // 可切换的部分3（> 或 空格）

      if(data11 == 0 && data21 == 0 && data10 == 2){
        if(OLED_UI_time3 == 0){
          strcpy(part_Z1, "<");
          strcpy(part_Z3, ">");
          OLED_UI_time4++;
          if(OLED_UI_time4 > 15){
            OLED_UI_time3 = 1;
            OLED_UI_time4 = 0;
          }
        }else{
          strcpy(part_Z1, " ");
          strcpy(part_Z3, " ");
          OLED_UI_time4++;
          if(OLED_UI_time4 > 15){
            OLED_UI_time3 = 0;
            OLED_UI_time4 = 0;
          }
        }
        strcpy(str_Z1, part_Z1);    // 
        strcat(str_Z1, part_Z2);    //
        strcat(str_Z1, part_Z3);    //

        u8g2.setCursor(48,60);
        u8g2.print(str_Z1);
      }
      else{
        u8g2.setCursor(48,60);
        u8g2.print("<桌面>");
      }
    ///////////////////////////////数据
      char str_S1[10] = "";  // 用于拼接的目标字符串（每次循环重置）
      char part_S1[10] = "<";  // 可切换的部分1（< 或 空格）
      char part_S2[10] = "数据";  // 固定中文部分
      char part_S3[10] = ">";  // 可切换的部分3（> 或 空格）

      if(data11 == 0 && data21 == 0 && data10 == 3){
        if(OLED_UI_time5 == 0){
          strcpy(part_S1, "<");
          strcpy(part_S3, ">");
          OLED_UI_time6++;
          if(OLED_UI_time6 > 15){
            OLED_UI_time5 = 1;
            OLED_UI_time6 = 0;
          }
        }else{
          strcpy(part_S1, " ");
          strcpy(part_S3, " ");
          OLED_UI_time6++;
          if(OLED_UI_time6 > 15){
            OLED_UI_time5 = 0;
            OLED_UI_time6 = 0;
          }
        }
        strcpy(str_S1, part_S1);    // 
        strcat(str_S1, part_S2);    //
        strcat(str_S1, part_S3);    //

        u8g2.setCursor(90,60);
        u8g2.print(str_S1);
      }
      else{
        u8g2.setCursor(90,60);
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
void L_a_button(){
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
void R_b_button(){
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
      u8g2.setCursor(9,22);
      u8g2.print("叮~");
      u8g2.drawRFrame(3, 25, 30, 13, 6); 
      u8g2.drawDisc( data4[0][0][0], 31, 5);                //X:10~25 //半圆
      digitalWrite(led, data3[0][0]);           //引脚输出

    //
    /////////////////////////NumberKey 只有X改变位置
      u8g2.setCursor(46,22);
      u8g2.print("Relay1");
      u8g2.drawRFrame( 45, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][1], 31, 5);                //X:97~
      //digitalWrite(Relay1_IO, data3[0][1]);

      u8g2.setCursor(88,22);
      u8g2.print("Relay2");
      u8g2.drawRFrame( 90, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][2], 31, 5);                //X:52~
      digitalWrite(Relay2_IO, data3[0][2]);
      if (data3[0][2] == 1)
      {//当数字按钮2被按下时，执行以下代码
        
      }
      

      u8g2.setCursor(6,48);
      u8g2.print("Relay3");
      u8g2.drawRFrame( 3, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][0], 55, 5);                //X:10~25
      //digitalWrite(Relay3_IO, data3[1][0]);

      u8g2.setCursor(46,48);
      u8g2.print("Relay4");
      u8g2.drawRFrame(45, 49, 30, 13, 6); 
      u8g2.drawDisc( data4[0][1][1], 55, 5);                //X:10~25 //半圆

      u8g2.setCursor(88,48);
      u8g2.print("蜂鸣静");
      u8g2.drawRFrame( 90, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][2], 55, 5);                //X:52~

    //可在一下判断条件中，添加你想要的相应的代码功能
    /////////////////////////数字按键输出 设置
      if (data3[0][0] == 1){//灯数字按钮
        data4[0][0][0] = 25;
      }
      else{
        data4[0][0][0] = 10;
      }

      if(data3[0][1] == 1){//数字按钮1
        data4[0][0][1] = 67;
      }
      else{
        data4[0][0][1] = 52;
      }

      if(data3[0][2] == 1){//数字按钮2
        data4[0][0][2] = 112;
      }
      else{
        data4[0][0][2] = 97;
      }

      if(data3[1][0] == 1){//数字按钮3
        data4[0][1][0] = 25;
        if(xuni_key1 == 0){//只执行一次
          //myDFPlayer.volume(0);
          xuni_key1 = 1;
          xuni_key2 = 0;
        }
      }
      else {
        data4[0][1][0] = 10; 
        if(xuni_key2 == 0){//只执行一次
          //myDFPlayer.volume(10);
          xuni_key2 = 1;
          xuni_key1 = 0;
        }
      }

      if(data3[1][1] == 1){//数字按钮4
          data4[0][1][1] = 67;
      }
      else{
        data4[0][1][1] = 52;
      }

      if(data3[1][2] == 1){//数字按钮5  
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
  }
  void zhuomianUI(){  //桌面页UI
    u8g2.setCursor(46,10);
    u8g2.print("桌面UI");

    INA226_loop();    //INA226运行
    serial_print1();  //串口打印
    ble_run();        //蓝牙

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
      caidanshuzhi();     //菜单数值 限制值
      GuangBiao ();       //光标值
      u8g2.sendBuffer();  //屏幕刷新作用
}

///////////////////////////////////////////////////////
void xuan (){//选择按键
  //////////////////////////////////按钮及编码刷新
    button[0].Update();           //MID按钮
    button[1].Update();           //L
    button[2].Update();           //R
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
      digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
    }

    
    if (button[1].clicks == 1) {//左
      L_a_button(); //光标值刷新
    }
    
    if (button[2].clicks == 1){//右
      R_b_button(); //光标值刷新
    }
    
    if (button[3].clicks == 1){//上 PWM
      PWM_DATA1 = PWM_DATA1 + 20; //pwm步进值
      if (PWM_DATA1 > 255)
      {
        PWM_DATA1 = 255;
      }
      Slider1.print(PWM_DATA1);
    }
    
    if (button[4].clicks == 1){//下 PWM
      PWM_DATA1 = PWM_DATA1 - 20; //pwm步进值
      if (PWM_DATA1 < 0)
      {
        PWM_DATA1 = 0;
      }
      Slider1.print(PWM_DATA1);
    }

    if(data3[1][2] == 0){//静音
      digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
    }
    //Serial.println("AAAAAAAAA");//调试用
    //Serial.println(data10);//调试用

    
    if (digitalRead(Buzzer_IO) == HIGH) {//蜂鸣音
      F_data++;
      if (F_data >= 200) {//设置蜂鸣时长
        digitalWrite(Buzzer_IO, LOW);
        F_data = 0;//蜂鸣时长重置
      }
    }
}



///////////////////////////////////////////////////////
void MONI(){//模拟量 测试屏幕位置或转换数据作用，可在loop启用
  Servo_DATA = map(analogRead(A_D_1), 0, 4096, 0, 180); // A/D转换
}
void setup() {
  Serial.begin(115200);

  //////////////////////////////////
  pinMode(OLED_GND, OUTPUT);  //
  pinMode(OLED_VCC, OUTPUT);  //
  digitalWrite(OLED_GND, LOW);
  digitalWrite(OLED_VCC, HIGH);
  //////////////////////////////////

  u8g2.begin();                             //初始化OLED
  u8g2.enableUTF8Print();                   //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);   //文字大小15

  INA226_setup(); //INA226初始化  
  setupPWM();     //PWM初始化
  BLE_RUN1();     //蓝牙初始化

  pinMode(Buzzer_IO, OUTPUT);           //蜂鸣器
  pinMode(Button_L_PIN, INPUT_PULLUP);  //L
  pinMode(Button_R_PIN, INPUT_PULLUP);  //R
  pinMode(Button_U_PIN, INPUT_PULLUP);  //U
  pinMode(Button_D_PIN, INPUT_PULLUP);  //D
  pinMode( Button_key1, INPUT_PULLUP);  //MID按钮

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//文字大小12

  pinMode(led, OUTPUT);       // 设置引脚为输出模式
  pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
  pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
  //pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式

  pinMode( A_D_1, INPUT);    //模拟量输入

  fastTimer.setInterval(10000); //多任务 //10MS //100,000微秒 = 100ms
  fastTimer1.setInterval(100); //多任务 //10MS //100,000微秒 = 100ms
  
  shuaxin();//执行一次OLED画面刷新
}
void loop() {
  //MONI();     //模拟量，测试及数据转换
  xuan ();      //选择功能

  if (fastTimer.isReady()) {//多任务就绪时执行
      shuaxin();          //刷新
      fastTimer.reset();  //重置多任务定时器
  }
  
  if (fastTimer1.isReady()) {//多任务就绪时执行
      BLE_control();          //刷新
      fastTimer1.reset();  //重置多任务定时器
  }
}




