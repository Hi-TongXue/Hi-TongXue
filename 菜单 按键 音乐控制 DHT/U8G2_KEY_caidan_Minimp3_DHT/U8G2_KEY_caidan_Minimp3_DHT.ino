/*此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563 现版本：V1.7
  ////库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    Playermini音乐        DFRobotDFPlayerMini.h
    DHT                   DHT.h
    多任务库（作用不大）    SimplifiedTimer.h

  BUG及解决更改记录:
    1 光标不移动
    2 光标宽度太小
    3 光标无限制，光标起飞~芜湖~~！
    4 没有二级菜单
    5 二级菜单无法进入
    6 进入二级菜单无法退出
    7 二级菜单页面无限制
    8 二级菜单选择无限制
    9 进入二级菜单仍能选择一级菜单光标移动同时无法返回一级菜单
    10 进入二级菜单无法重建其他二级菜单且无法建立三级菜单
    11 代码流畅度及算法优化1.0
    12 优化代码1.1
    13 引用MP3Mini
    14 初始化数字按键
    15 MP3桌面UI
    16 MP3UI不显示
    17 MP3UI显示异常，切换歌曲UI不显示
    18 引用多任务Library，防止程序起飞！（防内存溢出）
    19 代码流畅度及算法优化1.4
    20 留存bug：双击切换易误触
    21 写入设置界面UI
    22 编辑数字按键UI及功能
    23 数字按键UI乱码
    24 数字按键功能设定
    25 启用Relay引脚初始化及功能设定映射
    26 二级菜单UI光标Y轴设定
    27 代码流畅度及算法优化1.6
    28 引用DHT
    29 DHT转换及表盘UI&表针UI转换
    30 切换歌曲与切换UI冲突
    31 数字按钮4使用MP3一次静音并带有恢复
    32 数据UI显示部分启用的GPIO
    33 代码流畅度及算法优化1.7
*/
///////////////////////////////////////////////////////
///////////////////////////定义Library
  #include <U8g2lib.h>                //引用OLED库
  #include "ClickButton.h"            //引用消抖库
  #include "DFRobotDFPlayerMini.h"    //引用MP3Mini库
  #include <SimplifiedTimer.h>        //引用多任务库，防程序阻塞
  #include <DHT.h>                    //引用DHT系列温湿度库
///////////////////////////GPIO连接映射
  int minimp3_RX = 21;      //MP3引脚
  int minimp3_TX = 22;      //MP3引脚

  int SDA_U8G2 = 33;        //OLED屏幕引脚
  int SCL_U8G2 = 32;        //OLED屏幕引脚

  int x = 3; //OLED初始显示位置使用
  int y = 3; //OLED初始显示位置使用

  int key1 = 88;           //未使用
  int key2 = 88;           //未使用
  int key3 =  5;          //按键1
  int key4 = 18;          //按键2
  int key5 = 19;          //按键3

  int led = 2;            //数字按键灯引脚
  int Relay1_IO = 12;     //数字按键继电1引脚
  int Relay2_IO = 14;     //数字按键继电2引脚
  int Relay3_IO = 27;     //数字按键继电3引脚

  //int A_D_1 = 25;
///////////////////////////数组  光标位置 二级光标位置
  //
  int data1[10] = { 0, 1, 45, 88, 0};               //一级光标位置
  int data2[10] = { 1, 43, 88, 1, 0};               //二级菜单光标位置
  int data3[10] = { 0, 0, 0, 0};                    //二级菜单设置页面数字按键 
  int data4[10] = { 3, 45, 90, 3, 10, 97, 52, 10};  //二级设置页数字按键X位置
  int data5[10] = { 25, 49};                        //二级设置页数字按键Y位置
///////////////////////////数字量  选择和菜单-数量及参数
  int data10 = 2;             //一级光标选择
  int data11 = 0;             //菜单级别数量数值
  int data12 = 0;             //二级(设置)菜单光标选择
  int data13 = 48;

  int MP3_DHT_UI = 0;         //MP3_DHT ui切换

  int xuni_key1 = 0;           //虚拟按键1：保证代码只循环一次
  int xuni_key2 = 0;           //虚拟按键2：保证代码只循环一次
  ////////////////////////////////MP3数据
    int data23 = 0;             //MP3状态
    int data24 = 0;             //MP3文件数量
    int data25 = 0;             //MP3当前播放编号
    int data26 = 0;             //MP3当前音量
  ////////////////////////////////DHT数据
    float TEMP_dataR1 = 0;
    float TEMP_dataR2 = 0;
    float HUMI_dataR1 = 0;
    float HUMI_dataR2 = 0;

    float L_TEMP_draw_R1 = 0;
    float L_TEMP_draw_R2 = 0;
    float L_HUMI_draw_R1 = 0;
    float L_HUMI_draw_R2 = 0;

    float humi_read, temp_read;
