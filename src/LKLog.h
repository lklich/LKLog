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

/**
 * @file LKLog.h
 * @author Leszek Klich (leszek.klich@gmail.com)
 * @brief Universal log system for ESP32, ESP8266, AVR and Raspberry Pico.
 * @version 1.1.0
 * @date 2025-12-24
 * @copyright Copyright (c) 2025
 * Licensed under the terms provided in the LICENSE file.
 **/


#ifndef LKLOG_H
#define LKLOG_H

#include <Arduino.h>
#include <stdarg.h> 

/**
 * @brief Log levels available in the library.
 * * These levels correspond to the severity of the logged event.
 */
enum LogLevel {
    LOG_NONE,     /**< No log */
    LOG_ERROR,    /**< Critical errors, usually requiring immediate attention */
    LOG_WARNING,  /**< Important events that are not necessarily errors */
    LOG_INFO,     /**< General system information and status updates */
    LOG_DEBUG,    /**< Detailed information for developers during debugging */
    LOG_VERBOSE   /**< Extremely detailed trace logs */
};

/**
 * @class LKLog
 * @brief Singleton class for cross-platform logging.
 * * This class provides a unified API for logging across different hardware 
 * architectures. It integrates with native ESP-IDF logging on ESP32 
 * and falls back to Serial-based logging on AVR and other platforms.
 */
class LKLog {
public:
    /**
     * @brief Gets the Singleton instance of the logger.
     * * @return LKLog& Reference to the static LKLog instance.
    */
    static LKLog &getInstance();

    /**
     * @brief Initializes the Serial communication.
     * * Includes a timeout-based wait for USB CDC ports (like ESP32-C3 or Pico).
     * @param baudRate Communication speed (default: 115200).
    */
    void begin(unsigned long baudRate = 115200);

    /**
     * @brief Main logging method with printf-style formatting.
     * * @param level The severity level of the log (LogLevel).
     * @param tag A short string identifying the module (e.g., "WIFI", "BATT").
     * @param format The format string (standard printf specifiers).
     * @param ... Variable arguments for the format string.
    */
    void log(LogLevel level, const char *tag, const char *format, ...);
    
    /**
     * @brief Log an informational message.
     * * @param tag Module tag.
     * @param format Printf-style format string.
     * @param ... Variable arguments.
    */
    void logInfo(const char *tag, const char *format, ...);

    /**
     * @brief Log a warning message.
     * * @param tag Module tag.
     * @param format Printf-style format string.
     * @param ... Variable arguments.
    */
    void logWarning(const char *tag, const char *format, ...);

    /**
     * @brief Log an error message.
     * * @param tag Module tag.
     * @param format Printf-style format string.
     * @param ... Variable arguments.
    */
    void logError(const char *tag, const char *format, ...);

    /**
     * @brief Switche to native ESP32 log type.
     * @param ... true or false.
     */
    void setNativeESP(bool native);

private:
    /**
     * @brief Gets the Singleton instance of the logger.
     * * @return LKLog& Reference to the static LKLog instance.
     */
    LKLog() {}
    LKLog(const LKLog&) = delete;
    LKLog &operator=(const LKLog&) = delete;
    const char *levelToString(LogLevel level);
    void vlog(LogLevel level, const char* tag, const char* format, va_list args);
    bool _native_esp;
};

#endif