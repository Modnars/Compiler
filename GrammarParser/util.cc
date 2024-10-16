/*
 * @Author: modnarshen
 * @Date: 2024.10.13 17:53:04
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <cstdarg>
#include <cstdio>

#include "util.h"

namespace {

const char *RESET = "\e[0m";
const char *BLUE = "\e[0;34m";
const char *GREEN = "\e[0;32m";
const char *RED = "\e[0;31m";
const char *WHITE = "\e[1;37m";
const char *YELLOW = "\e[1;33m";

}  // namespace

namespace util {

bool IsVerboseMode = false;

void LOG_TRACE(const char *format, ...) {
    if (!IsVerboseMode) {
        return;
    }
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}

void LOG_INFO(const char *format, ...) {
    fprintf(stderr, "%s", GREEN);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "%s\n", RESET);
}

void LOG_WARN(const char *format, ...) {
    fprintf(stderr, "%s", YELLOW);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "%s\n", RESET);
}

void LOG_ERROR(const char *format, ...) {
    fprintf(stderr, "%s", RED);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "%s\n", RESET);
}

}  // namespace util