///////////////////////////库函数初始化使用
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SDA_U8G2, SCL_U8G2);

  ClickButton button[5] = {//消抖按键映射
    ClickButton (key1, LOW, CLICKBTN_PULLUP),
    ClickButton (key2, LOW, CLICKBTN_PULLUP),
    ClickButton (key3, LOW, CLICKBTN_PULLUP),
    ClickButton (key4, LOW, CLICKBTN_PULLUP),
    ClickButton (key5, LOW, CLICKBTN_PULLUP),
  };

  SimplifiedTimer firstTimer(800);  //多任务时间  定时器：初始化时直接指定间隔为1000

  HardwareSerial FPSerial(2);  // ESP32使用第二个硬件串口（编号可能因板型而异）
  DFRobotDFPlayerMini myDFPlayer;//ESP32

  #define DHTPIN 26  
  #define DHTTYPE DHT11 
  DHT dht(DHTPIN, DHTTYPE);
///////////////////////////////////////////////////////
void readmp3(){//读取MP3
  if (firstTimer.isReady()) {//判断是否读取
    data23 = myDFPlayer.readState();              //读取 MP3 状态
    data24 = myDFPlayer.readFileCounts();         //读取 SD 卡中的所有文件数量
    data25 = myDFPlayer.readCurrentFileNumber();  //读取当前播放文件编号
    data26 = myDFPlayer.readVolume();             //读取音量
    firstTimer.disable();
    // 重新启用定时器
    firstTimer.enable();
    // 重置定时器
    firstTimer.reset();
  }
}
void MONI(){//模拟量 测试屏幕位置或转换数据作用，可在LOOP启用
  //data2[0] = map(analogRead(A_D_1), 0, 4096, 0, 128); // A/D转换
  //data2[1] = map(analogRead(A_D_2), 0, 4096, 0, 64); // A/D转换
}
void setup() {
  Serial.begin(115200);

  u8g2.begin();                                          //初始化OLED
  u8g2.enableUTF8Print();                                //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);                //文字大小15

  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/minimp3_RX, /*tx =*/minimp3_TX);//esp32连接miniMP3 初始化

  dht.begin();                                           //温湿度初始化

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//文字大小12
  pinMode(led, OUTPUT);       // 设置引脚为输出模式
  pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
  pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
  pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式

  //pinMode( A_D_1, INPUT);    //模拟量输入
  //pinMode( A_D_2, INPUT);    //模拟量输入
}
void loop() {
  xuan ();      //选择功能
  GuangBiao (); //光标功能
  //MONI();     //模拟量，测试用
}
///////////////////////////////////////////////////////
void xuan (){//选择按键
  //////////////////////////////////按键刷新
    button[0].Update();   //key1 左
    button[1].Update();   //key2 右
    button[2].Update();   //key3 上
    button[3].Update();   //key4 下
    button[4].Update();   //key5 右上
  //////////////////////////////////按键数值映射
    if (button[0].clicks == 1) {//clicks次数 未使用
      //
    }
    else if (button[1].clicks == 1){ //未使用
      //
    }
    else if (button[2].clicks == 1){  //上
      if (data11 == 0){//一级菜单光标
        data10++ ;
      }
      else if(data11 == 1 && data10 == 1){//二级菜单光标
        data12++;
      }
      else if(data11 == 1 && data10 == 2 && MP3_DHT_UI == 0){//声音+
        myDFPlayer.volumeUp(); //声音加
      }
    }
    else if (button[2].clicks == 2){  //上一首
      if (data11 == 1 && data10 == 2){//桌面页面数字按键
        if (MP3_DHT_UI == 0){//MP3
          lastmp3();
        }
      }
    }
    else if (button[3].clicks == 1){  //下
      if (data11 == 0){//一级菜单光标
        data10-- ;
      }
      else if(data11 == 1 && data10 == 1){//二级菜单光标
        data12--;
      }
      else if(data11 == 1 && data10 == 2 && MP3_DHT_UI == 0){//声音-
        myDFPlayer.volumeDown(); //声音减
      }
    }
    else if (button[3].clicks == 2){  //下一首
      if (data11 == 1 && data10 == 2){//桌面页面数字按键
        if (MP3_DHT_UI == 0){//MP3
          nextmp3();
        }
      }
    }
    else if (button[4].clicks == 1){  //菜单进退
      data11++ ;//data11
    }
    else if (button[4].clicks == 2){  //双击
      if (data11 == 1 && data10 == 2){//停止音频
        stopmp3();
      }
      if (data11 == 1 && data10 == 1){//数字按键翻转
        data3[data12] = !data3[data12];//翻转
      }
    }
    else if (button[4].clicks == 3){  //三击继续播放
      if (data11 == 1 && data10 == 2){//桌面页面数字按键
        startmp3();
      }
    }
    else if (button[4].clicks == -1){ //进退键长按切换桌面UI
      if (data11 == 1 && data10 == 2){//桌面页面 MP3_DHT_UI切换
        MP3_DHT_UI++;
      }
    }
  /////////////////////////////////////////菜单数值
    if (data11 == 0){//菜单页
      OLEDcaidan();
    }
    else if(data11 == 1 && data10 == 1){//设置
      shezhiUI();
      if(data11 > 1){
        data11 = 0;
      }
    }
    else if(data11 == 1 && data10 == 2){//桌面
      zhuomianUI();
      if(data11 > 1){
        data11 = 0;
      }
    }
    else if(data11 == 1 && data10 == 3){//参数
      canshuUI();
      if(data11 > 1){
        data11 = 0;
      }
    }
    else{
      data11 = 0;
    }

}
///////////////////////////////////////////二级菜单页面
  void shezhiUI(){    //设置页UI
    u8g2.setCursor(x,y);      //屏幕刷新作用
    u8g2.firstPage();         //屏幕刷新作用

    u8g2.setCursor(42,10);
    u8g2.print("设置UI");
    /////////////////////////灯
      u8g2.setCursor(9,22);
      u8g2.print("叮~");
      u8g2.drawRFrame(data4[0], data5[0], 30, 13, 6); //画表针
      u8g2.drawDisc( data4[4], 31, 5);                //X:10~25 //半圆
      digitalWrite(led, data3[0]);                    //引脚输出

    //
    /////////////////////////NumberKey
      u8g2.setCursor(46,22);
      u8g2.print("Relay1");
      u8g2.drawRFrame( data4[1], data5[0], 30, 13, 6);
      u8g2.drawDisc( data4[5], 31, 5);          //X:52~
      digitalWrite(Relay1_IO, data3[1]);

      u8g2.setCursor(88,22);
      u8g2.print("Relay2");
      u8g2.drawRFrame(data4[2], data5[0], 30, 13, 6);
      u8g2.drawDisc( data4[6], 31, 5);          //X:97~
      digitalWrite(Relay2_IO, data3[2]);

      u8g2.setCursor(6,48);
      u8g2.print("Relay3");
      u8g2.drawRFrame( data4[3], data5[1], 30, 13, 6);
      u8g2.drawDisc( data4[7], 55, 5);          //X:10~25
      digitalWrite(Relay3_IO, data3[3]);

      if (data3[0] == 1){//灯数字按钮
        data4[4] = 25;
      }
      else{
        data4[4] = 10;
      }

      if(data3[1] == 1){//数字按钮1
        data4[5] = 67;
      }
      else{
        data4[5] = 52;
      }

      if(data3[2] == 1){//数字按钮2
        data4[6] = 112;
      }
      else{
        data4[6] = 97;
      }

      if(data3[3] == 1){//数字按钮3
        data4[7] = 25;
        if(xuni_key1 == 0){
          myDFPlayer.volume(0);
          xuni_key1 = 1;
          xuni_key2 = 0;
        }
      }
      else {
        data4[7] = 10; 
        if(xuni_key2 == 0){
          myDFPlayer.volume(10);
          xuni_key2 = 1;
          xuni_key1 = 0;
        }
      }
    //
    if (data12 > 3){//设定二级光标上限（二级数字按键数量）
      data12 = 3;
    }
    else if (data12 < 0){//设定二级光标下限
      data12 = 0;
    }
  }
  void zhuomianUI(){  //桌面页UI
    if (MP3_DHT_UI == 0){//MP3
      MP3UI();
    }
    else if (MP3_DHT_UI == 1){//温度
      DHT_READ_DATA();
      DHT_UI_TEMP();
    }
    else if (MP3_DHT_UI == 2){//湿度
      DHT_READ_DATA();
      DHT_UI_HUMI();
    }

    if (MP3_DHT_UI > 2){//ui限制
      MP3_DHT_UI = 0;
    }
  }
  void canshuUI(){    //参数页UI 主要用作显示GPIO接口引脚
    u8g2.setCursor(x,y);
    u8g2.firstPage();

    u8g2.setCursor(42,10);
    u8g2.print("数据UI");

    u8g2.setCursor( 3,25);
    u8g2.print("key3");
    u8g2.setCursor( 3,34);
    u8g2.print(key3);

    u8g2.setCursor(45,25);
    u8g2.print("key4");
    u8g2.setCursor(45,34);
    u8g2.print(key4);

    u8g2.setCursor(90,25);
    u8g2.print("key5");
    u8g2.setCursor(90,34);
    u8g2.print(key5);

    u8g2.setCursor( 3,49);
    u8g2.print("Relay1");
    u8g2.setCursor( 3,59);
    u8g2.print(Relay1_IO);

    u8g2.setCursor(45,49);
    u8g2.print("Relay2");
    u8g2.setCursor(45,59);
    u8g2.print(Relay2_IO);

    u8g2.setCursor(90,49);
    u8g2.print("Relay3");
    u8g2.setCursor(90,59);
    u8g2.print(Relay3_IO);
  }
