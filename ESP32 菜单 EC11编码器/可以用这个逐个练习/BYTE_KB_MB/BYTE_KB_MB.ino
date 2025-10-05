void setup() {
  Serial.begin(115200);
  ///////////////////////////////转换成MB
    unsigned long totalHeap_bytes = ESP.getHeapSize();                //总堆内存（字节）
    unsigned long freeHeap_bytes = ESP.getFreeHeap();                 //空闲堆内存（字节）
    unsigned long usedHeap_bytes = totalHeap_bytes - freeHeap_bytes;  //已使用堆内存（字节）

    //转换 MB（1 MB = 1024×1024 = 1048576 字节）
    float totalHeap_mb = (float)totalHeap_bytes / 1048576.0;
    float freeHeap_mb = (float)freeHeap_bytes / 1048576.0;
    float usedHeap_mb = (float)usedHeap_bytes / 1048576.0;

    float heapUsage = (float)usedHeap_bytes / totalHeap_bytes * 100;//使用率 百分比

    Serial.println("================ 堆内存信息(MB) ================");
    Serial.print("总堆内存：");
    Serial.print(totalHeap_mb, 2);  //保留2位小数
    Serial.println(" MB");

    Serial.print("空闲堆内存：");
    Serial.print(freeHeap_mb, 2);
    Serial.println(" MB");

    Serial.print("已使用堆内存：");
    Serial.print(usedHeap_mb, 2);
    Serial.println(" MB");

    Serial.print("堆内存使用率：");
    Serial.print(heapUsage, 1);
    Serial.println(" %");
    Serial.println("=============================================");

}

void loop() {
    /*
    if(data1 > 9000){
    Serial.println("内存信息：");
    Serial.print("总堆内存：");
    Serial.print(ESP.getHeapSize());
    Serial.println(" 字节");
    
    Serial.print("空闲堆内存：");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" 字节");
    
    Serial.print("最大可分配堆内存：");
    Serial.print(ESP.getMaxAllocHeap());
    Serial.println(" 字节");
    */
  
}