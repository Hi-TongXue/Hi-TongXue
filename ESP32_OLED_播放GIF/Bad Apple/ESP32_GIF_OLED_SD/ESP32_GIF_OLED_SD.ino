//
//此套代码作用是ESP32通过读取SD卡让GIF文件在OLED上显示出来，不是gif转C哈，用SD卡的这种方式比在代码里写GIF的C文件要简单多
//代码的存储量也小些
//此代码由本UP+AI搜索组合而成，边学边写边查资料，代码库的文件里有GIF各种转换的HTMI快捷网站，需要的可以看一看
//代码注释不易，分享给大家学习，且珍惜!
//新BUG：处理透明色有问题，可以把GIF文件转成黑白且透明（注意透明度）就可以了。
/////////////////////////////////////////////////库引用
  #include <AnimatedGIF.h>            //通用图形库：OLED、TFT、LCD //起图形显示接口和绘图作用
  #include <SPI.h>                    //硬件接口库
  #include <Adafruit_GFX.h>           //GIF库     //解析GIF
  #include <Adafruit_SSD1306.h>       //OLED库  驱动芯片SSD1306
  #include <SD.h>                     //SD卡库

///////////////////////////////////////////////OLED接口
  #define OLED_SDA       21
  #define OLED_SCL       22
  #define OLED_WIDTH     128  //宽
  #define OLED_HEIGHT    64   //高
///////////////////////////////////////////////SD模块接口
  #define SD_CS_PIN      5
  #define SD_SCK_PIN     18
  #define SD_MISO_PIN    19
  #define SD_MOSI_PIN    23
/////////////////////////////////////////////////初始化及通道映射
  Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);      //在其中&：取地址运算符，-1是OLED的RST不使用
  AnimatedGIF gif;                                                //创建类也就是实例，变量名是gif
  File f;                                                         //创建类，变量名是f
  bool needClearScreen = true;                                    //标记是否需要清屏 为真清屏，为假停
///////////////////////////////////GIF关闭和打开
  void *GIFOpenFile(const char *fname, int32_t *pSize) {//回调，通过 SD 卡文件系统打开指定的 GIF 文件//fname文件路径 //pSize返回打开文件的大小
    f = SD.open(fname);     //读取指定文件，读取GIF文件
    if (f) {
      *pSize = f.size();    //f.size() 获取文件总字节数，就是GIF大小
      return (void *)&f;    //返回 f 的地址（转换为 void* 类型）
    }
    return NULL;    //如打开GIF失败返回NULL
  }
  void GIFCloseFile(void *pHandle) {  //回调，关闭GIF文件，避免内存泄漏或文件占用 //pHandle为要关闭的GIF文件地址
    File *f = (File *)pHandle;//等效：File *f = static_cast<File *>(pHandle);  //要关闭功能先转换类型，静态类型转换： void* 转换为 File*
    if (f != NULL) {                  //检测是否传回的地址正确
      f->close();                     //关闭通过指针 f 指向的文件，关闭GIF文件及占用的系统资源  //->：通过指针访问对象的成员函数或成员变量
    }
  }
///////////////////////////////////GIF读取及通道数据转换
  //pFile为GIF文件的信息大小，文件位置等 //pBuf为读取到的GIF数据//iLen为读取的字节数，读取了多少数据
  int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {//回调，GIF 解码器获取数据的 “通道”，返回读取GIF的字节数
    int32_t iBytesRead = iLen;                      //记录实际读取的字节数
    File *f = static_cast<File *>(pFile->fHandle);  //类型转换
    //pFile->iSize：文件总大小//当前文件读取指针的位置
    if ((pFile->iSize - pFile->iPos) < iLen) {//计算实际可读取的最大字节数 //目的是要读取还未读的数据
      iBytesRead = pFile->iSize - pFile->iPos - 1;    //只能读取 剩余字节数 - 1
    }
    if (iBytesRead <= 0) {                            //若没有数据读返回（回调）为零
      return 0;
    }

    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);  //读取 iBytesRead 字节的数据，存入 pBuf 缓冲区
    pFile->iPos = f->position();                      //获取读取后的文件指针位置，更新到 pFile->iPos
    return iBytesRead;                                //最终返回实际读取的字节数
  }            
  int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {//读取gif到位置 //移动后的实际文件指针位置//iPosition为目标位置
    File *f = static_cast<File *>(pFile->fHandle);      //类型转换
    f->seek(iPosition);                                 //移动GIF文件到指定位置
    pFile->iPos = (int32_t)f->position();               //获取移动后的实际位置
    return pFile->iPos;                                 //返回实际位置
  }
