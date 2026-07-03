/*==========================================================================
  ===                           哔哩哔哩ID：620162563                     ===
  ==========================================================================
*/


//////////////////////////////////////////////
//#include <Arduino.h>
#define BLINKER_BLE     // 定义宏以启用Blinker BLE功能
#include <Blinker.h>    // 引入Blinker库

//////////////////////////////////////////////
BlinkerButton Button5("btn-SB5");   //404调试

BlinkerNumber Number1("num-abc");   //debug 反馈

BlinkerNumber TextV("tex-V");  //电压
BlinkerNumber TextA("tex-A");  //电流
BlinkerNumber TextW("tex-W");  //功率

BlinkerSlider Slider1("sli-PWM1");   // PWM

//////////////////////////////////////////////
int asd = 404;  //错误反馈代码

int PWM_DATA1;


//////////////////////////////////////////////
void button5_callback(const String & state) //调试404
{
    BLINKER_LOG("get button state: ", state);
    Blinker.print(asd);//404

}

void slider1_callback(int32_t value)    //滑条 PWM
{
    BLINKER_LOG("get slider value: ", value);
    PWM_DATA1 = value;
    //BLINKER_LOG(PWM_DATA1);
}   

//////////////////////////////////////////////
void dataRead(const String & data)  //返回值
{
    BLINKER_LOG("Blinker readString: ", data);
    //Number1.print(counter);
    //Blinker.print(asd);
    //Text1.print("", ); // 
}
//////////////////////////////////////////////

void BLE_RUN1() //初始化
{
  BLINKER_DEBUG.stream(Serial);
    
  Blinker.begin();
  Blinker.attachData(dataRead);

  Button5.attach(button5_callback);//404调试

  Slider1.attach(slider1_callback);//滑条 PWM
}
//////////////////////////////////////////////
void BLE_control()//BLE运行
{
  Blinker.run();
}










