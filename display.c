#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>

#include "display.h"

void pred(const char* msg, ...){
    printf(ANSI_COLOR_RED);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void predh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

void pblue(const char* msg, ...){
    printf(ANSI_COLOR_BLUE);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void pblueh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

void pylw(const char* msg, ...){
    printf(ANSI_COLOR_YELLOW);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void pylwh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

void pgrn(const char* msg, ...){
    printf(ANSI_COLOR_GREEN);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void pgrnh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

void pcyn(const char* msg, ...){
    printf(ANSI_COLOR_CYAN);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void pcynh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

void pmgn(const char* msg, ...){
    printf(ANSI_COLOR_MAGENTA);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
    printf(ANSI_COLOR_RESET);
}

void pmgnh(const char *header, const char* msg, ...){
    printf(ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET, header);
    va_list(args);
    va_start(args, msg);
    vprintf(msg, args);
}

#ifdef DEBUG
void dbg2(const char* msg, ...){
#else
void dbg(const char* msg, ...){
#endif
    printf(ANSI_FONT_BOLD);
    printf(ANSI_COLOR_GREEN "\n[Debug] ");
    printf(ANSI_COLOR_RESET);
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
}

#ifdef DEBUG
void info2(const char* msg, ...){
#else
void info(const char* msg, ...){
#endif
    printf(ANSI_FONT_BOLD);
    printf(ANSI_COLOR_BLUE "\n[Info] ");
    printf(ANSI_COLOR_RESET);
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
}

#ifdef DEBUG
void err2(const char* msg, ...){
#else
void err(const char* msg, ...){
#endif
    printf(ANSI_FONT_BOLD);
    printf(ANSI_COLOR_RED "\n[Error] ");
    printf(ANSI_COLOR_RESET);
    va_list args;
    va_start(args, msg);
    vprintf(msg,args);
}

#ifdef DEBUG
void warn2(const char* msg, ...){
#else
void warn(const char* msg, ...){
#endif
    printf(ANSI_FONT_BOLD);
    printf(ANSI_COLOR_YELLOW "\n[Warning] ");
    printf(ANSI_COLOR_RESET);
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
}
