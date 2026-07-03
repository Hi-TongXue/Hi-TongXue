/*==========================================================================
  == 哔哩哔哩ID：620162563                                                ===
  ==========================================================================
*/

#include <Arduino.h>
#include <INA226.h>
#include <Wire.h>

INA226 ina226(0x40 , &Wire1); // I2C地址0x40

struct{
    float voltage;// = ina226.getBusVoltage(); // 总线电压(V)
    float current;// = ina226.getCurrent();    // 负载电流(A)
    float power;// = ina226.getPower();        // 负载功率(W)
    float shuntV;// = ina226.getShuntVoltage();// 分流电压(mV)
} INA226_data;


void INA226_setup() {
  Wire1.begin(22, 23); // 初始化I2C通信，SDA引脚为22，SCL引脚为23

  Serial.begin(115200); 
  if(!ina226.begin()){
    Serial.println("INA226初始化失败!");
    while(1);
  }

  ina226.setMaxCurrentShunt(0.82, 0.0819); // 设置最大电流为0.82A，分流电阻为0.0819Ω（根据实际电路调整）I = U/R 
  ina226.setAverage(INA226_16_SAMPLES);// 设置平均次数为16次

  Serial.println("INA226 电压&电流&功率测量");
}

void INA226_loop() {
  INA226_data.voltage = ina226.getBusVoltage(); // 总线电压(V)
  INA226_data.current = ina226.getCurrent();    // 负载电流(A)
  INA226_data.power = ina226.getPower();        // 负载功率(W)
  INA226_data.shuntV = ina226.getShuntVoltage();// 分流电压(mV)

  if (INA226_data.shuntV < 0)
  {
    INA226_data.shuntV = 0.00;
  }

  if (INA226_data.current < 0)
  {
    INA226_data.current = 0.00;
  }
}


//================注意✳：电阻的丝印极其重要（丝印可能与实际不符）=======================//
// 电压0~36V  
/*  Imax = 81.92mV ÷ 实际分流电阻 (Rshunt)
目标量程	推荐分流电阻	最大电流	                   校准代码       	                                电流分辨率
0.82A	    0.0819Ω	    81.92mV ÷ 0.0819Ω ≈ 1.000A	ina226.setMaxCurrentShunt(0.82, 0.0819, false);	  25.0μA
1A	      0.0819Ω	    1.000A	                    ina226.setMaxCurrentShunt(1.0, 0.0819, false);	  30.5μA
2A	      0.04Ω	      2.048A	                    ina226.setMaxCurrentShunt(2.0, 0.04, false);	    61.0μA
5A	      0.016Ω	    5.12A	                      ina226.setMaxCurrentShunt(5.0, 0.016, false);	    152.6μA	
10A	      0.008Ω	    10.24A	                    ina226.setMaxCurrentShunt(10.0, 0.008, false);	  305.2μA
20A	      0.004Ω	    20.48A	                    ina226.setMaxCurrentShunt(20.0, 0.004, false);	  610.4μA


< 0.8 A	    0.1 Ω	  精度最佳
0.8 ~ 2 A	  0.05 Ω	平衡量程和分辨率
2 ~ 4 A	    0.02 Ω	常用 USB PD
4 ~ 8 A	    0.01 Ω	标准模块（如 INA226 模块自带）
8 ~ 16 A	  0.005 Ω	需考虑电阻功率
16 ~ 40 A	  0.002 Ω	建议使用 4 端子开尔文连接
40 ~ 80 A	  0.001 Ω	必须用大尺寸金属片电阻
*/






