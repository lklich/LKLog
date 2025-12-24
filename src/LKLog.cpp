/*
 * Copyright (C) 2025 Leszek Klich
 * * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LKLog.h"

#if defined(ESP32)
    #include "esp_log.h"
#elif defined(ARDUINO_ARCH_RP2040)
    // PICO SDK specific header
    #include <stdio.h>
#endif

LKLog &LKLog::getInstance() {
    static LKLog instance;
    return instance;
}

void LKLog::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
    _native_esp = false;
    // Wait for UART port (very important for USB CDC), max 2 seconds
    unsigned long start = millis();
    while (!Serial && (millis() - start < 2000)) {
        delay(10); 
    }
    delay(100);  
}


void LKLog::log(LogLevel level, const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog(level, tag, format, args);
    va_end(args);
}


void LKLog::logInfo(const char *tag, const char *format, ...) {
#if defined(ARDUINO_ARCH_AVR)
  char buffer[128]; 
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  // Sending formatted text to main log method
  this->log(LOG_INFO, tag, "%s", buffer);
#else
    va_list args; 
    va_start(args, format);
    //this->log(LOG_INFO, tag, format, args);
    vlog(LOG_INFO, tag, format, args);
    va_end(args);
#endif
}


void LKLog::logWarning(const char *tag, const char *format, ...) {
  #if defined(ARDUINO_ARCH_AVR)
  char buffer[128]; 
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  // Sending formatted text to main log method
  this->log(LOG_WARNING, tag, "%s", buffer);
#else
    va_list args; va_start(args, format);
    //this->log(LOG_WARNING, tag, format, args);
    vlog(LOG_WARNING, tag, format, args);
    va_end(args);
#endif
}

void LKLog::logError(const char *tag, const char *format, ...) {
  #if defined(ARDUINO_ARCH_AVR)
   char buffer[128]; 
   va_list args;
   va_start(args, format);
   vsnprintf(buffer, sizeof(buffer), format, args);
   va_end(args);
   // Sending formatted text to main log method
   this->log(LOG_ERROR, tag, "%s", buffer);
  #else
    va_list args; va_start(args, format);
    //this->log(LOG_ERROR, tag, format, args);
    vlog(LOG_ERROR, tag, format, args);
    va_end(args);
  #endif
}

void LKLog::setNativeESP(bool native){
  #if defined(ESP32)
    _native_esp = native;
    #else
        logInfo("LKLOG", "Only for ESP32!");  
    #endif
}

const char *LKLog::levelToString(LogLevel level) {
    switch (level) {
        case LOG_ERROR:   return "ERROR";
        case LOG_WARNING: return "WARNING";
        case LOG_INFO:    return "INFO";
        case LOG_DEBUG:   return "DEBUG";
        case LOG_VERBOSE: return "VERBOSE";
        default:          return "LOG";
    }
}

void LKLog::vlog(LogLevel level, const char *tag, const char *format, va_list args) {
#if defined(ESP32)
    if(_native_esp) {
        esp_log_level_t esp_level;
        switch (level) {
            case LOG_ERROR:   esp_level = ESP_LOG_ERROR;   break;
            case LOG_WARNING: esp_level = ESP_LOG_WARN;    break;
            case LOG_INFO:    esp_level = ESP_LOG_INFO;    break; 
            default:          esp_level = ESP_LOG_INFO;    break;
        }
        esp_log_writev(esp_level, tag, format, args);
        printf("\n");
    } else {
        printf("[%s][%s]: ", levelToString(level), tag);   
        vprintf(format, args);
        printf("\n");
    }
#else
    #if defined(ARDUINO_ARCH_AVR)
        const uint16_t bufferSize = 128;
    #else
        const uint16_t bufferSize = 256;
    #endif

    char buffer[bufferSize]; 
    vsnprintf(buffer, sizeof(buffer), format, args);   
    Serial.print("[");
    Serial.print(levelToString(level));
    Serial.print("][");
    Serial.print(tag);
    Serial.print("] ");
    Serial.println(buffer);
#endif
}