# Báo cáo thực hành IoT — YoloUNO (ESP32-S3)

> **Hướng dẫn:** Tài liệu này chứa toàn bộ ghi chú kỹ thuật cho từng task. Ảnh chụp sẽ được bổ sung vào các chỗ có nhãn `[📷 ẢNH]`.

---

## Thông tin phần cứng

| Thành phần | Chi tiết |
|-----------|---------|
| **Board** | YoloUNO (ESP32-S3, Arduino Uno form factor) |
| **Vi xử lý** | ESP32-S3 Dual Core 240 MHz Tensilica |
| **Bộ nhớ** | 16 MB Flash, 8 MB PSRAM |
| **Kết nối** | WiFi 802.11b/g/n, Bluetooth 5 / BLE |
| **Đèn tích hợp** | LED nguồn, LED chân D13, RGB NeoPixel |
| **Cổng Grove** | 4× Analog, 4× Digital, 4× I2C |
| **Cổng khác** | STEMMA QT / QWIIC (I2C), 4 chân GVS Servo |

---

## Task 1: Chớp tắt đèn D13 (LED Blinky)

### Mô tả

Điều khiển đèn LED tích hợp trên chân D13 của YoloUNO nhấp nháy liên tục với chu kỳ 500 ms bật — 500 ms tắt. Đây là chương trình "Hello World" của lập trình nhúng, dùng để xác nhận môi trường Arduino IDE đã được cài đặt đúng và board đang hoạt động bình thường.

### Code

**File:** `task_01_led_blinky/task_01_led_blinky.ino`

```cpp
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
```

### Giải thích

| Dòng lệnh | Chức năng |
|----------|----------|
| `pinMode(LED_BUILTIN, OUTPUT)` | Khai báo chân D13 là OUTPUT để xuất tín hiệu điều khiển LED |
| `digitalWrite(HIGH)` | Cấp điện cho LED → đèn sáng |
| `delay(500)` | Giữ trạng thái 500 ms |
| `digitalWrite(LOW)` | Ngắt điện cho LED → đèn tắt |

`LED_BUILTIN` là macro được định nghĩa sẵn trong Arduino/OhStem board package, trỏ đến chân D13. Dùng `LED_BUILTIN` thay vì giá trị số cứng giúp code portable sang board khác.

### Cải tiến / thay đổi

- Thêm `#ifndef LED_BUILTIN` để đảm bảo chạy được ngay cả khi board package chưa định nghĩa macro này.
- Có thể cải tiến thêm bằng cách thay `delay()` bằng `millis()` để không block vòng lặp chính (non-blocking blink).

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| Hệ thống chạy trên YoloUNO (đèn D13 đang sáng) | `[📷 ẢNH — chụp board YoloUNO khi đèn D13 nhấp nháy]` |
| Code chạy thực tế | `[📷 ẢNH — screenshot code trong Arduino IDE / Cursor]` |

---

## Task 2: Chớp tắt đèn RGB (RGB Blinky)

### Mô tả

Điều khiển đèn RGB NeoPixel tích hợp trên YoloUNO chuyển màu liên tục theo vòng: **Đỏ → Xanh lá → Xanh dương → Tắt**, mỗi màu duy trì 400 ms. Sử dụng thư viện **Adafruit NeoPixel** để gửi tín hiệu 1-wire WS2812.

### Code

**File:** `task_02_rgb_blinky/task_02_rgb_blinky.ino`

```cpp
#include <Adafruit_NeoPixel.h>

#ifndef RGB_NEOPIXEL_PIN
#define RGB_NEOPIXEL_PIN 48
#endif
#define NUM_PIXELS 1

Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixel.begin();
  pixel.setBrightness(40);
  pixel.clear();
  pixel.show();
}

void loop() {
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));  // Red
  pixel.show(); delay(400);
  pixel.setPixelColor(0, pixel.Color(0, 255, 0));  // Green
  pixel.show(); delay(400);
  pixel.setPixelColor(0, pixel.Color(0, 0, 255));  // Blue
  pixel.show(); delay(400);
  pixel.clear();
  pixel.show(); delay(400);
}
```

### Giải thích

