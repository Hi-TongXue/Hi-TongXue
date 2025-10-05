void setup() {
   Serial.begin(115200); 
   
   //获取芯片型号及版本
   Serial.printf("ESP32 芯片型号 = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
}

void loop() {}
