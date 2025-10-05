void setup() {
    Serial.begin(115200);

    //ESP.getChipModel() ——  获取cpu模式
    //ESP.getChipCores() ——  获取cpu核数
    //ESP.getCpuFreqMHz() —— 获取cpu运行频率
    //ESP.getCycleCount() ——  获取CPU周期计数

    //ESP.restart()重新启动CPU

    Serial.println("============CPU参数==============");
    Serial.printf("CPU模式: %s \n", ESP.getChipModel());
    Serial.printf("CPU核数: %u 核\n", ESP.getChipCores());
    Serial.printf("CPU运行频率: %u \n", ESP.getCpuFreqMHz());
    Serial.printf("CPU周期计数: %u \n", ESP.getCycleCount());
    Serial.println("=================================");
    //Serial.printf(": %u \n", );
    delay(2500);
}

void loop() {
  Serial.printf("CPU周期计数: %u \n", ESP.getCycleCount());
  delay(500);
}
