
#include <DHT.h>                    //引用DHT系列温湿度库

float humi_read, temp_read;

#define DHTPIN 15           //引脚
#define DHTTYPE DHT11       //使用的型号
DHT dht(DHTPIN, DHTTYPE);


void DHT_READ_DATA(){//读取温湿度数据
  float h = dht.readHumidity();//湿度
  float t = dht.readTemperature();//温度
  float hic = dht.computeHeatIndex(t, h, false);//体感温度 未使用
  humi_read = h;
  temp_read = t;
}
