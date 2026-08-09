/*==========================================================================
  == 哔哩哔哩ID：620162563                                                ===
  ==========================================================================
*/

#define _EXTENDED  //使用扩展

#include <WiFi.h>       //wifi库
#include <Settimino.h>  //S7库

const char* ssid     = "你的WiFi名";
const char* password = "你的WIFI密码";

IPAddress plcIP(192, 168, 0, 66); //西门子PLC的IP地址  默认值

S7Client client;

int result;
byte buffer[1]; 

int readResult;

int chaoshiPLCCONNECT = 0;

int wificonnect_biaozhi = 0;
/////////////////////////////////////////////////////////////PLC读写必要的变量
bool bufferI10_R , bufferI11_R;

bool bufferQ00_R , bufferQ01_R , bufferQ02_R , bufferQ03_R;
bool bufferQ00_W , bufferQ01_W ;

bool bufferQ10_R , bufferQ11_R , bufferQ12_R , bufferQ13_R;
bool bufferQ10_W , bufferQ11_W , bufferQ12_W , bufferQ13_W;

byte bufferVW100[2] , bufferVW200[2];
int data_VW100 , data_VW200;
/////////////////////////////////////////////////////////////

void wifi_CONNECT(){  //wifi初始化
  if (wificonnect_biaozhi == 0)
  {
    WiFi.begin(ssid, password); //WIFI初始化
    wificonnect_biaozhi = 1;
  }
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  else if(wificonnect_biaozhi == 1) {
    Serial.println("\nwifi已连接, IP: " + WiFi.localIP().toString());
    wificonnect_biaozhi = 2;
  }
}

void PLC_CONNECT(){ //PLC连接
  
  if (!client.Connected) {//PLC连接检测
    //client.Disconnect();  //关闭PLC连接

    result = client.ConnectTo(plcIP, 0, 1);     //S7-1200/1500 典型的机架号(Rack)=0 槽号(Slot)=1   这里用的是S7-200 SMART系列
    if (result == 0) {
        Serial.println("PLC连接成功！");  //串口打印
    } else {
      static unsigned long lastTime6 = 0;
      if (millis() - lastTime6 > 40000) { //约30s 超时关闭连接PLC开关
          lastTime6 = millis();
          chaoshiPLCCONNECT = !chaoshiPLCCONNECT;//PLC准备链接时未连接的状态显示 带 秒超时
      }
      Serial.print(".");
      //return;
    }
  }
}

void S7_loop() {
  buffer[1];
  readResult = client.ReadArea( S7AreaPA , 0, 0, 1, buffer);  //数据存储到buffer
  
  if (readResult != 0) {
    client.Connected = false;
    Serial.print("连接PLC中");
  }
}

//////////////////////////////////////////////读写
void s7_plc_Q00(){
  //client.ReadBit( S7AreaPA, 0, 0, bufferQ00_R);
  //bufferQ00_W = !bufferQ00_R;
  client.WriteBit( S7AreaPA, 0, 0, bufferQ00_W);  //（字节地址 * 8）+ 位偏移  //可以看库的说明来填写
}
void s7_plc_Q01(){
  //client.ReadBit( S7AreaPA, 0, 9, bufferQ01_R);
  client.WriteBit( S7AreaPA, 0, 1, bufferQ01_W);
}
/////////////////////////////////////////////
void s7_plc_Q10_R(){
  client.ReadBit( S7AreaPA, 0, 8, bufferQ10_R);
  //bufferQ10_W = !bufferQ10_R;
  //client.WriteBit( S7AreaPA, 0, 8, bufferQ10_W);
}
void s7_plc_Q11_R(){
  client.ReadBit( S7AreaPA, 0, 9, bufferQ11_R);
  //client.WriteBit( S7AreaPA, 0, 9, bufferQ11_W);
}
void s7_plc_Q12_R(){
  client.ReadBit( S7AreaPA, 0, 10, bufferQ12_R);
  //client.WriteBit( S7AreaPA, 0, 10, bufferQ11_W);
}
void s7_plc_Q13_R(){
  client.ReadBit( S7AreaPA, 0, 11, bufferQ13_R);
  //client.WriteBit( S7AreaPA, 0, 11, bufferQ11_W);
}

/////////////////////////////////////////////

void s7_plc_vw100(){
  client.ReadArea(S7AreaDB, 1, 100, 2, bufferVW100); 
  data_VW100 = S7.WordAt(bufferVW100 , 0);   //读VW100
}
void s7_plc_vw200(){
  client.ReadArea(S7AreaDB, 1, 200, 2, bufferVW200); 
  data_VW200 = S7.WordAt(bufferVW200 , 0);   //读VW200
}
////////////////////////////////////////////
void s7_plc_I10_R(){
  client.ReadBit( S7AreaPE, 0, 8, bufferI10_R);
}
void s7_plc_I11_R(){
  client.ReadBit( S7AreaPE, 0, 9, bufferI11_R);
}

////////////////////////////////////////////
void s7_plc_Q02_R(){
  client.ReadBit( S7AreaPA, 0, 2, bufferQ02_R);
}
void s7_plc_Q03_R(){
  client.ReadBit( S7AreaPA, 0, 3, bufferQ03_R);
}