| Thành phần | Chức năng |
|-----------|----------|
| `Adafruit_NeoPixel pixel(1, PIN, NEO_GRB)` | Khởi tạo đối tượng NeoPixel: 1 LED, chân điều khiển, định dạng màu GRB 800 kHz |
| `pixel.begin()` | Khởi tạo chân GPIO |
| `pixel.setBrightness(40)` | Giảm độ sáng (0–255) tránh chói và tiêu tốn điện quá mức |
| `pixel.setPixelColor(index, color)` | Đặt màu cho LED thứ `index`; `Color(R,G,B)` tạo màu từ 3 kênh 0–255 |
| `pixel.show()` | Gửi dữ liệu màu ra đèn thực (bắt buộc gọi sau khi đặt màu) |
| `pixel.clear()` | Tắt tất cả LED |

**Lưu ý pin:** Chân NeoPixel trên board phụ thuộc vào phiên bản. Nếu đèn không sáng, thay `RGB_NEOPIXEL_PIN` thành `38`. Với module RGB ngoài kết nối vào cổng Grove Digital D3, dùng `#define RGB_NEOPIXEL_PIN 3`.

### Cải tiến / thay đổi

- Đặt `setBrightness(40)` để bảo vệ mắt khi dùng đèn onboard.
- Dùng `#define` cho pin để dễ thay đổi mà không cần sửa code chính.
- Có thể nâng cấp thêm hiệu ứng fade (tăng/giảm dần) bằng cách chạy vòng lặp `for` tăng dần R/G/B.

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| YoloUNO đang chạy (đèn RGB đỏ / xanh / lam) | `[📷 ẢNH — chụp 1–3 trạng thái màu khác nhau]` |
| Screenshot code | `[📷 ẢNH — màn hình Arduino IDE]` |

---

## Task 3: DHT20 — Đọc nhiệt độ & độ ẩm

### Mô tả

Kết nối cảm biến DHT20 qua giao tiếp I2C, đọc nhiệt độ và độ ẩm mỗi 2 giây, in kết quả ra Serial Monitor. DHT20 sử dụng giao thức I2C với địa chỉ cố định **0x38**.

### Sơ đồ kết nối

| Chân DHT20 | Kết nối |
|-----------|--------|
| VCC (1) | 3.3 V |
| SDA (2) | SDA của YoloUNO (Grove I2C hoặc STEMMA QT) |
| GND (3) | GND |
| SCL (4) | SCL của YoloUNO |

### Code

**File:** `task_03_dht20/task_03_dht20.ino`

```cpp
#include <Wire.h>
#include <DHT20.h>

DHT20 dht;

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin();
  if (!dht.begin()) {
    Serial.println("DHT20 init failed — check I2C wiring (addr 0x38)");
    while (true) { delay(1000); }
  }
  Serial.println("DHT20 OK — readings every 2 s");
}

void loop() {
  int status = dht.read();
  if (status == 0) {
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
```

### Giải thích

| Hàm | Chức năng |
|-----|---------|
| `Wire.begin()` | Khởi tạo bus I2C |
| `dht.begin()` | Kết nối với DHT20 tại địa chỉ 0x38; trả về `true` nếu thành công |
| `dht.read()` | Gửi lệnh đo và chờ kết quả (~80 ms); trả về `0` nếu OK |
| `dht.getTemperature()` | Lấy giá trị nhiệt độ đã đo (°C) |
| `dht.getHumidity()` | Lấy giá trị độ ẩm đã đo (%RH) |

**Serial output mẫu:**
```
DHT20 OK — readings every 2 s
Temperature: 26.45 °C  |  Humidity: 68.30 %RH
Temperature: 26.47 °C  |  Humidity: 68.25 %RH
```

### Cải tiến / thay đổi

- Thêm xử lý lỗi: nếu `begin()` thất bại thì vòng lặp vô hạn với thông báo, không crash silent.
- In 2 chữ số thập phân (`.2`) để kết quả dễ đọc.
- Có thể cải tiến thêm: thêm cảnh báo khi nhiệt độ hoặc độ ẩm vượt ngưỡng.

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| YoloUNO + DHT20 đang chạy | `[📷 ẢNH — board và cảm biến đã nối dây]` |
| Serial Monitor hiển thị giá trị đo | `[📷 ẢNH — screenshot Serial Monitor]` |

---

## Task 4: WebServer

### Mô tả

YoloUNO kết nối WiFi (chế độ STA — station) và khởi chạy HTTP server trên cổng 80. Khi người dùng mở trình duyệt nhập địa chỉ IP của board, trang web trả về trạng thái thiết bị và (nếu có DHT20) nhiệt độ / độ ẩm thời gian thực, tự động làm mới mỗi 3 giây.

