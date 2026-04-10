/*==========================================================================
  ==  ===
  ==========================================================================
  //库
    FastLED.h         LED灯带控制库 

  //BUG及已解决和代码更改记录():
    1 还有四个模式未使用，留调试用，后续可以添加到UI界面中
    2 
*/

///////////////////////////////////////////////////////////////
#include "FastLED.h"             // 引入FastLED库

///////////////////////////////////////////////////////////////
#define NUM_LEDS 194               // LED灯珠总数量
#define DATA_PIN 27              // ESP32输出控制信号引脚
#define LED_TYPE WS2812          // LED灯珠类型
#define COLOR_ORDER GRB          // RGB灯珠中红色、绿色、蓝色LED的排列顺序，尝试RGB
 
uint8_t max_bright = 255;        // 设置最大亮度，范围为0-255  

CRGB leds[NUM_LEDS];             // 建立光带leds
///////////////////////////////////////////////////////////////
// 定义全局变量r、g、b，用于存储RGB颜色值
uint8_t r = 128;
uint8_t g = 128;
uint8_t b = 128;

uint8_t RGB_bright = 128;        // 控制亮度，范围为0-255  

#define NUM_LEDS2 194              // 控制LED灯珠数量

//线性转换
int SD[10] = {0, 0, 0, 0, 0}; // 转换后的数值数组
int SD_TM_map[10] = {2000000, 1800000, 5, 10, 10, 1000}; // 定时器数组

////////////////////////////////////////////////////////////////
void RGB_RUN1() { // 初始化LED灯带
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带 
  FastLED.setBrightness(max_bright);                            // 设置光带亮度
  FastLED.clear(); // 清除光带颜色，准备下一次更新
  FastLED.show(); // 更新LED灯的状态，显示颜色
}
////////////////////////////////////////////////////////////////
void RGB_control_1() {//控制全部LED灯的颜色
  //FastLED.showColor(CRGB::Red); // 显示红色
  //FastLED.showColor(CRGB(r, g, b));  //控制所有的LED灯的颜色为RGB值
  //FastLED.showColor(CRGB::Blue, 128);    // 蓝色，亮度 50% (0-255)
  
  FastLED.showColor(CRGB(r , g , b), RGB_bright);    // rgb，亮度 50% (0-255)
  //FastLED.show(); //  更新LED灯的状态，显示颜色

  if(RGB_bright > 255){
      RGB_bright = 255;//RGB灯亮度上限
  }
  if(RGB_bright < 0){
      RGB_bright = 0;//RGB灯亮度下限
  }
}
////////////////////////////////////////////////////////////////

void RGB_control_2() {//控制灯逐个亮，立即灭
  SD[0] = map(SD_TM_map[0], 0, 3000000, 0, 3000000); // 线性转换，调整TM的范围和速度
  for (int i = 0; i < NUM_LEDS2; TM1++) {
    if (TM1 >= 3000000) {//SD[0]
      i++; // 递增LED索引
      TM1 = 0; // 计时器间隔脉冲重置（清零）
      leds[i] = CRGB(r, g, b);
      FastLED.show();
      if (i == NUM_LEDS2 - 1)  
      {
        FastLED.clear();
      }
    }
  }
}

