/*==========================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563   编码器版本：V2.0      ===
  ==========================================================================
  //库
    U8G2                 U8g2lib.h
    按键消抖              ClickButton.h
    编码库(EC11)          Bounce2.h
    多任务库              SimplifiedTimer.h
    舵机库                ESP32Servo.h
  //BUG及已解决和代码更改记录:
    1 删除非必要代码
    2 定义编码防抖库
    3 定义多任务库
    4 画面消失光标抖动
    5 关闭除菜单UI外的所有UI
    6 设置光标函数代码摆放位置
    7 设置数字按键XY位置
    8 使用一 二 三维数组
    9 编码内置按键 定义 映射 初始化
    10 使用按键防抖ClickButton库，设置进出菜单
    11 EC11 AB相 定义 映射 初始化及旋钮和正反转检测
    12 启用光标UI
    13 更改菜单 菜单数据 按钮控制 及U8G2屏幕刷新 摆放位置
    14 设置蜂鸣器为按键旋钮音，其设置蜂鸣函数 映射 及时长
    15 启用二级UI
    16 一级光标选择抖动加延迟BUG
    17 OLED刷新使用多任务，并更改刷新关键字为clearBuffer和sendBuffer
    18 定义光标选择数据转换映射
    19 一二级UI混合出现双抖动异常，一二级菜单需全部设限
    20 二级UI三段混合，光标位置异常,二级光标位置及数据转换需全部设限
    21 设置进退菜单蜂鸣音
    22 代码及算法优化V1.0
    23 设置ui选项闪烁
    24 标题选项卡位置错乱，闪烁缺少
    25 标题选项卡闪烁位置错误
    26 增加数字按键，数字按键位置重置
    27 增加参数页
    28 设置蜂鸣静音数字按键
    29 引用ESP32舵机库
    30 启用表盘画面，使用模拟量，映射舵机角度
    31 读取堆内存使用情况 总 已用 已用率 打印OLED 设置桌面页面2
    32 读取本固件的存储空间及剩余空间
    33 设置一次启动位 停止位 （堆 固件 空间）
    34 添加芯片显示型号
    35 添加Flash存储信息（容量 频率 模式）及CPU信息（核数 运行频率）OLED桌面5
    36 代码及算法优化V2.0

*/
///////////////////////////////////////////////////////
///////////////////////////定义Library
  #include <U8g2lib.h>                //引用OLED库
  #include "ClickButton.h"            //引用消抖库
  #include <Bounce2.h>                //EC11编码器库
  #include <SimplifiedTimer.h>        //多任务库
  #include <ESP32Servo.h>             //舵机库
///////////////////////////GPIO连接映射
  const int Encoder_A_PIN = 17;      //编码A引脚
  const int Encoder_B_PIN = 16;      //编码B引脚

  int Encoder_key1 =  4;  //编码按键引脚

  int SDA_U8G2 = 22;        //OLED屏幕引脚
  int SCL_U8G2 = 23;        //OLED屏幕引脚

  const int Buzzer_IO = 26;     //蜂鸣器引脚

  int led = 2;            //数字按键灯引脚
  int Relay1_IO = 12;     //数字按键继电1引脚
  int Relay2_IO = 14;     //数字按键继电2引脚
  int Relay3_IO = 27;     //数字按键继电3引脚

  int Servo_PIN = 33;     //舵机引脚

  int A_D_1 = 35;         //模拟量电位器引脚
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
  int xuni_key3 = 0;           //舵机启动位：代码只循环一次

  int xuni_key4 = 0;           //读取堆内存位：代码只循环一次
  int xuni_key5 = 0;           //读取固件空间位：代码只循环一次
  int xuni_key6 = 0;           //读取芯片型号：代码只循环一次
  int xuni_key7 = 0;           //读取Flash：代码只循环一次
  int xuni_key8 = 0;           //读取CPU：代码只循环一次

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

  ////////////////////////内存使用数据
    unsigned long used_Heap_bytes1; //已使用堆内存
    float total_Heap_kb;            //总堆内存
    float free_Heap_kb;             //空闲堆内存
    float used_Heap_kb;             //已使用堆内存
    float used_Heap_bytes;          //已使用堆内存
    float heapUsage;                //堆内存使用率
  ////////////////////////固件使用数据
    unsigned long freeSketchSpace;//固件分区剩余空间
    unsigned long sketchSize;     //当前固件大小
    float freeMB;                 //MB/KB 转换
    float sketchKB;               //MB/KB 转换
  ////////////////////////芯片ID数据
    char part_XH1[50];      //芯片ID 组合数值
    int ID_time_UI;         //UI滚动
    int ID_time_X_UI = -240;//UI滚动值
  ////////////////////////Flash数据
    unsigned long flashConfigSize;  //FLASH容量
    float flash_ConfigMB;           //转换成MB
    uint32_t Flash_SpeedMHz;        //Flash 运行频率
    int FlashChipMode;              //Flash 模式
  ////////////////////////cpu数据
    uint32_t CPUChipCores;    //CPU核心
    uint32_t CPUFreqMHz;      //CPU频率