### Chức năng

| Chức năng | Chi tiết |
|----------|---------|
| WiFi STA | Kết nối vào router/hotspot bằng SSID + password |
| HTTP GET `/` | Trả về trang HTML với trạng thái, địa chỉ IP, và số liệu cảm biến |
| Auto-refresh | `<meta http-equiv='refresh' content='3'>` làm mới trang mỗi 3 giây |
| DHT20 tùy chọn | `#define USE_DHT20 1/0` bật/tắt đọc cảm biến |

### Giao diện web

```
┌─────────────────────────────┐
│      YoloUNO WebServer      │
│  Status: online             │
│  IP: 192.168.x.xxx          │
│  Temperature: 26.45 °C      │
│  Humidity:    68.30 %RH     │
│                             │
│  [Page auto-refreshes 3 s]  │
└─────────────────────────────┘
```

### Cấu hình

Trước khi nạp code, sửa trong file `.ino`:

```cpp
const char *WIFI_SSID     = "TEN_WIFI_CUA_BAN";
const char *WIFI_PASSWORD = "MAT_KHAU_WIFI";
#define USE_DHT20 1   // 0 nếu không có cảm biến
```

### Code (tóm tắt)

**File:** `task_04_webserver/task_04_webserver.ino`

```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", buildPage());
}

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

`buildPage()` xây dựng chuỗi HTML động, nhúng địa chỉ IP và giá trị cảm biến thực tế.

### Giải thích

| Thành phần | Chức năng |
|-----------|---------|
| `WebServer server(80)` | Khởi tạo HTTP server lắng nghe cổng 80 |
| `WiFi.begin(...)` | Kết nối WiFi theo chế độ station |
| `server.on("/", callback)` | Đăng ký handler cho đường dẫn `/` |
| `server.handleClient()` | Xử lý request đến (phải gọi liên tục trong `loop()`) |
| `server.send(200, "text/html", html)` | Trả về HTTP 200 với nội dung HTML |

### Cải tiến / thay đổi

- Sử dụng `WebServer` (built-in ESP32) thay vì `WiFiServer` cấp thấp để đơn giản hóa xử lý HTTP.
- Thêm `meta refresh` cho phép giám sát liên tục không cần F5.
- Chia `buildPage()` thành hàm riêng để dễ mở rộng thêm trang (e.g. `/sensor`, `/status`).

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| Giao diện web trên trình duyệt | `[📷 ẢNH — screenshot trang web hiển thị trong Chrome/Firefox]` |
| Serial Monitor hiển thị địa chỉ IP | `[📷 ẢNH — screenshot Serial Monitor sau khi kết nối WiFi]` |
| YoloUNO đang chạy | `[📷 ẢNH — board thực tế]` |

---

## Task 5: Tiny ML

### Mô tả

Chạy mô hình machine learning nhỏ (TensorFlow Lite Micro) trực tiếp trên chip ESP32-S3, **không cần server/cloud**. Mô hình demo: phân loại hoa Iris (4 đặc trưng đầu vào → 3 lớp) với kích thước ~5 KB. Kết quả được in ra Serial Monitor và D13 LED nhấp theo số lần bằng class được dự đoán.

### Chức năng

| Chức năng | Chi tiết |
|----------|---------|
| Load model | Load file `irisModel.h` (TFLite FlatBuffer) vào bộ nhớ |
| Inference | Dự đoán lớp của 3 mẫu dữ liệu Iris (x0, x1, x2) |
| Serial output | In class dự đoán + thời gian inference (microseconds) |
| LED feedback | D13 nhấp N lần = class N (class 0 → 1 lần, class 1 → 2 lần, ...) |

### Thư viện cần cài

1. **EloquentTinyML** — từ Library Manager (tác giả: eloquentarduino)
2. **tflm_esp32** — TensorFlow Lite Micro runtime cho ESP32

### Code (tóm tắt)

**File:** `task_05_tiny_ml/task_05_tiny_ml.ino`

```cpp
#include "irisModel.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>

#define ARENA_SIZE 8000
Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;

void setup() {
  tf.setNumInputs(4);
  tf.setNumOutputs(3);
  tf.resolver.AddFullyConnected();
  tf.resolver.AddSoftmax();
  tf.begin(irisModel);
}

