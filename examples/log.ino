#include <Arduino.h>
#include <LKLog.h>

static const char *TAGMAIN = "CUSTOMNAME";

void setup() {
  delay(2000);
  LKLog::getInstance().begin(115200);
  // Wymuszenie poziomu logowania globalnie na INFO (3)
  esp_log_level_set("*", ESP_LOG_INFO);
  // Sprawdźmy co widzi system
  Serial.printf("Current ESP Log Level: %d\n", esp_log_level_get("*")); 
  LKLog::getInstance().logInfo("TEST", "To jest test INFO");
  LKLog::getInstance().logError("TEST", "To jest test ERROR");

  LKLog::getInstance().logInfo("SYSTEM", "System started");
  LKLog::getInstance().logWarning("WiFi", "Try connect to WiFi...");
  LKLog::getInstance().logError("SENSOR", "DHT22 error");
  LKLog::getInstance().logError(TAGMAIN, "Tag name example");
  Serial.println("TEST");
}

void loop() {
    float temp = 24.55;
    int hum = 60;
    int id = 1;

    // Logging with variables
    LKLog::getInstance().logInfo("SENSOR", "Read ID: %d | Temp: %.2f C | Hum: %d%%", id, temp, hum);  
    delay(5000);
}
