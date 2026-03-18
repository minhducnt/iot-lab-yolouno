/**
 * Task 4 — Web server on YoloUNO (ESP32)
 *
 * Functions:
 *   - Connects to WiFi (STA)
 *   - HTTP server on port 80
 *   - Single-page HTML: device title, IP, optional live DHT20 readings
 *
 * Interface: open http://<device-ip>/ in a browser.
 *
 * Set USE_DHT20 to 0 if no sensor connected.
 */
#include <WiFi.h>
#include <WebServer.h>

// ---- WiFi (edit) ----
const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define USE_DHT20 1

#if USE_DHT20
#include <Wire.h>
#include <DHT20.h>
DHT20 dhtSensor;
#endif

WebServer server(80);

String buildPage() {
  String h = F("<!DOCTYPE html><html><head><meta charset='utf-8'>"
               "<meta http-equiv='refresh' content='3'>"
               "<title>YoloUNO WebServer</title>"
               "<style>body{font-family:system-ui,sans-serif;max-width:520px;margin:2rem auto;}"
               "h1{color:#0a6;} .box{background:#f4f8f6;padding:1rem;border-radius:8px;}</style>"
               "</head><body>");
  h += F("<h1>YoloUNO WebServer</h1>");
  h += F("<div class='box'><p><b>Status:</b> online</p>");
  h += F("<p><b>IP:</b> ");
  h += WiFi.localIP().toString();
  h += F("</p>");
#if USE_DHT20
  int st = dhtSensor.read();
  if (st == 0) {
    h += F("<p><b>Temperature:</b> ");
    h += String(dhtSensor.getTemperature(), 2);
    h += F(" &deg;C</p><p><b>Humidity:</b> ");
    h += String(dhtSensor.getHumidity(), 2);
    h += F(" %RH</p>");
  } else {
    h += F("<p><i>DHT20 read error (");
    h += String(st);
    h += F(")</i></p>");
  }
#else
  h += F("<p><i>Sensor data: connect DHT20 and set USE_DHT20 to 1</i></p>");
#endif
  h += F("</div><p style='color:#888;font-size:12px;'>Page auto-refreshes every 3 s.</p></body></html>");
  return h;
}

void handleRoot() {
  server.send(200, "text/html", buildPage());
}

void setup() {
  Serial.begin(115200);
  delay(300);

#if USE_DHT20
  Wire.begin();
  if (!dhtSensor.begin()) {
    Serial.println("DHT20 not found — WebServer still runs without sensor data");
  }
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Open http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