///////////////////////////////////GIF绘制
  void GIFDraw(GIFDRAW *pDraw) {//解析并绘制GIF
    //在绘制新帧的第一行前清屏
    if (needClearScreen && pDraw->y == 0) { //pDraw 是当前帧绘制信息的指针为0时清屏设为假取消清屏 //pDraw->y 表示当前帧的绘制起始 Y 坐标
      oled.clearDisplay();      //清屏
      needClearScreen = false;  //设为真
    }

    /*以下代码作用
      像素数据的位置（pixelBuffer）
      绘制的坐标    （frameX、frameY）
      绘制的范围    （lineWidth）
      透明效果的控制（transparentColor）
    */
    uint8_t *pixelBuffer = pDraw->pPixels; //pDraw->pPixels当前帧像素数据的指针//pixelBuffer存储了所有每帧图像的像素颜色值
    int frameX = pDraw->iX;               //GIF图像的起始X坐标//frameX用于记录横向偏移量，确保帧绘制在正确的水平位置

    //frameY为当前帧在屏幕上的实际起始 Y 坐标，确保帧绘制在正确的垂直位置
    int frameY = pDraw->iY + pDraw->y;    //pDraw->iY当前帧的基础 Y 坐标//pDraw->y当前扫描线在帧内的相对 Y 偏移量 
    int lineWidth = pDraw->iWidth;        //pDraw->iWidth 表示当前帧的宽度

    //绘制时，若像素值等于 transparentColor，则不绘制该像素（保持背景可见），实现 GIF 的透明效果
    uint8_t transparentColor = pDraw->ucTransparent; //pDraw->ucTransparent当前帧的透明色索引

    /*以下代码作用
      坐标转换：将帧内的相对像素位置转换为屏幕上的绝对坐标。
      边界裁剪：只绘制屏幕范围内的像素，避免越界。
      透明处理：只绘制非透明像素，保留背景或上一帧的内容，实现 GIF 的透明效果
    */
    //GIF帧像素数据绘制到OLED//同时处理边界和透明效果
    for (int x = 0; x < lineWidth; x++) {//循环遍历当前行的所有像素 //lineWidth是当前GIF帧的宽度 //从 x=0 到 x=lineWidth-1
      int screenX = frameX + x;          //像素在屏幕上的X坐标
      int screenY = frameY;              //像素在屏幕上的Y坐标（当前行的Y坐标固定）

      //检查像素是否在屏幕范围内 //确保只绘制屏幕可见范围内的像素
      if (screenX >= 0 && screenX < OLED_WIDTH && screenY >= 0 && screenY < OLED_HEIGHT) {
        // 优化：明确区分显示和不显示的像素
        if (pixelBuffer[x] != transparentColor) {       //处理非透明像素（核心）//pixelBuffer[x]是当前像素的颜色索引值
          //若不等于透明色索引transparentColor，则调用oled.drawPixel()在屏幕的(screenX, screenY)位置绘制一个白色像素
          oled.drawPixel(screenX, screenY, SSD1306_WHITE);
        } else if (pixelBuffer[x] == transparentColor) {//可选 //处理透明像素，可以显式绘制黑色（根据GIF特性调整）
          //若像素是透明色（pixelBuffer[x] == transparentColor），默认不绘制
          oled.drawPixel(screenX, screenY, SSD1306_BLACK);//绘制黑色 //透明的设计目的就是让下层内容可见（如 GIF 动画的背景）
        }
      }
    }
  }
///////////
void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);//SD接口初始化

  Serial.print("初始化SD卡中...");
  if (!SD.begin(SD_CS_PIN)) {                         //SD模块是否连接并初始化
    Serial.println("啊偶~失 败 了！");
    while (1);//连接失败 停止
  }
  Serial.println("嘿嘿！成功了！");

  Serial.print("初始化OLED中...");
  Wire.begin(OLED_SDA, OLED_SCL);                     //OLED接口初始化
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {      //检测OLED是否连接并初始化
    Serial.println("偶~不~失 败 了！");
    while (1);//连接失败 停止
  }
  //oled.invertDisplay(true);//黑白反转
  oled.clearDisplay();                //清屏
  oled.setTextColor(SSD1306_WHITE);   //设置文字颜色
  oled.setCursor(0, 0);               //设置文字位置
  oled.println("Read_GIF...");      //打印文字
  oled.display();                     //清屏
  delay(2000);

  gif.begin(LITTLE_ENDIAN_PIXELS);    //GIF解码初始化
}

void loop() {
  const char *gifPath = "/test.gif";                // *是指针声明符  //访问SD卡中的GIF主文件名和扩展名
  Serial.printf("尝试打开GIF文件中:%s\n", gifPath);
  /////////////////////////////////////////////////////////////////////循环执行GIF读取解析及绘制
              //硬件接口 回调,打开GIF||回调关闭打开的GIF||读取GIF    读取到位置     解析并绘制GIF
    if (gif.open(gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {      //打开并解析GIF
      Serial.printf("GIF打开成功哈!尺寸为：%d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());//打印并读取GIF文件的宽度和高度
      while (gif.playFrame(true, NULL)) {     //循环播放//为真则GIF播放循环，为假不循环
        needClearScreen = true;               // 设为真 下一帧需要清屏作用
        oled.display();                       // 清屏 确保当前帧完全显示
      }
      gif.close();                              //关闭GIF
      Serial.println("GIF播放结束,重新开始喽！...");
      oled.clearDisplay();                      //清屏  清空屏幕的内存缓存区                  //为黑色
      oled.display();                           //清屏  内存缓存区中的数据发送到物理 OLED 屏幕 //真清屏
      delay(500);
    } else {//打开失败时执行
      Serial.printf("打开GIF失败!错误码为：%d\n", gif.getLastError());//打印GIF文件打开失败
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.println("NO~GIF!");
      oled.display();
      while (1);
    }
}