///////////////////////////////////////////桌面MP3UI
  void MP3UI(){//读取MP3显示画面
      readmp3();//多任务读取MP3模块
      u8g2.setCursor(x,y);
      u8g2.firstPage();

      u8g2.setCursor(42,10);
      u8g2.print("桌面UI");

      u8g2.setCursor(0, 64);
      u8g2.print(data24);
      u8g2.setCursor(0, 10);
      u8g2.print("①");

      u8g2.setCursor(31, 26);
      u8g2.print("MP3播放UI");

      u8g2.setCursor(10, 62);
      u8g2.print("音量:");
      u8g2.setCursor(70, 62);
      u8g2.print(data23);

      u8g2.setCursor(10, 38);
      u8g2.print("MP3数量:");
      u8g2.setCursor(70, 38);
      u8g2.print(data25);

      u8g2.setCursor(10, 50);
      u8g2.print("当前编号:");
      u8g2.setCursor(70, 50);
      u8g2.print(data26);
  }
///////////////////////////////////////////桌面DHTUI
  void DHT_READ_DATA(){//读取温湿度数据
    float h = dht.readHumidity();//湿度
    float t = dht.readTemperature();//温度
    float hic = dht.computeHeatIndex(t, h, false);//体感温度 现未使用
    humi_read = h;
    temp_read = t;
  }

  void DHT_UI_TEMP(){//温度
    TEMP_dataR1 = map( temp_read, -10.00, 50.00, 90, 3);   //TEMP 温度转换表针位置半圆
    TEMP_dataR2 = map( temp_read, -10.00, 50.00, -90, -1); //TEMP 温度转换表针位置半圆
    //////////////////////////////////////////
    L_TEMP_draw_R1 = cos(TEMP_dataR1 * 2 * 3.14 / 180) * 60 + 67;//转换表针在屏幕显示
    /////////////////////////////////////区域大小///函数域中心轴
    L_TEMP_draw_R2 = sin(TEMP_dataR2 * 2 * 3.14 / 180) * 60 + 64;//转换表针在屏幕显示
    ///////////////////////////////////////////////////////
    u8g2.setCursor(x, y);
    u8g2.firstPage();

    u8g2.setCursor(0, 10);
    u8g2.print("②");

    u8g2.setCursor(64, 36);
    u8g2.print(temp_read);

    u8g2.drawCircle(65, 63, 62, U8G2_DRAW_ALL);//表盘
    u8g2.drawCircle(62, 64, 6, U8G2_DRAW_ALL); //表盘

    u8g2.drawLine(62, 62, L_TEMP_draw_R1, L_TEMP_draw_R2);  //表针
    u8g2.nextPage();
  }
  void DHT_UI_HUMI(){//湿度
    HUMI_dataR1 = map( humi_read, 0.00, 100.00, 90, 3);   //TEMP
    HUMI_dataR2 = map( humi_read, 0.00, 100.00, -90, -1); //TEMP
    //////////////////////////////////////////
    L_HUMI_draw_R1 = cos(HUMI_dataR1 * 2 * 3.14 / 180) * 60 + 67;
    /////////////////////////////////////区域大小///函数域中心轴
    L_HUMI_draw_R2 = sin(HUMI_dataR2 * 2 * 3.14 / 180) * 60 + 64;
    ///////////////////////////////////////////////////////
    u8g2.setCursor(x, y);
    u8g2.firstPage();
    u8g2.setCursor(0, 10);
    u8g2.print("③");

    u8g2.setCursor(64, 36);
    u8g2.print(humi_read);

    u8g2.drawCircle(65, 63, 62, U8G2_DRAW_ALL);//表盘
    u8g2.drawCircle(62, 64, 6, U8G2_DRAW_ALL); //表盘

    u8g2.drawLine(62, 62, L_HUMI_draw_R1, L_HUMI_draw_R2);  //表针
    u8g2.nextPage();

  }