void RGB_control_3() {//控制灯逐个亮，逐个灭0~1800000
  SD[1] = map(SD_TM_map[1], 0, 3000000, 0, 1800000); // 线性转换，调整TM的范围和速度
  for (int i = 0; i < NUM_LEDS2; TM2++) {//逐个亮
    if (TM2 >= 1700000) {//SD[1]
      i++; // 递增LED索引
      TM2 = 0; // 计时器间隔脉冲重置（清零）
      leds[i] = CRGB(r, g, b);
      FastLED.show();
    }
  }
  for (int i = NUM_LEDS2; i > 0; TM2--) {//逐个灭
    if (TM2 <= 1700000 * -1) {//SD[1]
      i--; // 递减LED索引
      TM2 = 0; // 计时器间隔脉冲重置（清零）
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
  }
}

void RGB_control_4() {//呼吸灯0~8
    SD[2] = map(SD_TM_map[2], 0, 3000000, 0, 8); // 线性转换，调整TM的范围和速度
    for (int brightness = 0 ; brightness <= 128; TM3++) {
      //FastLED.setBrightness(brightness);
      FastLED.showColor(CRGB(r, g, b), brightness);
      if (TM3 >= SD[2]){
        TM3 = 0; // 计时器间隔脉冲重置（清零）
        brightness++; // 递增亮度
      }
    }
  
    for (int brightness = 128; brightness >= 0; TM3--) {
      //FastLED.setBrightness(brightness);
      FastLED.showColor(CRGB(r, g, b), brightness);
      if (TM3 <= SD[2] * -1){
        TM3 = 0; // 计时器间隔脉冲重置（清零）
        brightness--; // 递减亮度
      }
    }
}

int i1 = 0; // 从第一个LED开始
void RGB_control_5() {// IF版本 单个灯循环往复 0~10000
  SD[3] = map(SD_TM_map[3], 0, 3000000, 0, 10000); // 线性转换，调整TM的范围和速度
  TM4++; // 递增计时器间隔脉冲
  if (TM4 >= 8000) // 0 ~10000，调整单个灯循环往复的速度SD[3]
  {
    TM4 = 0; // 计时器间隔脉冲重置（清零）
    leds[i1] = CRGB(r, g, b);
    FastLED.show();
    FastLED.clear();
    if (i1 < NUM_LEDS - 1) {
      i1++; // 递增LED索引
    } else {
      i1 = 0; // 重置索引，重新开始循环
    }
  }
}

uint8_t startHue = 0;  // 全局变量，用于跟踪起始色调
void RGB_control_6() { // 彩虹渐变  0~4000
  //SD[4] = map(SD_TM_map[4], 0, 4000, 0, 4000); // 线性转换，调整TM的范围和速度

  TM5++; // 递增计时器间隔脉冲
  if (TM5 >= SD[4]) // 0 ~4000，调整彩虹渐变的速度
  {
    TM5 = 0; // 计时器间隔脉冲重置（清零）
    fill_palette(leds, NUM_LEDS, startHue, 255 / NUM_LEDS, RainbowColors_p, 255, LINEARBLEND);
    //色调范围: 在 FastLED 中，色调（hue）是一个 0-255 的值，对应 0-360 度的颜色轮（0=红色，85=绿色，170=蓝色，255=红色）。
    //startHue: 起始色调索引（uint8_t 类型，范围 0-255），表示从调色板的哪个位置开始填充。每次调用时 startHue 会递增，实现动态移动效果。
    startHue += 1;  // 每次调用增加起始色调，实现移动效果
    if (startHue >= 255) startHue = 0;  // 循环重置
    FastLED.show();

    //fill_palette (led名称, 光珠数字,   起始颜色序号, 光珠间颜色差, palette色板名,   亮度,  TBlendType blendType)
    /*
    RainbowColors_p           彩虹色
    RainbowStripeColors_p     彩虹条纹颜色
    */
   if (SD[4] > 10){
      SD[4] = 10;//彩虹渐变频率上限
   }
   else if (SD[4] < 0){
      SD[4] = 0;//彩虹渐变频率下限
   }
   
  }
}




////////////////////////////////////////////////////////////////
//////////////////调试/测试/控制///////////////////
/*
//////////////////////////////////////////////
void RGB_control_5() {//控制灯单个闪烁 
    for (TM5 = 0 ; TM5 <= 100; TM5++) {
      leds[0] = CRGB(r, g, b);
      FastLED.show();
    }
  
    for (TM5 = 100; TM5 >= 0; TM5--) {
      FastLED.clear();
      FastLED.show();
    }
}
//////////////////////////////////////////////

void RGB_control_7() {// 单个灯循环往复 delay版本
  for (int i = 0; i < NUM_LEDS; i++) {
    delay(50);     // 等待10毫秒
    leds[i] = CRGB(r, g, b);
    FastLED.show();
    FastLED.clear();
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    delay(50);     // 等待10毫秒
    leds[i] = CRGB(r, g, b);
    FastLED.show();
    FastLED.clear();
  }
}

void RGB_control_2() {//控制灯单个亮 加
  for (int i = 0; i < NUM_LEDS; i++) {
    delay(200);     // 等待1000毫秒
    leds[i] = CRGB(255, 255, 255);
    FastLED.show();
    FastLED.clear();
  }
}

void RGB_control_3() {//控制灯单个亮 减
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    delay(200);     // 等待1000毫秒
    leds[i] = CRGB(255, 255, 255);
    FastLED.show();
    FastLED.clear();
  }
}

//////////////////////////////////////////////
*/





