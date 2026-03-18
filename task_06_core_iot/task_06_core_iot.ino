/**
 * Task 6 — Core IoT (MQTT telemetry + RPC from dashboard)
 *
 * - Server: app.coreiot.io:1883
 * - Set WIFI_SSID, WIFI_PASSWORD, TOKEN (device access token from CoreIoT)
 * - Telemetry: temperature, humidity (DHT20 if USE_DHT20, else random demo)
 * - RPC: setLedState (bool) toggles D13; shared attribute led_brightness (0-255)
 *
 * Library: Core IoT – Thingsboard (OhStem board package or coreiot-client-sdk ZIP)
 * Docs: https://coreiot.io/docs/ket-noi-va-gui-du-lieu/
 */
#include <WiFi.h>

#include <Arduino_MQTT_Client.h>
#include <Server_Side_RPC.h>
#include <Attribute_Request.h>
#include <Shared_Attribute_Update.h>
#include <ThingsBoard.h>

#define USE_DHT20 1
#if USE_DHT20
#include <Wire.h>
#include <DHT20.h>
DHT20 dht20;
bool g_dhtReady = false;
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";
constexpr char TOKEN[] = "YOUR_ACCESS_TOKEN";

constexpr char COREIOT_SERVER[] = "app.coreiot.io";
constexpr uint16_t COREIOT_PORT = 1883U;

constexpr uint16_t MAX_MESSAGE_SEND_SIZE = 256U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 256U;
constexpr size_t MAX_ATTRIBUTES = 3U;
constexpr uint64_t REQUEST_TIMEOUT_MICROSECONDS = 5000U * 1000U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr int16_t TELEMETRY_SEND_INTERVAL = 5000U;

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";

volatile bool ledState = false;
volatile int ledBrightness = 255;
volatile bool ledStateChanged = false;
volatile bool ledBrightessChanged = false;

constexpr const char LED_STATE_ATTR[] = "led_state";
constexpr const char LED_BRIGHTNESS_ATTR[] = "led_brightness";

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
Server_Side_RPC<3U, 5U> rpc;
Attribute_Request<2U, MAX_ATTRIBUTES> attr_request;
Shared_Attribute_Update<3U, MAX_ATTRIBUTES> shared_update;

const std::array<IAPI_Implementation *, 3U> apis = {&rpc, &attr_request, &shared_update};

constexpr std::array<const char *, 1U> SHARED_ATTRIBUTES_LIST = {LED_BRIGHTNESS_ATTR};
constexpr std::array<const char *, 1U> CLIENT_ATTRIBUTES_LIST = {LED_STATE_ATTR};

ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE,
               Default_Max_Stack_Size, apis);

uint32_t previousTelemetrySend = 0;

void InitWiFi() {
  Serial.println(F("Connecting to WiFi..."));
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("\nWiFi connected"));
}

bool reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  InitWiFi();
  return true;
}

void processSetLedState(const JsonVariantConst &data, JsonDocument &response) {
  ledState = data.as<bool>();
  Serial.print(F("RPC setLedState -> "));
  Serial.println(ledState ? F("ON") : F("OFF"));

  if (ledState) {
    analogWrite(LED_BUILTIN, ledBrightness);
  } else {
    analogWrite(LED_BUILTIN, 0);
  }

  StaticJsonDocument<64> response_doc;
  response_doc["newState"] = (int)ledState;
  response.set(response_doc);
  ledStateChanged = true;
}

const std::array<RPC_Callback, 1U> rpcCallbacks = {
    RPC_Callback{"setLedState", processSetLedState}};

void processSharedAttributes(const JsonObjectConst &data) {
  if (!data[LED_BRIGHTNESS_ATTR].isNull()) {
    int nb = data[LED_BRIGHTNESS_ATTR].as<int>();
    if (nb >= 0 && nb <= 255) {
      ledBrightness = nb;
      Serial.print(F("led_brightness = "));
      Serial.println(ledBrightness);
      ledBrightessChanged = true;
    }
  }
}

