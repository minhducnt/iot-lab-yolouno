/**
 * Task 3 — DHT20 temperature & humidity (I2C)
 * Wiring: VCC 3.3V, GND, SDA, SCL (Grove I2C or STEMMA QT)
 * Library: DHT20 by Rob Tillaart
 */
#include <Wire.h>
#include <DHT20.h>

DHT20 dht;

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin();
  if (!dht.begin()) {
    Serial.println("DHT20 init failed — check I2C wiring (addr 0x38)");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("DHT20 OK — readings every 2 s");
}

void loop() {
  int status = dht.read();
  if (status == 0) {  // DHT20_OK
    Serial.print("Temperature: ");
    Serial.print(dht.getTemperature(), 2);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(dht.getHumidity(), 2);
    Serial.println(" %RH");
  } else {
    Serial.print("Read error: ");
    Serial.println(status);
  }
  delay(2000);
}
