void setup() {
  Serial.begin(115200);

  unsigned long freeSketchSpace = ESP.getFreeSketchSpace();//读取剩余容量
  unsigned long sketchSize = ESP.getSketchSize();//读取当前容量大小

  float freeMB = (float)freeSketchSpace / (1024 * 1024);//MB/KB转换
  float sketchKB = (float)sketchSize / 1024;

  Serial.println("===== 程序存储区信息 =====");
  Serial.printf("当前程序大小：%.2f KB(%lu 字节)\n", sketchKB, sketchSize);
  Serial.printf("剩余可用空间：%.2f MB(%lu 字节)\n", freeMB, freeSketchSpace);
}

void loop() {}  
