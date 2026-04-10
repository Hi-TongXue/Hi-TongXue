#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define PIN_LEDS 21
#define NUMPIXELS 194

#define LIGHTNESS 0.05f
#define BRIGHTNESS 64

RgbColor BLACK(0, 0, 0);
RgbColor RED(BRIGHTNESS, 0, 0);
RgbColor GREEN(0, BRIGHTNESS, 0);
RgbColor BLUE(0, 0, BRIGHTNESS);
RgbColor YELLOW(BRIGHTNESS, BRIGHTNESS, 0);
RgbColor WHITE(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(NUMPIXELS, PIN_LEDS);

NeoPixelAnimator animations(10); // NeoPixel animation management object

#define NUM_RINGS 11
#define RING_MAX_PIXELS 26
#define RING_MIN_PIXELS 8

// all rings starts at 0 ray
byte RINGS[NUM_RINGS][RING_MAX_PIXELS] = {
  {78, 79, 80, 81, 82, 83, 84, 77}, // 8
  {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 63, 64}, // 14
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 45, 46, 47}, // 18
  {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 25, 26, 27}, // 20
  {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 1, 2, 3, 4}, // 24
  {102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 110, 109, 108, 107, 106, 105, 104, 103}, // 26
  {127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 134, 133, 132, 131, 130, 129, 128}, // 24
  {148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 154, 153, 152, 151, 150, 149}, // 20
  {167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 172, 171, 170, 169, 168}, // 18
  {182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 186, 185, 184, 183}, // 14
  {192, 191, 190, 189, 188, 187, 194, 193} // 8
};

byte RING_SIZES[NUM_RINGS] = {8, 14, 18, 20, 24, 26, 24, 20, 18, 14, 8};

unsigned long startMillis = 0;
short animation = 0;

void setup() {
  Serial.begin(115200);
  initRandom();

  leds.Begin();
  leds.ClearTo(BLACK);
  leds.Show();

  //animations.StartAnimation(0, 6000, rainbowAnimation);
  animations.StartAnimation(0, 2000, raysRainbow);
  //animations.StartAnimation(0, 2000, ringsRainbow);
  //animations.StartAnimation(0, 500, xmasOrbAnimation);
  //animations.StartAnimation(0, 1000, ringAnimation);
  //animations.StartAnimation(0, 500, rayAnimation);
  //animations.StartAnimation(0, 100, randomAnimation);
}

void loop() {
  animations.UpdateAnimations();

  leds.Show();
  delay(10);

  if (false && (startMillis == 0 || startMillis + 20000 < millis())) { // 30s
    startMillis = millis();
    switch (animation) {
      case 0: 
        animations.StartAnimation(0, 500, rayAnimation);
        break;
      case 1: 
        animations.StartAnimation(0, 1000, ringAnimation);
        break;
      case 2: 
        animations.StartAnimation(0, 2000, ringsRainbow);
        break;
      case 3: 
        animations.StartAnimation(0, 2000, raysRainbow);
        break;
      case 4: 
        animations.StartAnimation(0, 6000, rainbowAnimation);
        break;
      case 5:
        animations.StartAnimation(0, 100, randomAnimation);
        animation = -1;
        break;
    }
    animation ++;
  }
}

void randomAnimation(const AnimationParam& param) {
  float hue;
  HslColor color;

  if (param.state == AnimationState_Completed) {
    for (byte i = 0; i < 194; i ++) {
      hue = random(0, 1000) / 1000.0f;
      color = HslColor(hue, 1.0f, LIGHTNESS);
      leds.SetPixelColor(i, color);
    }

    animations.RestartAnimation(0);
  }
}

void rainbowAnimation(const AnimationParam& param) {
  HslColor color = HslColor(param.progress, 1.0f, LIGHTNESS);
  leds.ClearTo(color);

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void raysRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < RING_MAX_PIXELS; i++) {
    hue = param.progress + (float) i / (float) RING_MAX_PIXELS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    rayColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringsRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < NUM_RINGS; i++) {
    hue = param.progress + (float) i / (float) NUM_RINGS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    ringColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void xmasOrbAnimation(const AnimationParam& param) {
  ringColor(0, WHITE);
  ringColor(1, RED);
  ringColor(2, RED);
  ringColor(3, RED);
  ringColor(4, RED);
  ringColor(5, WHITE);
  ringColor(6, RED);
  ringColor(7, RED);
  ringColor(8, RED);
  ringColor(9, RED);
  ringColor(10, WHITE);

  byte offset = round(param.progress);
  for (byte i = offset; i < RING_SIZES[3]; i+=2) {
    leds.SetPixelColor(RINGS[3][i] - 1, WHITE);
  }
  for (byte i = offset; i < RING_SIZES[7]; i+=2) {
    leds.SetPixelColor(RINGS[7][i] - 1, WHITE);
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);

  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, BLUE);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, BLUE);
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void rayAnimation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);

  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, BLUE);
  rayColor(index + (RING_MAX_PIXELS / 2), BLUE);

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void rayColor(byte rayIndex, RgbColor color) {
  int pixelIndex;
  byte pixel;

  if (rayIndex >= RING_MAX_PIXELS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < NUM_RINGS; i ++) {
    pixelIndex = round((float) RING_SIZES[i] / (float) RING_MAX_PIXELS * rayIndex);
    pixel = RINGS[i][pixelIndex];
    if (pixel == 0) {
      continue; // skip condition
    }
    leds.SetPixelColor(pixel - 1, color); // index starts from 1 (0 is stop condition)
  }
}

void ringColor(byte ringIndex, RgbColor color) {
  byte pixel;

  if (ringIndex >= NUM_RINGS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < RING_MAX_PIXELS; i ++) {
    pixel = RINGS[ringIndex][i];
    if (pixel == 0) {
      return; // end condition
    }
    leds.SetPixelColor(pixel - 1, color); // index starts from 1 (0 is stop condition)
  }
}

void initRandom() {
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  uint32_t seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}

RgbColor colorWheel(byte wheelPos) {
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return RgbColor(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return RgbColor(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return RgbColor(wheelPos * 3, 255 - wheelPos * 3, 0);
}