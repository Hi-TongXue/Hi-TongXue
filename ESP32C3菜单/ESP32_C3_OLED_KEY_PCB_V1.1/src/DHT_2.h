#include <Wire.h>
#include <DHT20.h>                    //引用DHT系列温湿度库

float humi_read2, temp_read2;

#define SCL_DHT 6
#define SDA_DHT 5
DHT20 DHT;

void DHT_setup(){
  Wire.begin(SDA_DHT, SCL_DHT);
  DHT.begin();
}

void DHT_READ_DATA_2(){//读取温湿度数据
  DHT.read();
  float h = DHT.getHumidity();//湿度
  float t = DHT.getTemperature();//温度
  humi_read2 = h;
  temp_read2 = t;
}
