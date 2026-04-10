/*==========================================================================
  ==  ===
  ==========================================================================
  //库
    
  //BUG及已解决和代码更改记录():
    1 添加颜色拾取及亮度控制功能
    2 RGB开启/关闭功能
    3 设置模式按钮（六种模式）
    4 目前只有呼吸灯&白灯&彩虹模式添加滑块控制
    5 总共为5个按钮，两个滑块，一个颜色拾取，一个模式显示，再加调试窗口
    6 404为测试调试使用
*/


//////////////////////////////////////////////
#define BLINKER_BLE     // 定义宏以启用Blinker BLE功能
#include <Blinker.h>    // 引入Blinker库

//////////////////////////////////////////////
BlinkerButton Button1("btn-SB1");   //开启关闭RGB
BlinkerButton Button2("btn-SB2");   //模式选择
BlinkerButton Button3("btn-SB3");   //白灯
BlinkerButton Button4("btn-SB4");   //彩虹
BlinkerButton Button5("btn-SB5");   //404调试

BlinkerNumber Number1("num-abc");   //debug 反馈
BlinkerRGB RGB1("col-RGB");         //RGB颜色选择器&亮度控制
BlinkerText Text1("tex-t1");        //模式代码显示
BlinkerText Text2("tex-t2"); //调试用

BlinkerSlider Slider1("sli-SD1"); // 滑动条
BlinkerSlider Slider2("sli-SD2"); // 滑动条2

//////////////////////////////////////////////
int asd = 404;  //错误反馈代码

int RGB_IO = 0; // RGB灯的开关状态，0表示关闭，1表示打开

//模式选择
int mode = 0; // 当前模式，初始为0

//////////////////////////////////////////////
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
    RGB_IO = !RGB_IO; // 切换RGB灯的开关状态
    if (RGB_IO) {
        BLINKER_LOG("RGB打开");
        mode = 7; // 切换到白灯模式
    } else {
        BLINKER_LOG("RGB关闭");
        mode = 0; // 关闭RGB灯时重置模式为0
    }

    Text1.print("模式", mode); // 显示当前模式
}
void button2_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    
    mode++; // 切换到下一个模式
    if (mode > 6) { // 如果模式超过7，重置为0
        mode = 0;
    }

    Text1.print("模式", mode); // 显示当前模式
}
void button3_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    FastLED.clear();
    mode = 7; // 切换到白灯模式
    Text1.print("模式", mode); // 显示当前模式
}
void button4_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    
    mode = 6; // 切换到彩虹模式
    Text1.print("模式", mode); // 显示当前模式
}
void button5_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    Blinker.print(asd);//404

}
void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);
    if (mode == 1 || mode == 2 || mode == 3 || mode == 4 || mode == 5)
    {
      r = r_value;
      g = g_value;
      b = b_value;
      if (mode == 1)
      {
        RGB_bright = bright_value;
      }     
    }
}

void slider1_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
    if (mode == 6)
    {//模式6彩虹速度控制
      SD_TM_map[4] = value; // 将滑动条的值映射到SD_TM_map数组的第5个元素（索引4）
    }
    else if (mode == 7)
    {//模式7白灯亮度控制
      SD_TM_map[5] = value; // 将滑动条的值映射到SD_TM_map数组的第6个元素（索引5），用于白灯亮度控制
    }
    
}
void slider2_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
    if (mode == 2)
    {//模式2控制
      //SD_TM_map[0] = value; 
    }
    else if (mode == 3)
    {//模式3控制
      //SD_TM_map[1] = value;
    }
    else if (mode == 4)
    {//模式4呼吸灯速度控制
      SD_TM_map[2] = value; 
    }
    else if (mode == 5)
    {//模式5控制
      //SD_TM_map[3] = value; 
    }
}
    


//////////////////////////////////////////////
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    //Number1.print(counter);
    //Blinker.print(asd);
}
//////////////////////////////////////////////
void BLE_RUN1()
{
  BLINKER_DEBUG.stream(Serial);
    
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
    
  Blinker.begin();
  Blinker.attachData(dataRead);

  Button1.attach(button1_callback);//RGB开关
  Button2.attach(button2_callback);//模式选择
  Button3.attach(button3_callback);//白灯
  Button4.attach(button4_callback);//彩虹
  Button5.attach(button5_callback);//404调试

  RGB1.attach(rgb1_callback);//RGB颜色选择器&亮度控制

  Slider1.attach(slider1_callback);//滑动条
  Slider2.attach(slider2_callback);//滑动条2

}
//////////////////////////////////////////////
void BLE_control()
{
  Blinker.run();
}