///////////////////////////库函数初始化使用
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SDA_U8G2, SCL_U8G2);

  ClickButton button[1] = {//消抖按键映射
    ClickButton (Encoder_key1, LOW, CLICKBTN_PULLUP),
  };

  Bounce Encoder_A_Button = Bounce();//A相
  Bounce Encoder_B_Button = Bounce();//B相
  Bounce Encoder_key1_Button  = Bounce();//未使用 Bounce消抖编码按钮

  SimplifiedTimer fastTimer;    // 多任务 任务1

  Servo myservo;                //舵机
///////////////////////////////////////////////////////
void MONI(){//模拟量 测试屏幕位置或转换数据作用，可在loop启用
  Servo_DATA = map(analogRead(A_D_1), 0, 4096, 0, 180); // A/D转换
}
void setup() {
  Serial.begin(115200);

  u8g2.begin();                                          //初始化OLED
  u8g2.enableUTF8Print();                                //初始化中文
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);                //文字大小15

  pinMode(Buzzer_IO, OUTPUT);            //蜂鸣器
  pinMode(Encoder_A_PIN, INPUT_PULLUP);  //A相
  pinMode(Encoder_B_PIN, INPUT_PULLUP);  //B相
  pinMode( Encoder_key1, INPUT_PULLUP);  //编码按钮
 
  Encoder_A_Button.attach(Encoder_A_PIN);
  Encoder_A_Button.interval(1);           //5-20ms// 设置按键检测间隔，单位为毫秒
 
  Encoder_B_Button.attach(Encoder_B_PIN);
  Encoder_B_Button.interval(1);

  myservo.attach(Servo_PIN);  //舵机初始化

  Heap_DATA();    //堆 内存
  sketch_DATA();  //固件空间
  Flash_DATA();   //Flash信息

  OLEDjiazai();         //加载页
  delay(1000);
  OLEDkaiji();          //开机页
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);//文字大小12

  pinMode(led, OUTPUT);       // 设置引脚为输出模式
  pinMode(Relay1_IO, OUTPUT); // 设置引脚为输出模式
  pinMode(Relay2_IO, OUTPUT); // 设置引脚为输出模式
  pinMode(Relay3_IO, OUTPUT); // 设置引脚为输出模式

  pinMode( A_D_1, INPUT);    //模拟量输入

  Encoder_A_Button.update();    //A相
  begin_dataA = Encoder_A_Button.read();//记录A相初始值

  fastTimer.setInterval(10000); //多任务 //10MS //100,000微秒 = 100ms
  
  shuaxin();//执行一次OLED画面刷新
}
void loop() {
  MONI();     //模拟量，测试及数据转换
  xuan ();      //选择功能

  if (fastTimer.isReady()) {//多任务就绪时执行
      shuaxin();          //刷新
      fastTimer.reset();  //重置多任务定时器
  }
}
///////////////////////////////////////////////////////
void xuan (){//选择按键
  //////////////////////////////////按钮及编码刷新
    button[0].Update();           //Encoder_key1编码按钮
    Encoder_A_Button.update();    //A相
    Encoder_B_Button.update();    //B相

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

    UP_dataA = Encoder_A_Button.read();//读A相值
    if (UP_dataA != begin_dataA) {//判断是否旋转，A相
      
      UP_dataB = Encoder_B_Button.read();//读B相值

      if(begin_dataA == HIGH && UP_dataA == LOW){//判断旋转方向A相
        if (UP_dataB == HIGH) {////判断旋转方向B相
          Encoder_a_button(); //旋钮光标值刷新
        }
        else{
          Encoder_b_button(); //旋钮光标值刷新
        }

        if(data3[1][2] == 0){//静音
          digitalWrite(Buzzer_IO, HIGH);//蜂鸣器
        }
        //Serial.println("AAAAAAAAA");//调试用
        //Serial.println(data10);//调试用
      }
      begin_dataA = UP_dataA; //更新A相值
    }
    
    if (digitalRead(Buzzer_IO) == HIGH) {//蜂鸣音
      F_data++;
      if (F_data >= 200) {//设置蜂鸣时长
        digitalWrite(Buzzer_IO, LOW);
        F_data = 0;//蜂鸣时长重置
      }
    }
}
///////////////////////////////////////////
void shuaxin(){//刷新
      u8g2.clearBuffer(); //清屏
      caidanshuzhi();     //菜单数值 限制值
      GuangBiao ();       //光标值
      u8g2.sendBuffer();  //屏幕刷新作用
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
///////////////////////////////////////////光标 限制 刷新
void Encoder_a_button(){
        //Serial.println("顺时针");//调试用
        
        if (data21 == 0){//一级菜单光标
          data20++ ;
          data10 = data20;// - 1;//做减一 是防止编码两次跳动外加数值转换
        }
        else if(data21 == 1 && data20 == 1){//二级菜单光标
          data22++;
          data12 = data22;// - 1;
        }
}
void Encoder_b_button(){
  //Serial.println("逆时针旋转");//调试用
  
  if (data21 == 0){//一级菜单光标
      data20-- ;
      data10 = data20;// + 1;//加一 防止编码两次跳动外加数值转换
  }
  else if(data21 == 1 && data20 == 1){//二级菜单光标
      data22--;
      data12 = data22;// + 1;
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
      digitalWrite(Relay1_IO, data3[0][1]);

      u8g2.setCursor(88,22);
      u8g2.print("Relay2");
      u8g2.drawRFrame( 90, 25, 30, 13, 6);
      u8g2.drawDisc( data4[0][0][2], 31, 5);                //X:52~
      digitalWrite(Relay2_IO, data3[0][2]);

      u8g2.setCursor(6,48);
      u8g2.print("Relay3");
      u8g2.drawRFrame( 3, 49, 30, 13, 6);
      u8g2.drawDisc( data4[0][1][0], 55, 5);                //X:10~25
      digitalWrite(Relay3_IO, data3[1][0]);

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

    if (WINDOW_UI == 0){//桌面1
      zhuomianUI1();
    } 
    else if (WINDOW_UI == 1){//堆内存
      zhuomianUI2();
    }
    else if (WINDOW_UI == 2){//固件空间
      zhuomianUI3();
    }
    else if (WINDOW_UI == 3){//Flash信息
      zhuomianUI4();
    }
    else if (WINDOW_UI == 4){//CPU信息
      zhuomianUI5();
    }
    
    if (WINDOW_UI > 4){//ui限制
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
///////////////////////////////////////////桌面UI
  void zhuomianUI1(){ //桌面1
      u8g2.setCursor(0, 10);
      u8g2.print("①");

      Servo_DATA_UI();//舵机数据
  }
  void zhuomianUI2(){ //桌面2
      u8g2.setCursor(0, 10);
      u8g2.print("②");

      if(xuni_key4 == 0){//只运行一次
        Heap_DATA(); //读取内存使用情况
        xuni_key4 = 1;//运行一次的停止位
      }
      
    //*调试
      u8g2.setCursor(26, 26);
      //u8g2.setCursor(42,10);
      u8g2.print("堆内存使用情况");

      u8g2.setCursor(5, 38);
      u8g2.print("总堆内存:");
      u8g2.setCursor(75, 38);
      String total_Heap_UI = String(total_Heap_kb) + "KB";
      u8g2.print(total_Heap_UI);

      u8g2.setCursor(5, 50);
      u8g2.print("已用堆内存:");
      u8g2.setCursor(75, 50);
      String used_Heap_UI = String(used_Heap_kb) + "KB";
      u8g2.print(used_Heap_UI);

      u8g2.setCursor(5, 62);
      u8g2.print("堆内存用率:");
      u8g2.setCursor(75, 62);
      String heapUsage_UI = String(heapUsage) + "%";
      u8g2.print(heapUsage_UI);
    //*/
  }
  void zhuomianUI3(){ //桌面3
      u8g2.setCursor(0, 10);
      u8g2.print("③");

      if(xuni_key5 == 0){//只运行一次
        sketch_DATA(); //读取固件空间使用情况
        xuni_key5 = 1;//运行一次的停止位
      }
      
    //*调试
      u8g2.setCursor(22, 26);
      //u8g2.setCursor(42,10);
      u8g2.print("固件空间使用情况");

      u8g2.setCursor(5, 38);
      u8g2.print("已用空间:");
      u8g2.setCursor(75, 38);
      String sketchKB_UI = String(sketchKB) + "KB";
      u8g2.print(sketchKB_UI);

      u8g2.setCursor(5, 50);
      u8g2.print("剩余空间:");
      u8g2.setCursor(75, 50);
      String freeMB_UI = String(freeMB) + "MB";
      u8g2.print(freeMB_UI);
    //*/
  }
  void zhuomianUI4(){ //桌面4
      u8g2.setCursor(0, 10);
      u8g2.print("④");

      if(xuni_key7 == 0){//只运行一次
        Flash_DATA(); //读取Flash 容量
        xuni_key7 = 1;//运行一次的停止位
      }
      
      u8g2.setCursor(22, 26);
      //u8g2.setCursor(42,10);
      u8g2.print("Flash 容量信息");

      u8g2.setCursor(5, 38);
      u8g2.print("容量:");
      u8g2.setCursor(75, 38);
      String flash_ConfigMB_UI = String(flash_ConfigMB) + "MB";
      u8g2.print(flash_ConfigMB_UI);

      u8g2.setCursor(5, 50);
      u8g2.print("运行频率:");
      u8g2.setCursor(75, 50);
      String FlashChipSpeed_UI = String(Flash_SpeedMHz) + "MHz";
      u8g2.print(FlashChipSpeed_UI);

      u8g2.setCursor(5, 62);
      u8g2.print("Flash模式:");
      u8g2.setCursor(75, 62);
      u8g2.print(FlashChipMode);
  }
  void zhuomianUI5(){ //桌面5
      u8g2.setCursor(0, 10);
      u8g2.print("⑤");

      if(xuni_key8 == 0){//只运行一次
        CPU_DATA(); //读取CPU数据
        xuni_key8 = 1;//运行一次的停止位
      }
      
      //u8g2.setCursor(22, 26);//备选位置
      u8g2.setCursor(42,26);
      u8g2.print("CPU参数");

      u8g2.setCursor(5, 38);
      u8g2.print("核数");
      u8g2.setCursor(75, 38);
      String CPUCores_UI = String(CPUChipCores) + "核";
      u8g2.print(CPUCores_UI);

      u8g2.setCursor(5, 50);
      u8g2.print("运行频率:");
      u8g2.setCursor(75, 50);
      String CPUFreqMHz_UI = String(CPUFreqMHz) + "MHz";
      u8g2.print(CPUFreqMHz_UI);
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
    u8g2.print("Relay3");
    u8g2.setCursor(90,59);
    u8g2.print(Relay3_IO);

}
void canshuUI2(){
    u8g2.setCursor(0, 10);
    u8g2.print("②");
    
    u8g2.setCursor( 3,25);
    u8g2.print("编码S");
    u8g2.setCursor( 3,34);
    u8g2.print(Encoder_key1);

    u8g2.setCursor(45,25);
    u8g2.print("编码A");
    u8g2.setCursor(45,34);
    u8g2.print(Encoder_A_PIN);

    u8g2.setCursor(90,25);
    u8g2.print("编码B");
    u8g2.setCursor(90,34);
    u8g2.print(Encoder_B_PIN);

    u8g2.setCursor( 3,49);
    u8g2.print("蜂鸣");
    u8g2.setCursor( 3,59);
    u8g2.print(Buzzer_IO);

    u8g2.setCursor(45,49);
    u8g2.print("模拟");
    u8g2.setCursor(45,59);
    u8g2.print(A_D_1);

    u8g2.setCursor(90,49);
    u8g2.print("舵机");
    u8g2.setCursor(90,59);
    u8g2.print(Servo_PIN);

}
///////////////////////////////////////////舵机
  void Servo_DATA_UI(){//舵机
    myservo.write(Servo_DATA);  // 转动到90度位置
    int Servo_dataR1;
    int Servo_dataR2;
    int Servo_DATA_draw_R1;
    int Servo_DATA_draw_R2;
    //myservo.read();//舵机读取 //未使用，影响代码运行速度
    Servo_dataR1 = map( Servo_DATA, 0, 180, 90, 3);   //TEMP 温度转换表针位置半圆
    Servo_dataR2 = map( Servo_DATA, 0, 180, -90, -1); //TEMP 温度转换表针位置半圆
    //////////////////////////////////////////
    Servo_DATA_draw_R1 = cos(Servo_dataR1 * 2 * 3.14 / 180) * 60 + 67;//转换表针在屏幕显示
    /////////////////////////////////////区域大小///函数域中心轴
    Servo_DATA_draw_R2 = sin(Servo_dataR2 * 2 * 3.14 / 180) * 60 + 64;//转换表针在屏幕显示
    ///////////////////////////////////////////////////////
    u8g2.setCursor(62, 36);
    String Servo_DATA_printUI = String(Servo_DATA) + "°";
    u8g2.print(Servo_DATA_printUI);

    u8g2.drawCircle(65, 63, 62, U8G2_DRAW_ALL);//表盘
    u8g2.drawCircle(62, 64,  6, U8G2_DRAW_ALL);//表盘

    u8g2.drawLine(62, 62, Servo_DATA_draw_R1, Servo_DATA_draw_R2);  //表针
    u8g2.sendBuffer();
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
  /////////////////////////////////////

  /////////////////////运行一次的启动位（堆空间 固件空间 Flash CPU）
    xuni_key4 = 0;
    xuni_key5 = 0;
    xuni_key7 = 0;
    xuni_key8 = 0;
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

void Heap_DATA(){   //获取(堆)内存参数
    used_Heap_bytes1 = ESP.getHeapSize() - ESP.getFreeHeap();  // 已使用堆内存（字节） 已用内存 = 总内存 - 剩余空内存

    // 转换为KB（1 KB = 1024 字节）//长整数 转 浮点数 // 字节 转 千字节
    total_Heap_kb = (float)ESP.getHeapSize() / 1024.0;  // 强制转换为浮点运算
    free_Heap_kb = (float)ESP.getFreeHeap() / 1024.0;
        //(float)used_Heap_bytes
    used_Heap_kb = (float)used_Heap_bytes1 / 1024.0;     //总使用率 = 已用内存 / 1024.0 
    
    heapUsage = (float)used_Heap_bytes1 / ESP.getHeapSize() * 100; //内存使用率  
}

void sketch_DATA(){ //计算固件存储空间及大小
  freeSketchSpace = ESP.getFreeSketchSpace();//获取固件分区剩余空间（字节）
  sketchSize = ESP.getSketchSize(); //获取当前固件大小（字节）

  freeMB = (float)freeSketchSpace / (1024 * 1024);//MB/KB 转换
  sketchKB = (float)sketchSize / 1024;            //MB/KB 转换
}

void ESP32_ID(){  //芯片型号
  sprintf( part_XH1, "ESP32芯片型号 = %s Rev%d \n", ESP.getChipModel(), ESP.getChipRevision());
  //Serial.println(part_XH1);//调试
  //C语言格式化函数 %s字符串 %d整数
}

void Flash_DATA(){ //Flash存储
  // 读取 Flash 容量（字节）
  flashConfigSize = ESP.getFlashChipSize();    // 配置的容量

  // 转换为 MB（1 MB = 1024×1024 = 1048576 字节）
  flash_ConfigMB = (float)flashConfigSize / 1048576.0;

  Flash_SpeedMHz = ESP.getFlashChipSpeed() / 1000000;//获取闪存芯片运行频率 获取FLASH芯片 速度
  FlashChipMode = ESP.getFlashChipMode() ;//获取闪存芯片模式
}

void CPU_DATA(){  //CPU参数
  CPUChipCores = ESP.getChipCores();//CPU核数
  CPUFreqMHz = ESP.getCpuFreqMHz();//CPU运行频率
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
  u8g2.print("加载OLED&舵机模块");
  Serial.println("=================== 芯片型号 ===================");
    Serial.printf("ESP32 芯片型号 = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  delay(500);
  u8g2.sendBuffer();
    Serial.println("================ 堆内存信息(KB) ================");
    
    u8g2.setCursor(0,60);
    u8g2.print(">");
      Serial.print("总堆内存：");
      Serial.print(total_Heap_kb, 2);  // 保留2位小数
      Serial.println(" KB");
    delay(300);

    u8g2.sendBuffer();
    u8g2.setCursor(20,60);
    u8g2.print(">");
      Serial.print("空闲堆内存：");
      Serial.print(free_Heap_kb, 2);
      Serial.println(" KB");
    delay(300);

    u8g2.sendBuffer();
    u8g2.setCursor(40,60);
    u8g2.print(">");
      Serial.print("已使用堆内存：");
      Serial.print(used_Heap_kb, 2);
      Serial.println(" KB");
    delay(300);
  
    Serial.print("堆内存使用率：");
      Serial.print(heapUsage, 1);  // 保留1位小数
      Serial.println(" %");
  Serial.println("================ 固件存储区信息 ==============");
  
  u8g2.sendBuffer();
  u8g2.setCursor(60,60);
  u8g2.print(">");
  Serial.printf("当前固件大小：%.2f KB\n", sketchKB);
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(80,60);
  u8g2.print(">");
  Serial.printf("剩余可用空间：%.2f MB\n", freeMB);
  Serial.println("=============================================");
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(100,60);
  u8g2.print(">");
    serial_print4();
  delay(500);

  u8g2.sendBuffer();
  u8g2.setCursor(120,60);
  u8g2.print(">");
    serial_print5();
  delay(500);

  u8g2.sendBuffer();
  delay(700);
}


//=========串口打印 调试用===========//可在加载页启用
  void serial_print1(){//调试 串口打印 堆 内存
      Serial.println("================ 堆内存信息(KB) ================");
      Serial.print("总堆内存：");
      Serial.print(total_Heap_kb, 2);  // 保留2位小数
      Serial.println(" KB");

      Serial.print("空闲堆内存：");
      Serial.print(free_Heap_kb, 2);
      Serial.println(" KB");

      Serial.print("已使用堆内存：");
      Serial.print(used_Heap_kb, 2);
      Serial.println(" KB");

      Serial.print("堆内存使用率：");
      Serial.print(heapUsage, 1);  // 保留1位小数
      Serial.println(" %");
      Serial.println("=============================================");
  }
  void serial_print2(){//调试 串口打印 固件
      Serial.println("================ 固件存储区信息 ==============");
      Serial.printf("当前固件大小：%.2f KB（%lu 字节）\n", sketchKB, sketchSize);
      Serial.printf("剩余可用空间：%.2f MB（%lu 字节）\n", freeMB, freeSketchSpace);
      Serial.println("=============================================");
  }
  void serial_print3(){//调试 串口打印 型号
      Serial.println("================== 芯片型号 =================");
      Serial.printf("ESP32 芯片型号 = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
      Serial.println("=============================================");
  }
  void serial_print4(){//调试 串口打印 Flash
    Serial.println(  "=========== ESP32 Flash 容量信息 ============");
    Serial.printf("Flash 容量：%.2f MB（%.0f 字节）\n", flash_ConfigMB, flashConfigSize);
    Serial.printf("Flash 运行频率: %u MHz\n", ESP.getFlashChipSpeed() / 1000000);//获取闪存芯片运行频率 获取FLASH芯片的速度
    Serial.printf("Flash 模式: %u \n", ESP.getFlashChipMode());//获取闪存芯片模式
    Serial.println(  "============================================");

  }
  void serial_print5(){//调试 串口打印 CPU
    Serial.println(   "==================CPU参数===================");
    Serial.printf("CPU核数: %u 核\n", ESP.getChipCores());
    Serial.printf("CPU运行频率: %u \n", ESP.getCpuFreqMHz());
    Serial.println(  "============================================");

  }
//==========调试 符号===============//未使用
  //╬╩╧ ☟ ⏱ ♫ ㎏ g ◁ ◀▷ ▶ <(￣︶￣)↗[GO!] 
  //(→_→)  ^_^ ¬_¬  ◄) )  ◀) ☼ ●♫
  //
  //        ⇣
  //     / ▱ / 
  //      
  //       ┴
//==========fLASH工作模式===========//
  /*
  Flash 常用的工作模式有 4 种：DOUT/DIO/QOUT/QIO
    DOUT:地址为 1 线模式输入，数据为 2 线模式输出
    DIO：地址为 2 线模式输入，数据为 2 线模式输出
    QOUT：地址为 1 线模式输入，数据为 4 线模式输出
    QIO：地址为 4 线模式输入，数据为 4 线模式输出
    如果需要使用 QIO 模式，则需要在选择 Flash 时确认该 Flash 是否支持 QIO 模式。
  */
//=================================//


