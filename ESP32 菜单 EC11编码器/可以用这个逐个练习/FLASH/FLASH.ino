void setup() {
  Serial.begin(115200);

  unsigned long flashConfigSize = ESP.getFlashChipSize();    //读取Flash容量

  float flashConfigMB = (float)flashConfigSize / 1048576.0;//转换为 MB（1 MB = 1024×1024 = 1048576 字节）

  Serial.println("===== ESP32 Flash 容量信息 =====");
  Serial.printf("Flash 容量：%.2f MB(%.0f 字节)\n", flashConfigMB, flashConfigSize);
  Serial.printf("Flash 运行频率: %u MHz\n", ESP.getFlashChipSpeed() / 1000000); //获取FLASH芯片的速度
  Serial.printf("Flash 模式: %u \n", ESP.getFlashChipMode());                   //获取闪存芯片模式
  Serial.println("===============================");
}

void loop() {}

/*
Flash 常用的工作模式有 4 种：DOUT/DIO/QOUT/QIO
  DOUT:地址为 1 线模式输入，数据为 2 线模式输出
  DIO：地址为 2 线模式输入，数据为 2 线模式输出
  QOUT：地址为 1 线模式输入，数据为 4 线模式输出
  QIO：地址为 4 线模式输入，数据为 4 线模式输出
  如果需要使用 QIO 模式，则需要在选择 Flash 时确认该 Flash 是否支持 QIO 模式。
*/