///////////////////////////////////////////
void OLEDcaidan() {       //菜单界面
  u8g2.setCursor(x,y);
  u8g2.firstPage();

  u8g2.setCursor(42,10);
  u8g2.print("菜单儿~");

  u8g2.setCursor(4,60);
  u8g2.print("<设置>");

  u8g2.setCursor(48,60);
  u8g2.print("<桌面>");

  u8g2.setCursor(90,60);
  u8g2.print("<数据>");

  if(data10 > 3){//光标选择高限
    data10 = 3;
  }
  else if(data10 < 1){//光标选择低限
    data10 = 1;
  }
 
  //u8g2.nextPage();
}

void GuangBiao(){   //光标
  u8g2.setCursor(x,y);
  //u8g2.firstPage();
  if(data11 == 0){
    u8g2.drawRFrame(data1[data10] , 48, 40, 15, 5);
  }
  else if (data11 == 1 && data10 == 1){//光标位置选择限制
    if (data12 == 3){//光标Y限制
      data13 = 48;
    }
    else{
      data13 = 24;
    }
    u8g2.drawRFrame(data2[data12] , data13, 34, 16, 5);//光标
  }
  u8g2.nextPage();//屏幕刷新作用
}

void OLEDjiazai(){  //加载页
  u8g2.setCursor(x,y);
  u8g2.firstPage();

  u8g2.setCursor(37,30);
  u8g2.print("启动中...");
  u8g2.nextPage();  
}

