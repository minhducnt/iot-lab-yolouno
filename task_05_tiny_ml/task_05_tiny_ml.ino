/**
 * Task 5 — Tiny ML (TensorFlow Lite Micro on device)
 *
 * Runs a small TFLite model (Iris flower classifier: 4 inputs -> 3 classes).
 * Interface: Serial Monitor @ 115200 — predicted class + inference time.
 * Optional: tie LED D13 to predicted class (short blink count = class + 1).
 *
 * Libraries (Library Manager):
 *   - EloquentTinyML
 *   - tflm_esp32
 *
 * Model: irisModel.h (bundled in this sketch folder)
 */
#include "irisModel.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>

#define ARENA_SIZE 8000

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;

void blinkClass(uint8_t c) {
  for (uint8_t i = 0; i <= c; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(120);
    digitalWrite(LED_BUILTIN, LOW);
    delay(120);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("=== Task 5: Tiny ML (Iris TFLite) ==="));

  tf.setNumInputs(4);
  tf.setNumOutputs(3);
  tf.resolver.AddFullyConnected();
  tf.resolver.AddSoftmax();

  while (!tf.begin(irisModel).isOk()) {
    Serial.println(tf.exception.toString());
    delay(1000);
  }
  Serial.println(F("Model loaded. Predicting x0, x1, x2 each second..."));
}

void loop() {
  if (!tf.predict(x0).isOk()) {
    Serial.println(tf.exception.toString());
  } else {
    Serial.print(F("Sample 0 -> class "));
    Serial.print(tf.classification);
    Serial.print(F("  ("));
    Serial.print(tf.benchmark.microseconds());
    Serial.println(F(" us)"));
    blinkClass(tf.classification);
  }
  delay(300);

  if (!tf.predict(x1).isOk()) {
    Serial.println(tf.exception.toString());
  } else {
    Serial.print(F("Sample 1 -> class "));
    Serial.println(tf.classification);
    blinkClass(tf.classification);
  }
  delay(300);

  if (!tf.predict(x2).isOk()) {
    Serial.println(tf.exception.toString());
  } else {
    Serial.print(F("Sample 2 -> class "));
    Serial.println(tf.classification);
    blinkClass(tf.classification);
  }

  delay(2000);
}
