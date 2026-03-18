# IoT Lab — YoloUNO (ESP32-S3)

Arduino sketches for a 6-task IoT lab on the **YoloUNO** board (ESP32-S3). Each task is in its own folder for easy build and upload.

- **Requirements & deliverables:** [REQUIREMENTS.md](REQUIREMENTS.md)
- **Tasks:** LED Blinky (D13), RGB Blinky, DHT20 sensor, Web server, Tiny ML (Iris), Core IoT (MQTT + RPC)

---

## Getting started

### 1. Clone the repository

```bash
git clone https://github.com/minhducnt/iot-lab-yolouno.git
cd iot-lab-yolouno
```

### 2. Install Arduino IDE and board support

1. Install [Arduino IDE](https://www.arduino.cc/en/software) (or Arduino CLI).
2. Open **File → Preferences** and add these **Additional boards manager URLs** (one per line):

   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   https://raw.githubusercontent.com/AITT-VN/ohstem_arduino_board/refs/heads/main/package_ohstem_index.json
   ```

3. Go to **Tools → Board → Boards Manager**, search for **OhStem** and install **OhStem Boards by Ohstem Education** (or **esp32** by Espressif for a generic ESP32-S3).
4. Select **Tools → Board → Yolo UNO** (or your ESP32-S3 board).

### 3. Install required libraries

From **Sketch → Include Library → Manage Libraries** install:

| Library            | Used in tasks   |
|--------------------|-----------------|
| **Adafruit NeoPixel** | Task 2          |
| **DHT20** (Rob Tillaart) | 3, 4 (optional), 6 (optional) |
| **EloquentTinyML** | Task 5          |
| **tflm_esp32**     | Task 5          |

For **Task 6 (Core IoT)** use the library bundled with OhStem Boards, or install from ZIP: [coreiot-client-sdk](https://github.com/ohstem-public/coreiot-client-sdk).

### 4. Open a sketch and upload

- **File → Open** and choose the `.ino` file inside the task folder (e.g. `task_01_led_blinky/task_01_led_blinky.ino`).
- Select the correct **Port** (Tools → Port).
- Click **Upload**.

### 5. Configuration (where needed)

- **Task 2 (RGB):** If the onboard NeoPixel does not light, change `RGB_NEOPIXEL_PIN` in the sketch (try `48`, `38`, or `3` for an external Grove RGB on D3).
- **Tasks 4 & 6:** Set `WIFI_SSID` and `WIFI_PASSWORD` in the sketch.
- **Task 6:** Create a device on [Core IoT](https://coreiot.io), copy its **Access Token**, and set `TOKEN` in the sketch.

---

## Project structure

| Folder                | Description                          |
|-----------------------|--------------------------------------|
| `task_01_led_blinky`  | Blink onboard LED (D13)             |
| `task_02_rgb_blinky`  | RGB NeoPixel color cycle             |
| `task_03_dht20`       | DHT20 temperature & humidity (I2C)   |
| `task_04_webserver`   | WiFi + HTTP server (optional DHT20)  |
| `task_05_tiny_ml`     | TinyML Iris classifier (TFLite)      |
| `task_06_core_iot`    | Core IoT MQTT, telemetry, RPC (LED)  |

Each folder is a standalone Arduino sketch. Open the `.ino` file inside the folder.

---

## Requirements and lab report

See **[REQUIREMENTS.md](REQUIREMENTS.md)** for:

- Hardware (YoloUNO) and pin notes  
- Task list and deliverables (photos, code screenshots, descriptions)  
- Where to add GitSource (repository link) in your report  

---

## License

Use and adapt as needed for your course. If you reuse this repo, a link back or credit is appreciated.
