# IoT Lab — Requirements (YoloUNO)

This document summarizes lab deliverables and technical notes. Source code lives in separate task folders (see [README.md](README.md)).

## Hardware

- **Board:** YoloUNO (ESP32-S3, Arduino Uno–style pinout)
- **Onboard:** power LED, **D13** user LED, **RGB NeoPixel** (WS2812-style)
- **Connectors:** Grove (Analog / Digital / I2C), STEMMA QT / QWIIC

## Tasks and deliverables

| Task | Name (VI / EN) | What to submit |
|------|----------------|----------------|
| 1 | Chớp tắt đèn D13 / LED Blinky | Photo of board running; screenshot of source code; short note on improvements/changes |
| 2 | Chớp tắt đèn RGB / RGB Blinky | Same as Task 1 |
| 3 | DHT20 | Same as Task 1 |
| 4 | WebServer | Photo(s); describe **functions** and **web interface** |
| 5 | Tiny ML | Photo(s); describe **functions** and **interface** (e.g. Serial / LED feedback) |
| 6 | Core IOT | Screenshot of **CoreIoT Dashboard**; photo of YoloUNO online and **receiving commands** from server |
| — | **GitSource** | Link to your Git repository with this project |

## Technical notes

| Topic | Detail |
|-------|--------|
| **D13 LED** | Use `LED_BUILTIN` (maps to D13 on YoloUNO). |
| **RGB NeoPixel** | Onboard data pin may vary by board revision. Sketch uses a `#define` you can change; external Grove RGB modules often use **D3** (see [OhStem Tiny RGB](https://docs.ohstem.vn/en/latest/yolo_uno/yolo_uno_khoi_lenh/hien_thi/tiny_rgb.html)). Library: **Adafruit NeoPixel**. |
| **DHT20** | I2C address **0x38**. Library: **DHT20** by Rob Tillaart. Connect to 3.3 V, GND, SDA, SCL (Grove I2C or STEMMA QT). |
| **Web server** | WiFi STA + HTTP on port **80**; optional DHT20 on same page (`USE_DHT20` in sketch). |
| **Tiny ML** | TensorFlow Lite Micro via **EloquentTinyML** + **tflm_esp32**; Iris classifier demo (~5 KB model). |
| **Core IoT** | MQTT to **app.coreiot.io:1883**; device **access token**; RPC e.g. `setLedState`. Docs: [Kết nối và gửi dữ liệu](https://coreiot.io/docs/ket-noi-va-gui-du-lieu/). SDK: [coreiot-client-sdk](https://github.com/ohstem-public/coreiot-client-sdk) (often bundled with OhStem boards package). |

## Where to put screenshots in your report

- Paste **photos** and **code screenshots** into your lab template (e.g. Word) next to each task.
- For Task 6, add **Dashboard screenshot** and **hardware photo** showing connection/command behavior.
- Add your **Git repository URL** under **GitSource**.
