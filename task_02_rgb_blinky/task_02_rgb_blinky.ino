/**
 * Task 2 — RGB NeoPixel blink / color cycle
 * YoloUNO: onboard WS2812-style LED. If nothing lights up, change RGB_NEOPIXEL_PIN
 * (common tries: 48, 38). External Grove RGB on D3: set to 3.
 *
 * Library: Adafruit NeoPixel (Library Manager)
 */
#include <Adafruit_NeoPixel.h>

#ifndef RGB_NEOPIXEL_PIN
#define RGB_NEOPIXEL_PIN 48
#endif

#define NUM_PIXELS 1

Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixel.begin();
  pixel.setBrightness(40);  // avoid eye-searing onboard LED
  pixel.clear();
  pixel.show();
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  pixel.show();
  delay(400);
  pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  pixel.show();
  delay(400);
  pixel.setPixelColor(0, pixel.Color(0, 0, 255));
  pixel.show();
  delay(400);
  pixel.clear();
  pixel.show();
  delay(400);
}
