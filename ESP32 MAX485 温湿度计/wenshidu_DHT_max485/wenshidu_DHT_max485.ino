//此方案及代码由  hi同学_cs制作，哔哩哔哩ID：620162563
////使用了硬件串口库，TTL转485模块（芯片为MAX485），其模块传输方向自动识别，省下个引脚
//学习485是为了ESP32菜单能扩展大量的设备做准备。
//几乎所有带有485通讯的而且是ModbusRTU协议的设备都可以连接使用，如温湿度计，空气或者液体压力计等大小型工业设备
//边学边练，制作不易，请珍惜！
///////////////////////
#include <HardwareSerial.h> //硬件串口库
///////////////////////
HardwareSerial RS485Serial(2);  // 使用硬件接口UART端口号2
///////////////////////////////////////////功能码处理
uint8_t sendData[] = {0x02, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xF8};//查询8位温湿度的功能码（根据其设备的说明书而定）
//计算查询功能码其数组元素的个数 数组的元素个数（sendLen） = 总字节数sizeof(sendData) ÷ 单个元素字节数sizeof(sendData[0])  
uint8_t sendLen    = sizeof(sendData) / sizeof(sendData[0]);//sizeof计算变量所占用的字节数
///////////////////////////////////////////
uint8_t recvBuffer[16]; //存储接收的功能码//最大接收16字节
uint8_t recvLen = 0;    //赋值8位功能码
///////////////////////////////////////////
uint16_t tempRaw ;  //读取的温度移位之后的数据
uint16_t humiRaw ;  //读取的湿度以为之后的数据
float tempRead_DATA;//转换成浮点数的温度
float humiRead_DATA;//转换成浮点数的湿度
///////////////////////////////////////////
void setup() {
  Serial.begin(115200); 
  RS485Serial.begin(9600, SERIAL_8N1, 16, 17);  //初始化硬件串口
}
void loop() {
  Read485Write_DATA();  //读取485功能码数据
  Read485_DATA();       //读取并解析功能码数据
}
//////////////////////////////////////////
void Read485Write_DATA(){//读取485功能码数据
  RS485Serial.write(sendData, sendLen); //写入查询的功能码
  RS485Serial.flush();                  //串口等待，确保发送的功能码完整
  printHex(sendData, sendLen);          //打印出十六进制的功能码
}
void Read485_DATA(){//读取并解析功能码数据
  recvLen = RS485Serial.readBytes(recvBuffer, sizeof(recvBuffer));//readBytes批量读取字节
  //解析读回的功能码
  if (recvLen == 9) {//判断是否传回8位9个字节并解析和打印
    Serial_print3();//收到功能码之后打印出来
    //验证读取的功能码    //02从机地址         //04读输入寄存器           
    if (recvBuffer[0] == 0x02 && recvBuffer[1] == 0x04 && recvBuffer[2] == 0x04) {
      
      //从其485设备的说明手册中得出每个寄存器数值为16位无符号整数

      //从功能码中分析合并温度的数据
      //总16位：左移8位之后把另外8位补上组成16位的温度数据
      //注意：这里有个大端模式的协议：是多字节数据中，高位字节在前（先传输），低位字节在后（后传输）的传输模式
      tempRaw = (recvBuffer[3] << 8) | recvBuffer[4]; //合并高8位和低8位 //<<左移 |组合 
      tempRead_DATA = tempRaw / 10.0; //除以10.1，输出成浮点数也就是小数

      //从功能码中分析合并湿度数据
      humiRaw = (recvBuffer[5] << 8) | recvBuffer[6];
      humiRead_DATA = humiRaw / 10.0;
      Serial_print1();
    } 
    else {//如读错误其打印
      Serial.println("格式错误：地址/功能码/数据长度不符");
    }
  } 
  else {//如功能码读错误
    Serial_print2();
  }
}
//////////////////////////////////////////
void Serial_print1(){
  Serial.print("分析功能码: 温度 = ");
  Serial.print(tempRead_DATA, 1);  //1：保留1位小数
  Serial.print("℃, 湿度 = ");
  Serial.print(humiRead_DATA, 1);
  Serial.println("%");
}
void Serial_print2(){
  Serial.print("功能码长度错误,应为9字节,实际收到");
  Serial.print(recvLen);
  Serial.println("字节）");
}
void Serial_print3(){
  Serial.print("收到功能码: ");
  printHex(recvBuffer, recvLen);  //串口打印成十六进制的
}
//打印十六进制数组  //  打印的数组   打印要读取数组的长度（多少字节）
void printHex(uint8_t* data, uint8_t len) {//可移植，后直接使用
  for (int i = 0; i < len; i++) {//循环查询
    if (data[i] < 0x10) Serial.print("0");  //单个字节补0
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
