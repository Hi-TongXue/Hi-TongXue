/*==========================================================================
  == 哔哩哔哩ID：620162563                                                ===
  ==========================================================================
*/


#include <Adafruit_NeoPixel.h>
#define PIN 1     //板载RGB
#define PIN2 3    //外接RGB
#define NUMPIXELS 1        //RGB数量
#define NUMPIXELS2 12      //RGB数量

Adafruit_NeoPixel pixels(  NUMPIXELS,   PIN,  NEO_GRB + NEO_KHZ800); //板载RGB
Adafruit_NeoPixel pixels2( NUMPIXELS2,  PIN2, NEO_GRB + NEO_KHZ800); //外接RGB

int r = 0, g = 0, b = 0;
uint16_t count1 = 0;

int count2 = 0;
int B = 100;

bool U_zhuanhuan = false;
bool D_zhuanhuan = false;

void RGB_setup_1() {
  pixels.begin();
  pixels2.begin();

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

  pixels2.fill( pixels2.Color(0, 0, 0), 0);
  pixels2.show();
  pixels2.setBrightness(100);
}

void RGB_loop_1() {//板载RGB  三色单个变化
 static unsigned long last1 = 0;
 if (millis() - last1 > 1100)
 {
    count2++;
    if (count2 > 3)
    {
        count2 = 0;
    }
    last1 = millis();
 }

 if (count2 == 0)
 {
    r = 50; 
    g = 0; 
    b = 0; 
 }
 else if (count2 == 1)
 {
    r = 0; 
    g = 50; 
    b = 0; 
 }
 else if (count2 == 2)
 {
    r = 0; 
    g = 0; 
    b = 50; 
 }
 else if (count2 == 3)
 {
    r = 50; 
    g = 50; 
    b = 50; 
 }
 
 pixels.setPixelColor(0, pixels.Color(r, g, b));   //板载RGB
 pixels.show();
}

void RGB_loop_2() {  //外接RGB 白光可设置亮度
    count1 = count1 + 512;
    pixels2.rainbow(count1);
    pixels2.show();
}

void RGB_loop_2_1() {  //外接RGB 彩色循环
   //pixels2.setPixelColor(0, 100, 100, 100);
   pixels2.fill(pixels.Color(100, 100, 100), 0, NUMPIXELS2);
   pixels2.show();
}
