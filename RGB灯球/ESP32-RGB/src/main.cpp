/*==========================================================================
  ==此代码方案由 hi同学_cs制作 哔哩哔哩ID：620162563        版本：V1.0      ===
  ==========================================================================
  //库&子程序
    Arduino.h               // Arduino核心库
    RGB-2812.h              // RGB灯带控制
    Blinker-RGB-BLE.H       // Blinker蓝牙通信
    SimplifiedTimer.h       //引用多任务库，防程序阻塞

  //BUG及已解决&代码更改记录():
    1 非阻塞定时TM系列 （TM1, TM2, TM3, TM4, TM5......）
    2 RGB 六钟模式 五个动画（留下调试的动画代码）
    3 彩虹模式加入速度控制
    4 启用多任务库（虽然没啥用）
    5 0模式下RGB灯关闭，其他模式下RGB灯颜色更新，准备下一次更新
    6 本项目代码的部分注释由GPT自动生成（AI做注释，哈哈哈！好强的术语，是个不错的辅助工具）
    7 IDE+platformIO混合开发（点灯科技库可能不支持platform，暂时没找原因）
    8 V1.0
*/

///////////////////////////////////////
//定时器（非阻塞）
int TM1 = 0;  // 计时器间隔脉冲 0~2000000
int TM2 = 0;  // 计时器间隔脉冲 0~1800000
int TM3 = 0;  // 计时器间隔脉冲 0~5
int TM4 = 0;  // 计时器间隔脉冲 0~5000
int TM5 = 0;  // 计时器间隔脉冲 0~800       彩虹渐变

///////////////////////////////////////
#include <Arduino.h>            // Arduino核心库
#include "RGB-2812.h"           // RGB灯带控制
#include "Blinker-RGB-BLE.H"    // Blinker蓝牙通信
#include <SimplifiedTimer.h>        //引用多任务库，防程序阻塞
///////////////////////////////////////
SimplifiedTimer timer1;
SimplifiedTimer timer2;

///////////////////////////////////////
//串口打印
void serial_print1() {// 打印RGB值
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.println(b);
}
void serial_print2() {// 打印LED灯的数量和数据引脚
  Serial.print("数量: ");
  Serial.print(NUM_LEDS);
  Serial.print(" 数据引脚: ");
  Serial.print(DATA_PIN);
}

///////////////////////////////////////

void RGB_control() {//RGB控制
  if (mode == 1)
  {//控制全部LED灯的颜色
    RGB_control_1();  // 控制全部LED灯的颜色
  }
  else if (mode == 2)
  {//控制灯逐个亮,立即灭
    RGB_control_2();  // 控制灯逐个亮,立即灭
  }
  else if (mode == 3)
  {//控制灯逐个亮，逐个灭
    RGB_control_3();  // 控制灯逐个亮，逐个灭
  }
  else if (mode == 4)
  {//呼吸灯
    RGB_control_4();  // 呼吸灯
  }
  else if (mode == 5)
  {//单个灯循环往复
    RGB_control_5();  // 单个灯循环往复 IF版本
  }
  else if (mode == 6)
  {//彩虹渐变
    RGB_control_6();  // 彩虹渐变
  }
  else if (mode == 7)
  {//白灯
    r = 255; g = 255; b = 255; // 白灯模式
    RGB_bright = map(SD_TM_map[5], 0, 4000, 0, 255); // 将滑动条的值映射到RGB亮度范围
    RGB_control_1();  // 

  }
  else if (mode == 0)
  {//RGB灯关闭
    FastLED.clear(); // 其他模式清除LED灯颜色，准备下一次更新
    FastLED.show(); // 更新LED灯的状态，显示颜色
  }

//////RGB_control_2();  // 控制灯单个亮 加
//////RGB_control_3();  // 控制灯单个亮 减
//////RGB_control_5();  // 控制灯单个闪烁
//////RGB_control_7();  // 单个灯循环往复 带延时版本FOR
}

void BLE_control_1() {//控制蓝牙通信
  BLE_control(); // 控制蓝牙通信
  //BLINKER_DEBUG.stream(Serial);
}

///////////////////////////////////////
void setup() {
  Serial.begin(115200);

  serial_print1();// 打印RGB值
  serial_print2();// 打印LED灯的数量和数据引脚

  RGB_RUN1(); //RGB初始化
  BLE_RUN1(); //Blinker蓝牙初始化

  timer1.setInterval(1); // 每 500ms 触发一次
  timer2.setInterval(2); // 每 500ms 触发一次

}

void loop() {
  if (timer1.isReady()) {
    RGB_control();  // RGB控制
    timer1.reset(); // 重置，开始下一次计时
  }
  if (timer2.isReady()) {
    BLE_control_1();  // 控制蓝牙通信
    timer2.reset(); // 重置，开始下一次计时
  }
}