void processClientAttributes(const JsonObjectConst &data) {
  if (!data[LED_STATE_ATTR].isNull()) {
    ledState = data[LED_STATE_ATTR].as<bool>();
    if (ledState) {
      analogWrite(LED_BUILTIN, ledBrightness);
    } else {
      analogWrite(LED_BUILTIN, 0);
    }
  }
}

void requestTimedOut() {
  Serial.println(F("Attribute request timeout"));
}

const Shared_Attribute_Callback<MAX_ATTRIBUTES> attributes_callback(
    &processSharedAttributes, SHARED_ATTRIBUTES_LIST.cbegin(),
    SHARED_ATTRIBUTES_LIST.cend());
const Attribute_Request_Callback<MAX_ATTRIBUTES> attribute_shared_request_callback(
    &processSharedAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
    SHARED_ATTRIBUTES_LIST);
const Attribute_Request_Callback<MAX_ATTRIBUTES> attribute_client_request_callback(
    &processClientAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
    CLIENT_ATTRIBUTES_LIST);

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
#if defined(ESP32)
  analogWrite(LED_BUILTIN, 0);
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif

#if USE_DHT20
  Wire.begin();
  g_dhtReady = dht20.begin();
  if (g_dhtReady) {
    Serial.println(F("DHT20 OK"));
  } else {
    Serial.println(F("DHT20 not detected — using random telemetry"));
  }
#endif

  delay(1000);
  InitWiFi();
}

void loop() {
  delay(10);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    Serial.print(F("MQTT connect "));
    Serial.print(COREIOT_SERVER);
    Serial.print(F(" ... "));
    if (!tb.connect(COREIOT_SERVER, TOKEN, COREIOT_PORT)) {
      Serial.println(F("failed"));
      delay(3000);
      return;
    }
    Serial.println(F("Connected to Core IoT"));

    tb.sendAttributeData("mac_address", WiFi.macAddress().c_str());
    tb.sendAttributeData("ip_address", WiFi.localIP().toString().c_str());
    tb.sendAttributeData("ssid", WiFi.SSID().c_str());

    if (!rpc.RPC_Subscribe(rpcCallbacks.cbegin(), rpcCallbacks.cend())) {
      Serial.println(F("RPC subscribe failed"));
      return;
    }
    if (!shared_update.Shared_Attributes_Subscribe(attributes_callback)) {
      Serial.println(F("Shared attributes subscribe failed"));
      return;
    }
    if (!attr_request.Shared_Attributes_Request(attribute_shared_request_callback)) {
      Serial.println(F("Shared attributes request failed"));
    }
    if (!attr_request.Client_Attributes_Request(attribute_client_request_callback)) {
      Serial.println(F("Client attributes request failed"));
    }
  }

  if (millis() - previousTelemetrySend > (uint32_t)TELEMETRY_SEND_INTERVAL) {
    float temperature;
    float humidity;

#if USE_DHT20
    if (g_dhtReady) {
      int st = dht20.read();
      if (st == 0) {
        temperature = dht20.getTemperature();
        humidity = dht20.getHumidity();
      } else {
        temperature = random(200, 400) / 10.0f;
        humidity = random(500, 1000) / 10.0f;
      }
    } else {
      temperature = random(200, 400) / 10.0f;
      humidity = random(500, 1000) / 10.0f;
    }
#else
    temperature = random(200, 400) / 10.0f;
    humidity = random(500, 1000) / 10.0f;
#endif

    if (!isnan(temperature)) {
      tb.sendTelemetryData(TEMPERATURE_KEY, temperature);
      tb.sendTelemetryData(HUMIDITY_KEY, humidity);
      Serial.print(F("Telemetry: T="));
      Serial.print(temperature);
      Serial.print(F(" H="));
      Serial.println(humidity);
    }
    tb.sendAttributeData("rssi", WiFi.RSSI());
    previousTelemetrySend = millis();
  }

  if (ledStateChanged || ledBrightessChanged) {
    ledStateChanged = false;
    ledBrightessChanged = false;
#if defined(ESP32)
    if (ledState) {
      analogWrite(LED_BUILTIN, ledBrightness);
    } else {
      analogWrite(LED_BUILTIN, 0);
    }
#endif
    tb.sendAttributeData(LED_STATE_ATTR, ledState);
  }

  tb.loop();
}