void loop() {
  tf.predict(x0);
  Serial.print("Sample 0 -> class ");
  Serial.print(tf.classification);
  Serial.print(" in ");
  Serial.print(tf.benchmark.microseconds());
  Serial.println(" us");
  // ... predict x1, x2
  delay(2000);
}
```

### Giải thích

| Thành phần | Chức năng |
|-----------|---------|
| `irisModel.h` | Mô hình TFLite ở dạng `unsigned char[]`, tổng ~5 KB |
| `ARENA_SIZE 8000` | Bộ nhớ tĩnh cấp phát cho tensor arena (~8 KB) |
| `tf.begin(irisModel)` | Khởi tạo interpreter TFLite, parse model |
| `tf.predict(x)` | Chạy inference với mảng float 4 phần tử |
| `tf.classification` | Index của class có xác suất cao nhất |
| `tf.benchmark.microseconds()` | Thời gian chạy inference |

**Output mẫu trên Serial:**
```
=== Task 5: Tiny ML (Iris TFLite) ===
Model loaded. Predicting x0, x1, x2 each second...
Sample 0 -> class 0  (8450 us)
Sample 1 -> class 1
Sample 2 -> class 2
```

**Ý nghĩa class Iris:**
- Class 0: Iris setosa
- Class 1: Iris versicolor
- Class 2: Iris virginica

### Về mô hình

Mô hình Iris là mạng fully-connected 3 lớp được huấn luyện bằng Keras/TensorFlow, sau đó convert sang TFLite và export dưới dạng C header bằng công cụ [everywhereml](https://github.com/eloquentarduino/everywhereml). Trọng số (~5 KB) được nhúng trực tiếp vào flash.

### Cải tiến / thay đổi

- `ARENA_SIZE` được tăng lên 8000 (so với 2000 mặc định trong ví dụ) để đảm bảo đủ bộ nhớ trên ESP32-S3.
- Thêm hàm `blinkClass()` để phản hồi kết quả inference qua đèn LED vật lý.
- Có thể mở rộng sau: thay `irisModel.h` bằng mô hình từ Edge Impulse (e.g. phân loại cử chỉ, nhận dạng âm thanh).

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| Serial Monitor hiển thị kết quả inference | `[📷 ẢNH — screenshot Serial Monitor với các dòng "Sample X -> class Y"]` |
| D13 LED nhấp nháy | `[📷 ẢNH — board đang chạy, đèn D13 sáng]` |
| Screenshot code | `[📷 ẢNH — code trong Arduino IDE]` |

---

## Task 6: Core IoT

### Mô tả

Kết nối YoloUNO tới nền tảng **Core IoT** ([coreiot.io](https://coreiot.io)) qua giao thức MQTT. Board:
- Gửi **telemetry** (nhiệt độ, độ ẩm) lên dashboard mỗi 5 giây.
- Nhận **lệnh từ dashboard** qua RPC để bật/tắt đèn D13 và điều chỉnh độ sáng.
- Đồng bộ **trạng thái** qua shared attributes.

### Kiến trúc

```
YoloUNO ──(WiFi)──► app.coreiot.io:1883 (MQTT)
    │                        │
    │  telemetry (T, H)       │
    │──────────────────────► Dashboard (biểu đồ)
    │                        │
    │◄────────────────────── RPC: setLedState(true/false)
    │  shared attr: led_brightness
```

### Chuẩn bị trên CoreIoT

1. Đăng ký tại [coreiot.io](https://coreiot.io)
2. Tạo thiết bị mới → sao chép **Access Token**
3. Tạo Dashboard với widget:
   - **Value card**: hiển thị `temperature`, `humidity`
   - **Line chart**: vẽ lịch sử nhiệt độ
   - **Button RPC**: gọi `setLedState` với `true`/`false`
4. Thêm widget **Shared attribute control** cho `led_brightness`

### Cấu hình code

```cpp
constexpr char WIFI_SSID[]     = "TEN_WIFI_CUA_BAN";
constexpr char WIFI_PASSWORD[] = "MAT_KHAU_WIFI";
constexpr char TOKEN[]         = "ACCESS_TOKEN_TU_COREIOT";
```

### Code (tóm tắt)

**File:** `task_06_core_iot/task_06_core_iot.ino`

```cpp
// Kết nối MQTT
tb.connect(COREIOT_SERVER, TOKEN, COREIOT_PORT);

// Gửi telemetry mỗi 5 giây
tb.sendTelemetryData("temperature", temperature);
tb.sendTelemetryData("humidity", humidity);