void OLEDkaiji(){   //开机页
  u8g2.setCursor(x,y);
  u8g2.firstPage();
  
  u8g2.setCursor(0,27);
  u8g2.print("加载OLED&MP3模块");
  delay(500);
  u8g2.nextPage();

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //检测使用串行接口与 MP3 设备进行通信
    u8g2.setCursor(0,60);
    u8g2.print( "MP3连接异常" );
    u8g2.nextPage();
    //Serial.println(F("无法开始:"));
    //Serial.println(F("1.请重新检查连接!"));
    //Serial.println(F("2.请插入SD卡!"));
    while(true);
  }
  else {
    myDFPlayer.volume(10);
    u8g2.setCursor(0,60);
    u8g2.print(">");
    delay(300);
    u8g2.nextPage();
    u8g2.setCursor(20,60);
    u8g2.print(">");
    delay(300);
    u8g2.nextPage();
    u8g2.setCursor(40,60);
    u8g2.print(">");
    delay(300);
  }

  u8g2.nextPage();
  u8g2.setCursor(60,60);
  u8g2.print(">");
  delay(500);
  u8g2.nextPage();
  u8g2.setCursor(80,60);
  u8g2.print(">");
  delay(500);
  u8g2.nextPage();
  u8g2.setCursor(100,60);
  u8g2.print(">");
  delay(500);
  u8g2.nextPage();
  u8g2.setCursor(120,60);
  u8g2.print(">");
  delay(500);
  u8g2.nextPage();
  delay(700);
}

void nextmp3(){myDFPlayer.next();}        //下一首
void stopmp3(){myDFPlayer.pause();}       //暂停
void lastmp3(){myDFPlayer.previous();}    //上一首
void startmp3(){myDFPlayer.start();}      //继续播放
