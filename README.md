# LKLog

A lightweight, high-performance logging library for the Arduino framework. Designed for **ESP32**, **ESP8266**, **PICO** and **Atmega (AVR)**.

## License

- license type: GNU GPL v3
- author: Leszek Klich (2025) 
- e-mail: leszek.klich@gmail.com

## Key Features

* **Native ESP-IDF Support**: On ESP32, it uses the official `esp_log.h` system, providing colored output and system-level integration.
* **Printf-style Formatting**: Log variables easily without messy string concatenation (e.g., `%d`, `%f`, `%s`).
* **Singleton Pattern**: Global access from any file in your project without passing object references.
* **Cross-Platform**: Consistent API across 32-bit and 8-bit microcontrollers.

## Log levels

* LOG_ERROR - Critical issues.
* LOG_WARNING - Important events that are not errors.
* LOG_INFO - General system information.
* LOG_DEBUG - Verbose information for development.
* LOG_VERBOSE - Detailed trace logs.

## Installation

### STM32

```
void setup() {
  LKLog::getInstance().begin(115200);
  delay(1000);
}

void loop() {
    float temp = 24.55;
    int hum = 60;
    int id = 1;
    LKLog::getInstance().logWarning("SENSOR", "ID: %ld | Temp: %.2f | Hum: %ld", (long)id, (double)temp, (long)hum);
    delay(5000);
}
```

platformio.ini

```
[env:nucleo_f401re]
platform = ststm32
board = nucleo_f401re
monitor_dtr = 1
monitor_rts = 1
monitor_speed = 115200
build_flags = 
    -Wl,-u,_printf_float
    -Wl,-u,_scanf_float
```


### Atmel AVR

platformio.ini

```
[env:atmega_env]
platform = atmelavr
monitor_speed = 9600
board = megaatmega2560
; Enable float type for AVR in printf/sprintf
build_flags = -Wl,-u,vfprintf -lprintf_flt -lm
```

```
void setup() {
  LKLog::getInstance().begin(9600);
  LKLog::getInstance().logInfo("TEST", "To jest test INFO");
  LKLog::getInstance().logError("TEST", "To jest test ERROR");
  LKLog::getInstance().logInfo("SYSTEM", "System started");
  LKLog::getInstance().logWarning("WiFi", "Try connect to WiFi...");
  LKLog::getInstance().logError("SENSOR", "DHT22 error");
  LKLog::getInstance().logError(TAGMAIN, "Tag name example");
  Serial.println("TEST");
}
```

### Raspberry PICO

```
void setup() {
  delay(2000);
  LKLog::getInstance().begin(115200);
  LKLog::getInstance().logInfo("TEST", "To jest test INFO");
  LKLog::getInstance().logError("TEST", "To jest test ERROR");

  LKLog::getInstance().logInfo("SYSTEM", "System started");
  LKLog::getInstance().logWarning("WiFi", "Try connect to WiFi...");
  LKLog::getInstance().logError("SENSOR", "DHT22 error");
  LKLog::getInstance().logError(TAGMAIN, "Tag name example");
}
```

### ESP32 C3 Supermini
 
platformio.ini

```
[env:esp32_espmini]
platform = espressif32
board = esp32-c3-devkitm-1
build_flags = 
    -D CORE_DEBUG_LEVEL=3
    -D LOG_LOCAL_LEVEL=ESP_LOG_INFO
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
monitor_dtr = 1
monitor_rts = 1
```

```
void setup() {
  delay(2000); // IMPORTANT!
  LKLog::getInstance().begin(115200);
  // Force global log level to INFO (3)
  esp_log_level_set("*", ESP_LOG_INFO);
  // Check log level
  Serial.printf("Current ESP Log Level: %d\n", esp_log_level_get("*")); 
  LKLog::getInstance().logInfo("TEST", "To jest test INFO");
  LKLog::getInstance().logWarning("WiFi", "Try connect to WiFi...");
  LKLog::getInstance().logError("SENSOR", "DHT22 error");
}
```

### ESP8266 Node MCU V3

platformio.ini

```
[env:esp8266_env]
platform = espressif8266
board = nodemcuv2
```

### ESP32 - setNativeESP(bool)

Switche between standard and native log style (only for ESP32).

**Example of standard style (printf)**
```
[INFO][SENSOR]: Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
[ERROR][SENSOR1]: Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
[WARNING][SENSOR2]: Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
```

**Example of native log style (esp_log_writev)**
```
Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
Read ID: 1073422800 | Temp: 1.70 C | Hum: 12%
```

### PlatformIO
Add the following to your `platformio.ini`:
```ini
lib_deps = 
    LKLog