// Xử lý RPC từ dashboard
void processSetLedState(const JsonVariantConst &data, JsonDocument &response) {
  ledState = data.as<bool>();
  analogWrite(LED_BUILTIN, ledState ? ledBrightness : 0);
}
```

### Giải thích các thành phần

| Thành phần | Chức năng |
|-----------|---------|
| `ThingsBoard tb(...)` | Client MQTT tương thích ThingsBoard/CoreIoT |
| `tb.connect(server, token, port)` | Kết nối MQTT với access token làm username |
| `tb.sendTelemetryData(key, value)` | Gửi 1 cặp key-value lên stream telemetry |
| `tb.sendAttributeData(key, value)` | Cập nhật thuộc tính thiết bị (metadata) |
| `RPC_Subscribe(callbacks)` | Đăng ký lắng nghe lệnh RPC từ dashboard |
| `Shared_Attributes_Subscribe(cb)` | Nhận cập nhật shared attribute (e.g. độ sáng) |
| `tb.loop()` | Xử lý tin nhắn đến (phải gọi liên tục) |

**Serial output mẫu:**
```
WiFi connected
MQTT connect app.coreiot.io ... Connected to Core IoT
DHT20 OK
Telemetry: T=26.5 H=68.3
Telemetry: T=26.6 H=68.1
RPC setLedState -> ON
led_brightness = 128
```

### Thuộc tính đồng bộ

| Tên | Loại | Hướng | Chức năng |
|-----|------|-------|----------|
| `temperature` | Telemetry | Board → Server | Nhiệt độ (°C) |
| `humidity` | Telemetry | Board → Server | Độ ẩm (%RH) |
| `rssi` | Attribute | Board → Server | Cường độ WiFi |
| `ip_address` | Attribute | Board → Server | Địa chỉ IP hiện tại |
| `led_state` | Client attribute | Hai chiều | Trạng thái đèn bật/tắt |
| `led_brightness` | Shared attribute | Server → Board | Độ sáng đèn (0–255) |

### Cải tiến / thay đổi

- Telemetry dùng DHT20 thực tế (khi `USE_DHT20 1`); fallback về giá trị ngẫu nhiên nếu cảm biến không kết nối.
- Xử lý tái kết nối WiFi và MQTT tự động khi mất kết nối.
- Gửi thêm `rssi`, `mac_address`, `ssid` làm attribute để giám sát tình trạng mạng trên dashboard.

### Ảnh minh chứng

| Nội dung | Ảnh |
|---------|-----|
| Dashboard CoreIoT (biểu đồ nhiệt độ, độ ẩm) | `[📷 ẢNH — screenshot trang dashboard trên coreiot.io]` |
| YoloUNO kết nối server và nhận lệnh | `[📷 ẢNH — board thực tế + Serial Monitor hiển thị "Connected" và "RPC received"]` |

---

## GitSource

| Mục | Thông tin |
|-----|---------|
| **Repository** | https://github.com/minhducnt/iot-lab-yolouno |
| **Branch** | `main` |
| **Ngôn ngữ** | C++ (Arduino / PlatformIO) |

**Cấu trúc thư mục:**
```
iot-lab-yolouno/
├── README.md               ← Hướng dẫn cài đặt & chạy
├── REQUIREMENTS.md         ← Yêu cầu & deliverables
├── LAB_NOTES.md            ← Tài liệu này
├── task_01_led_blinky/
├── task_02_rgb_blinky/
├── task_03_dht20/
├── task_04_webserver/
├── task_05_tiny_ml/
└── task_06_core_iot/
```

---

## Tóm tắt thư viện sử dụng

| Thư viện | Phiên bản | Cài đặt | Task |
|---------|----------|--------|------|
| Arduino / ESP32 core | ≥ 2.0 | Boards Manager | Tất cả |
| **Adafruit NeoPixel** | ≥ 1.11 | Library Manager | 2 |
| **DHT20** (Rob Tillaart) | ≥ 0.3.2 | Library Manager | 3, 4, 6 |
| **WebServer** (built-in) | — | Đã có trong ESP32 core | 4 |
| **EloquentTinyML** | ≥ 2.x | Library Manager | 5 |
| **tflm_esp32** | matching version | Library Manager | 5 |
| **Core IoT – ThingsBoard** | ≥ 4.x | OhStem board pkg / ZIP | 6 |
