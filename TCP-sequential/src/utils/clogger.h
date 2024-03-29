/**
Copyright 2015 Christopher Schneider

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

/**
 * Author's note: Suitable for small projects that don't want the huge overhead of installing
 * third party logging libraries (e.g. Apache Log4cpp)
 *
 * Capabilities are limited intentionally.
 *
 * Call just like printf
 * e.g. LogE("Oh noes, call tech support! Their number is %s", "unknown");
 *
 * LogLevel | Definition | Description
 *    4         LogI         Info
 *    3         LogD         Debug
 *    2         LogW         Warn
 *    1         LogE         Error
 *
 * Log level 4 enables all logs, log level 1 only enables error logs
 * log level 0 disables all logs.
 *
 */
#ifndef CLOGGER_H
#define CLOGGER_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h> /* va_list, va_start, va_arg, va_end */
#include <sys/time.h>
#include <time.h>

/**
 * Log colors
 */
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define RESET "\033[0m"

/**Log levels**/
#define CCS_LOG_LEVEL_MORE_VERBOSE 6 //For stupid stuff that nobody needs to see
#define CCS_LOG_LEVEL_VERBOSE 5
#define CCS_LOG_LEVEL_INFO 4
#define CCS_LOG_LEVEL_DEBUG 3
#define CCS_LOG_LEVEL_WARN 2
#define CCS_LOG_LEVEL_ERROR 1
#define CCS_LOG_LEVEL_OFF 0

#define BUF_SIZE 256

/**Settings**/

#define CCS_LOGS_LEVEL_DEFAULT CCS_LOG_LEVEL_DEBUG
#define CCS_LOGS_LEVEL CCS_LOG_LEVEL_MORE_VERBOSE
#define CCS_LOGS_FILENAME "logs/TCP-SEQUENTIAL.log"

//Max log entry size. If you exceed this, you either need to shorten your logs
//or you should be using a more robust logging system :-)
#define CCS_CLogger_BUFFER_SIZE 1024
#define CCS_CLogger_LOGGER_FORMAT_DEFAULT "[%s] \t %s \t %s%s\n"
#define CCS_CLogger_LOGGER_FORMAT_RED KRED CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT_YEL KYEL CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT_GRN KGRN CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT_NRM KNRM CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT_BLU KBLU CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT_MAG KMAG CCS_CLogger_LOGGER_FORMAT_DEFAULT RESET
#define CCS_CLogger_LOGGER_FORMAT CCS_CLogger_LOGGER_FORMAT_DEFAULT
extern void setLogLevel(int);
extern long long getTimeMilliseconds();
extern void initLogger();
extern void makeMessage(FILE *output, const char *tag, const char *func, const int line, const char *format, ...);
FILE *logFile;

/**empty defines if logging disabled**/
#define logE(fmt, ...)
#define logW(fmt, ...)
#define logD(fmt, ...)
#define logI(fmt, ...)
#define logV(fmt, ...)
#define logMV(fmt, ...)

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_ERROR)
#undef logE
#define logE(fmt, ...) makeMessage(stdout, "ERROR", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_WARN)
#undef logW
#define logW(fmt, ...) makeMessage(stdout, "WARN", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_DEBUG)
#undef logD
#define logD(fmt, ...) makeMessage(stdout, "DEBUG", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_INFO)
#undef logI
#define logI(fmt, ...) makeMessage(stdout, "INFO", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_VERBOSE)
#undef logV
#define logV(fmt, ...) makeMessage(stdout, "VERBOSE", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if (CCS_LOGS_LEVEL >= CCS_LOG_LEVEL_MORE_VERBOSE)
#undef logMV
#define logMV(fmt, ...) makeMessage(stdout, "MORE", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#endif //CCS_CLOGGER_H
