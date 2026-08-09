/*==========================================================================
  == 哔哩哔哩ID：620162563                                                ===
  ==========================================================================
*/

///////////////////////////////////////////////////////////////
#include "FastLED.h"             // 引入FastLED库

///////////////////////////////////////////////////////////////
#define NUM_LEDS 60               // LED灯珠总数量
#define DATA_PIN 23              // ESP32输出控制信号引脚
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

#define NUM_LEDS2 60              // 控制LED灯珠数量

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
